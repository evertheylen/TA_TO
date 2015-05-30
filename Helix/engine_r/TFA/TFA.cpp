#include "TFA.h"
#include <vector>
#include <iostream>
using namespace std;

// Table

int Table::loc(int k, int l) {
	// n is the number of states
	
	if (k > l) {
		uint temp = l;
		l = k;
		k = temp;
	}
	
	// k <= l from here on
	
	// (n(n+1))/2 - ((n-k)(n-k+1))/2 + (l-i-1)
	// smaller:
	
	int result = (2*num_states*k - k*k - k + 2*l - 2)/2;
	//std::cout << "getting/setting (" << k << ", " << l << ") = " << result << "\n";
	return result;
}


bool Table::get(int k, int l) {
	return data[loc(k,l)];
}

void Table::set(int k, int l, bool val) {
	data.at(loc(k,l)) = val;
}


Table::Table(int num):
		num_states(num) {
	int N = (num_states*(num_states+1))/2;
	
	data = std::vector<bool>(N, false);
}

void Table::print(ostream& out, s_DFA& D) {
	out << "\t";
	for (int ss=1; ss<num_states; ss++) {
		out << D.realState(ss) << "\t";
	}
	out << "\n";
	
	for (int s=0; s<num_states; s++) {
		out << D.realState(s) << ":\t";
		for (int pre_s=0; pre_s<s; pre_s++) {
			out << "\t";
		}
		for (int ss=s+1; ss<num_states; ss++) {
			if (get(s,ss)) out << "X\t";
			else out << "O\t";
		}
		out << "\n";
	}
}

bool operator==(Table& lhs, Table& rhs) {
	return lhs.data == rhs.data;
}

bool operator!=(Table& lhs, Table& rhs) {
	return lhs.data != rhs.data;
}



bool block::operator==(block const& b) {
	if (b.state1 == this->state1 and b.state2 == this->state2) {
		return true;
	} else return false;
}

// name is weird :p
bool block::hasFinal(s_DFA D) {
	if (D.isFinal(state1) == true xor D.isFinal(state2) == true) return true;
	else return false;
}
// ^ equivalent to D.isFinal(state1) != D.isFinal(state2)


//this erases the same blocks in a vector
void filter(vector<block>& table_vec) {
	vector<block> copy_vec = table_vec;
	for (int i =0; i < table_vec.size(); i++) {
		for (int e=0; e < copy_vec.size(); e++) {
			if (table_vec.at(i) == copy_vec.at(e)) {
				table_vec.erase(table_vec.begin() + i);
			}
		}
	}
	return;
}

vector<block> makeTable(s_DFA D) {
	vector<block> table_vec;
	for (int e=0; e <= D.num_states+1; e++) {
		for (int i=0; i<= D.num_states+1; i++) {
			if (i == e) continue;
			block tempblock(e,i);
			if (tempblock.hasFinal(D) == true) tempblock.dist = true;
			table_vec.push_back(tempblock);
		}
		filter(table_vec);
	}
	return table_vec;
}


void makePairs(s_DFA D, vector<block> table_vec) {
	for (vector<block>::iterator it = table_vec.begin(); it != table_vec.end(); ++it) {
		for (char a: D.sigma) {
			int nextState1 = D.delta(it->state1,a);
			int nextState2 = D.delta(it->state2,a);
			if (D.isFinal(nextState1) != D.isFinal(nextState2)) {
				it->dist = true;
				break;
			}
		}
	}
}

vector<block> removeDist(vector<block> table_vec) {
	vector<block> new_vector;
	for (vector<block>::iterator it = table_vec.begin(); it != table_vec.end(); ++it) {
		if (it->dist == false) {
			new_vector.push_back(*it);
		}
	}
	return new_vector;
}

