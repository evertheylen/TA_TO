#include <iostream>
#include <map>
#include "tinyxml/tinyxml.h"
#include <cmath>

#include "FSM.h"

using namespace std;

// Returns which bits are on in the integer a                                                                                                                                                                                              
vector<int> getOnLocations(int a) {
  vector<int> result;
  int place = 0;
  while (a != 0) {
    if (a & 1) {
      result.push_back(place);
    }
    ++place;
    a >>= 1;
  }
  return result;
}

	
template<typename T>
vector<vector<T> > powerSet(const vector<T>& set) {
	vector<vector<T> > result;
	int numPowerSets = static_cast<int>(pow(2.0, static_cast<double>(set.size())));
	cout << "powerSet: numPowerSets=" << numPowerSets << endl;
	for (size_t i = 0; i < numPowerSets; ++i) {
		vector<int> onLocations = getOnLocations(i);
		vector<T> subSet;
		for (size_t j = 0; j < onLocations.size(); ++j) {
// 			cout << "pushing " << onLocations.at(j) << "=" << set.at(onLocations.at(j)) << endl;
			subSet.push_back(set.at(onLocations.at(j)));
		}
		cout << endl;
		result.push_back(subSet);
	}
	return result;
}


Automaton eps_NFA_to_DFA(Automaton& NFA) {
	auto DFA = Automaton();
	
	// First things first.
	DFA.alphabet = NFA.alphabet;
	
	// Build powerset. DFA.states = 2^(NFA.states)
	auto allstates = vector<State*>();
	for (auto it: NFA.states) {
// 		cout << "it->second == " << it.second->name << endl;
		allstates.push_back(it.second);
	}
	
	auto powerset = powerSet<State*>(allstates);
	
	// also remember what state in the DFA corresponds to what set of states in the NFA
	auto powermap = map<State*, vector<State*>>();
	int i = 0;
	for (auto set: powerset) {
		string totalname = "";
		for (auto state: set) {
			totalname += state->name + ", ";
		}
		State* new_state = new State(totalname);
		DFA.states[totalname] = new_state;
		powermap[new_state] = set;
		i++;
	}
	
	cout << "|2^Q| = " << i << endl;
	
	// for state in 2^Q
	for (auto it = DFA.states.begin(); it != DFA.states.end(); ++it) {
		// state is it->second
		// name is it->first, or it->second->name
		
		// for substate in state
		for (auto substate: powermap[it->second]) {
			
			// for symbol in delta_N(substate)
			for (auto symbol_it = NFA.transitions[substate->name].begin(); symbol_it != NFA.transitions[substate->name].end(); ++symbol_it) {
				// symbol is symbol_it->first, name of to_state is symbol_it->second
				DFA.transitions[it->first][symbol_it->first] += symbol_it->second;
				
				// TODO: EPSILON
			}
		}
	}
	
	DFA.start = DFA.states[NFA.start->name];
	
	return DFA;
}


int main(int argc, char** argv) {
	
	int a = 707; // = 101011001
	auto loc = getOnLocations(a);
	for (auto l: loc) {
		cout << l << ", ";
	}
	cout << endl;
	
	TiXmlDocument doc;

	if(argc <= 1) {
		std::cerr << "Not enough arguments given.\n";
		return 1;
	}
	
	cout << "Starting...\n";
	
	bool doc_loaded = doc.LoadFile(argv[1]);
	
	if (!doc_loaded) {
		std::cerr << "Error loading board.\n";
	}
	
	Automaton N(doc);
	
// 	cout << "About to print stuff ...\n";
// 	
// 	cout << N.transitions[N.start->name].size() << "\n";
// 	
// 	cout << N.transitions[N.start->name][N.alphabet[0]][0] << "\n";
	
	auto DFA = eps_NFA_to_DFA(N);
	
	cout << "test: " << DFA.transitions["q0, "]["+"] << endl;
	
	return 0;
}
