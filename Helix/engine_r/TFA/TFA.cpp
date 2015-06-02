#include "TFA.h"
#include "Run.h"
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

s_DFA TFA(s_DFA D, bool print) {
	D.lazy_evaluation();
	// Create the table
	Table table(D.num_states);
	if (print) {
	    table.print(std::cout, D);
    }
	// Mark base cases
	for (int k=0; k < D.num_states; k++) {
		for (int l=k+1; l < D.num_states; l++) {
			if (D.isFinal(l) != D.isFinal(k)) {
				table.set(l,k,true);
			}
		}
	}
	if (print) {
	    std::cout << "[TFA] after base cases:\n";
	    table.print(std::cout, D);
    }
	
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
		
		if (print) {
		    std::cout << "[TFA] table:\n";
		    table.print(std::cout, D);
	    }
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
	optimus_prime.sigma = D.sigma;
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
    
    for (int s=0; s<D.num_states; s++) {
        int grouped_s = groups[s];
        if(tempnames.at(grouped_s).back() == ' '){
            tempnames.at(grouped_s).pop_back();
            tempnames.at(grouped_s).pop_back();
        }
    }
	
	optimus_prime.q0 = groups[D.q0];
	
	for (int ns=0; ns<optimus_prime.num_states; ns++) {
		tempnames[ns] += "}";
		optimus_prime.map.insert(s_DFA::Bimap::value_type(ns, tempnames[ns]));
	}
    //Runner<s_DFA> r(D);
    //Runner<s_DFA> rr(optimus_prime);
    //bool accepted = r.process("00110");
    //bool accepted2 = r.process("11011000");
    //bool accepted3 = r.process("011000");
    //cout << "accepted? " << accepted << endl;
    //cout << "accepted2? " << accepted2 << endl;
    //cout << "accepted3? " << accepted3 << endl;
	return optimus_prime;
}