s_DFA makeNewDFA(s_DFA D, vector<block> table) {
	s_DFA mini;
	for (int i=0; i < table.size(); i++) {
		block tempBlock = table.at(i);
		string new_name = "{" + D.realState(tempBlock.state1) + ", " + D.realState(tempBlock.state2) + "}";
		mini.add_state(new_name, false);
		for (char a: D.sigma) {
			int old1 = D.delta(tempBlock.state1, a);
			int old2 = D.delta(tempBlock.state2, a);
			string tempName = "{" + D.realState(old1) + ", " + D.realState(old2) + "}";
			mini.set_delta(D.ID(new_name),a,D.ID(tempName)); // waar gaat mijn nieuwe delta naartoe? naar de combinatie van old1 en old2 OPGELOST!
		}
	}
	return mini;
}

s_DFA TFA(s_DFA D) {
	/*
	int n = D.num_states+1;
	// Step 0 --> should work
	D.lazy_evaluation();
	
	// Step 1: create the table and mark all the pairs with a final and a nonfinal state
	// rather simple, should work
	vector<block> table = makeTable(D);
	
	// Step 2: mark all the rest and build the equivalent 'block'
	// ...
	makePairs(D,table);
	
	table = removeDist(table);
	// Step 3: reconstruct DFA based on the new 'blocks'
	s_DFA optimusDFA = makeNewDFA(D, table);
	return optimusDFA;
	*/
	
	D.lazy_evaluation();
	
	// Create the table
	Table table(D.num_states);
	table.print(std::cout, D);
	// Mark base cases
	for (int k=0; k < D.num_states; k++) {
		for (int l=k+1; l < D.num_states; l++) {
			if (D.isFinal(l) != D.isFinal(k)) {
				table.set(l,k,true);
			}
		}
	}
	std::cout << "\nafter base cases:\n";
	table.print(std::cout, D);
	
	// mark all the rest
	bool changed = false;
	do {
		changed = false;
		for (int k=0; k < D.num_states; k++) {
			for (int l=k+1; l < D.num_states; l++) {
				if (!table.get(l,k)) for (char a: D.sigma) {
					int k_to = D.delta(k, a);
					int l_to = D.delta(l, a);
					
					// if (one of) the pairs of states the TFA is going to is distinguishable,
					// this pair is too.
					if (table.get(l_to, k_to)) {
						table.set(l, k, true);
						changed = true;
						break;
					}
				}
			}
		}
		
		std::cout << "--------------------------------\n";
		table.print(std::cout, D);
	} while (changed);
	
	
	// Build groups
	std::vector<int> groups(D.num_states, 0);
	int max = 0;
	for (int s=0; s<D.num_states; s++) {
		// if this state is equivalent to another state with a lower number,
		// the group of this state is going to be the group of that lower
		// state.
		// more precisely, we'll search the highest equivalent state lower than s.
		int highest = -1;
		for (int ss=0; ss<s; ss++) {
			if (!table.get(s,ss)) {
				highest = ss;
			}
		}
		
		if (highest != -1) {
			groups[s] = groups[highest];
		} else {
			groups[s] = max;
			max++;
		}
	}
	
	s_DFA optimus_prime;
	optimus_prime.num_states = max;
	
	std::vector<std::string> tempnames(optimus_prime.num_states, "{");
	
	for (int s=0; s<D.num_states; s++) {
		int grouped_s = groups[s];
		for (char a: D.sigma) {
			optimus_prime.set_delta(grouped_s, a, groups[D.delta(s,a)]);
		}
		if (D.isFinal(s)) {
			optimus_prime.F.insert(grouped_s);
		}
		tempnames.at(grouped_s) += D.realState(s);
		tempnames.at(grouped_s) += ", ";
	}
	
	optimus_prime.q0 = groups[D.q0];
	
	for (int ns=0; ns<optimus_prime.num_states; ns++) {
		tempnames[ns] += "}";
		optimus_prime.map.insert(s_DFA::Bimap::value_type(ns, tempnames[ns]));
	}
	
	return optimus_prime;
}
