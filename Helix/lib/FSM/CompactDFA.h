
#ifndef _H_CompactDFA
#define _H_CompactDFA

#include <vector>

#include "FSM.h"

class CompactDFA {
public:
	
	int num_states;
	std::vector<char> sigma;
	int q0;
	std::vector<std::vector<int>> d_data;
	
	std::vector<char> Final; // vector<bool> is evil
	std::vector<char> Dead;
	
	template <typename StateT>
	CompactDFA(DFA<StateT, char> D):
			num_states(D.num_states),
			q0(D.q0),
			d_data(num_states, std::vector<int>(D.sigma.size(), -1)),
			Final(num_states, false),
			Dead(num_states, false)
			{
		
		// Build compact sigma
		sigma.reserve(D.sigma.size());
		for (char a: D.sigma) {
			sigma.push_back(a);
		}
		
		// Build compact d_data
		for (int s=0; s<num_states; s++) {
			for (int a_i=0; a_i<sigma.size(); a_i++) {
				d_data.at(s).at(a_i) = D.d_data[s][sigma[a_i]];
			}
		}
		
		// Build Final
		for (int s: D.F) {
			Final[s] = true;
		}
		
		// Build Dead
		// (assuming that TFA will have optimized all possible dead states into one)
		for (int s=0; s<num_states; s++) {
			if (Final[s]) {
				continue;
			}
			
			for (int a_i=0; a_i<sigma.size(); a_i++) {
				if (d_data.at(s).at(a_i) != s) {
					goto continue_bigfor;
				}
			}
			// If we reach this point, s is dead
			Dead[s] = true;
			// break;
			
			continue_bigfor:;
		}
	}
	
	int delta(int s, char c);
	
	bool is_dead(int s);
	
	bool is_final(int s);
};


#endif
