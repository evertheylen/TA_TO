
#include "FSM.tcc"
#include "conv.h"
#include <vector>
#include <cmath>
#include <set>

	
template<typename T>
std::set<std::set<T>> powerSet(const std::set<T>& set) {
	std::vector<T> setv(set.begin(), set.end());
	std::set<std::set<T>> result;
	int numPowerSets = static_cast<int>(pow(2.0, static_cast<double>(set.size())));
// 	cout << "powerSet: numPowerSets=" << numPowerSets << endl;
	for (size_t i = 0; i < numPowerSets; ++i) {
		std::vector<int> onLocations = getOnLocations(i);
		std::set<T> subSet;
		for (size_t j = 0; j < onLocations.size(); ++j) {
// 			cout << "pushing " << onLocations.at(j) << "=" << set.at(onLocations.at(j)) << endl;
			subSet.insert(setv.at(onLocations.at(j)));
		}
// 		cout << endl;
		result.insert(subSet);
	}
	return result;
}

template<typename T>
T concatenate(std::set<T> s) {
	T str = "\"{";
	for (auto i: s) {
		str += i + ", ";
	}
	str += "}\"";
	return str;
}

template <typename eNFA_T, typename DFA_T>
DFA_T MSSC(eNFA_T& N) {
	DFA_T D;
	
	// First things first
	D.sigma = N.sigma;
	
	D.q0 = concatenate(typename eNFA_T::States({N.q0}));
	
	// Build powerset
	// TODO 'regenerate' N.states?
	auto powerset = powerSet<typename eNFA_T::State>(N.Q);
	
	// For every set in the powerset
	for (auto set: powerset) {
		auto set_name = concatenate(set);
		
		// For every symbol in the alphabet
		for (auto symbol: D.sigma) {
			std::set<typename DFA_T::State> to_set;
			
			// For every state in the set
			for (auto state: set) {
				// Skip undefined stuff in N.d
				if (N.d.find(state) == N.d.end() || N.d[state].find(symbol) == N.d[state].end()) {
					continue;
				}
				
				// Now add all the states this symbol would lead to, plus their ECLOSE!
				for (auto to_state: N.d[state][symbol]) {
					to_set.insert(to_state);
					for (auto e_to_state: N.ECLOSE(to_state)) {
						to_set.insert(e_to_state);
					}
				}
			}
			
			// Once we know this, we can define D.d[set][symbol]
			D.d[set_name][symbol] = concatenate(to_set);
		}
		
		// Also insert it as a proper name
		D.Q.insert(concatenate(set));
		
		// And, if the set contains a final state, add it to F
		bool containsFinal = false;
		for (auto state: set) {
			if (N.F.find(state) != N.F.end()) {
				containsFinal = true;
				break;
			}
		}
		if (containsFinal) {
			D.F.insert(set_name);
		}
	}
		
	return D;
}
