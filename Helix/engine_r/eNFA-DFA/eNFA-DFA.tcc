#include "FSM.h"
#include <string>
#include <cmath>
#include <assert.h>
#include <vector>
#include <iostream>
#include <set>
#include <map>

#include "Run.h"



template<typename T>
void print(std::set<T> s) {
	std::cout << "{";
	for (auto e: s) {
		std::cout << e << ", ";
	}
	std::cout << "}\n";
}


template<	typename StateT,
			typename SymbolT,
			SymbolT epsilon>
DFA<StateT, SymbolT> MSSC(eNFA<StateT, SymbolT, epsilon>& N) {
	DFA<StateT, SymbolT> D;
	
	D.sigma = N.sigma;
	D.q0 = 0;  // by definition the first state
	
	std::map<std::set<int>, int> states;
	std::set<int> start = N.ECLOSE(N.q0);
	states[start] = 0;
	recursive_add(D, N, states, start);
	
	D.num_states = states.size();
// 	std::cout << "states.size = " << states.size() << "\n";
	
	/*
	for (auto state: states) {
		D.map.left.insert(typename DFA<StateT,SymbolT>::Bimap::left_value_type(state.second, "whatever"));
		std::cout << state.second << "\n";
	}
	*/
	
	typedef typename DFA<StateT, SymbolT>::Bimap::value_type insert_type;
	
	for (auto it: states) {
		bool final = false;
		std::string s = "{";
		for (auto subs: it.first) {
			s += N.realState(subs) + ",";
			if (N.isFinal(subs)) {
				final = true;
			}
		}
		s += "}";
		D.map.insert(insert_type(it.second, s));
		
		if (final) D.F.insert(it.second);
	}
	
	return D;
}


template<	typename StateT,
			typename SymbolT,
			SymbolT epsilon>
void recursive_add(DFA<StateT, SymbolT>& D, eNFA<StateT, SymbolT, epsilon>& N,
		std::map<std::set<int>, int>& states, std::set<int>& current) {
	Walker<eNFA<StateT, SymbolT, epsilon>> w(N, current);
	
	for (SymbolT symb: D.sigma) {
		w.input(symb);
		//print(w.current);
		auto wtf = states.find(w.current);
		if (wtf == states.end()) {
			states[w.current] = states.size()-1; // this -1 is a *bit* weird
			recursive_add(D, N, states, w.current);
		} else {
			//print(wtf->first);
		}
		
		D.set_delta(states.at(current), symb, states.at(w.current));
		
		w.current = current; // reset
	}
}




/*
// Returns which bits are on in the integer a
std::vector<int> getOnLocations(int a) {
	std::vector<int> result;
	int place = 0;
	while (a != 0) {
		if (a & 1) {
			result.push_back(place);
		}
		++place;
		a >>= 1;
	}
	return result;
}*/


/*
 * This one uses the powerset much more literally
 * 
template<	typename StateT,
			typename SymbolT,
			SymbolT epsilon>
DFA<StateT, SymbolT> MSSC_old(eNFA<StateT, SymbolT, epsilon> N) {
	DFA<StateT, SymbolT> D;

	D.sigma = N.sigma;
	D.q0 = 0;
	
	// state with number i in D:
	// i in binary for example: 00..0010001
	//   -> that would mean the set containing the 0'th and 4'th set of N.

	// The NFA cannot have more than (sizeof(int)*8) - 1 states!
	assert(N.num_states <= (sizeof(int) * 8 - 1));

	D.num_states = static_cast<int>(pow(2.0, static_cast<double>(N.num_states)));

	// For every set in the powerset
	for (int i = 0; i < D.num_states; i++) {
		std::vector<int> from_states = getOnLocations(i);
		std::string from_name = "{";
		
		// For every symbol in the alphabet
		for (auto symb: D.sigma) {
			int to_set = 0; // means {}
			
			// For every state in the set
			for (int state: from_states) {
				// Skip undefined stuff
				if (N.d_data.find(state) == N.d_data.end() || N.d_data[state].find(symb) == N.d_data[state].end()) {
					continue;
				}
				
				// Add all the states
				for (int to_state: N.d_data[state][symb]) {
					for (int e_to_state: N.ECLOSE(to_state)) {
						// flip the e_to_state-th bit in to_set
						to_set |= 1 << e_to_state;
					}
				}
			}
			
			// now define D.d_data[i][symb]
			D.set_delta(i, symb, to_set);
// 			std::cout << "d[" << i << "][" << symb << "] = " << to_set << "\n";
		}
		
		// Also give this state in D a proper name
		for (int state: from_states) {
			from_name += N.realState(state) + ", ";
		}
		from_name += "}";
		
		D.map.left.insert(s_eNFA::Bimap::left_value_type(i, from_name));
// 		std::cout << "[" << i << "]: " << from_name << "\n";
	}
	
	// Set q0
	for (int state: N.ECLOSE(N.q0)) {
// 		std::cout << state << "\n";
		D.q0 |= 1 << state;
	}
	
// 	std::cout << "eNFA start state is " << N.q0 << "\n";
// 	std::cout << "DFA Start state is " << D.q0 << "\n";
// 	std::cout << "I created a DFA with " << D.num_states << " states.\n";
	D.lazy_evaluation();
// 	std::cout << "Optimized version only contains " << D.num_states << " states.\n";
	return D;
}
*/
