
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <boost/bimap.hpp>
#include "tinyxml.h"


template <typename To>
To XML_Cast(std::string s) {
	return To(s);  // standard cast by default
}

//BEGIN ----[ FSM ]-------------------------

// Main class (abstract)

template <	typename StateT,              // State type
			typename SymbolT,             // Symbol type
			typename DeltaResultT = int>  // Delta type (difference between NFA and DFA)
			                              // you should be using int or std::set<int> for this.
class FSM {
public:
	// some typedefs
	typedef StateT State;
	typedef std::vector<State> States;  // only used in constructor
	typedef std::set<int> StateIDs;
	typedef SymbolT Symbol;
	typedef std::set<Symbol> Alphabet;
	typedef DeltaResultT DeltaResult;
	typedef std::map<int, std::map<Symbol, DeltaResult>> Delta;
	typedef boost::bimap<int, State> Bimap;
	
	
	// Theory
		//States Q;           // NOT used while running
		int num_states;       // NOT used while running
		Alphabet sigma;       // should NOT be used while running (errors otherwise)
		Delta d_data;         // used a lot while running
		int q0;               // used while running
		StateIDs F;           // used while running
	
	// Practical
		Bimap map;  // Couples the ints to the type you really want
		            // contains the States (instead of Q)
		            // although it's a map, map.left should be used more as a vector
		            // NOT used while running
	
	FSM(States states, Alphabet _sigma, int _q0, StateIDs _F):
			sigma(_sigma), q0(_q0), F(_F), d_data(Delta()) {
		// insert all states, in order of definition
		int i=0;
		for (auto state: states) {
			map.left.insert(typename Bimap::left_value_type(i, state));
			++i;
		}
		num_states = i;
	}
	
	FSM(): num_states(0) {}
	
	int ID(State state) {
		return map.right.at(state);
	}
	
	State realState(int _ID) {
		std::cout << "realState(" << _ID << ") max:" << num_states << "\n";
		return map.left.at(_ID);
	}
	
	bool isFinal(int _ID) {
		return (F.find(_ID) != F.end());
	}
	
	bool isInSigma(Symbol symb) {
		return (sigma.find(symb) != sigma.end());
	}
	
	// function to use delta
	DeltaResultT delta(int from, Symbol symbol) {
		return d_data[from][symbol];
	}
	
	// set delta
	void set_delta(int from, Symbol symbol, DeltaResult to) {
		d_data[from][symbol] = to;
	}
	
	// add state
	int add_state(State state, bool end) {
		map.left.insert(typename Bimap::left_value_type(num_states, state));
		if (end) {
			F.insert(num_states);
		}
		num_states++;
		return num_states-1;
	}

	// puts the (old) IDs of all the used states in used_states
	virtual void _calc_used_states(int s_ID, StateIDs& used_states) = 0;
	
	
	// add old_ID (and its transitions) to d_data and map.
	virtual void clean_add(int old_ID, State s, Delta& new_d_data, Bimap& new_map, std::map<int, int>& new_stateID) = 0;
	
	
	bool lazy_evaluation() {
		StateIDs used_states;
		
		// calculate all necessary states
		this->_calc_used_states(q0, used_states);
		
		if (used_states.size() == this->num_states) {
			return false;  // no unused states
		}
		
		StateIDs new_F;
		Delta new_d_data;
		Bimap new_map;
		int new_q0;
		
		std::map<int, int> new_stateID;
		int i = 0;
		for (auto old_ID: used_states) {
			new_stateID[old_ID] = i;
			if (old_ID == this->q0) {
				new_q0 = old_ID;
			}
			i++;
		}
		// new_stateID[old_ID] = new_ID;
		
		for (auto it: map) {
			// it.left = ID
			// it.right = state
			
			// if the state is used
			if (used_states.find(it.left) != used_states.end()) {
				// if the used state is a final state, add it to the new final states
				if (F.find(it.left) != F.end()) {
					new_F.insert(new_stateID[it.left]);
				}
				
				// add it to d_data and map (using new_stateID)
				clean_add(it.left, it.right, new_d_data, new_map, new_stateID);
			}
		}
		
		this->F = new_F;
		this->d_data = new_d_data;
		this->map = new_map;
		this->q0 = new_q0;
		this->num_states = used_states.size();
		
		return true;  // we did make it smaller!
	}
	
	// Input
	void from_xml(TiXmlDocument& doc) {
		TiXmlElement* root = doc.RootElement();
		//name = readAttribute(root, "name");

		// Parse sigma
		auto alphabet_el = root->FirstChildElement("ALPHABET");
		for (TiXmlElement* symbol_el = alphabet_el->FirstChildElement("SYMBOL");
				symbol_el != nullptr;
				symbol_el = symbol_el->NextSiblingElement("SYMBOL")) {
			sigma.insert(XML_Cast<Symbol>(symbol_el->GetText()));
		}
		
		// Parse states (last start state defined will be the one we use)
		auto states_el = root->FirstChildElement("STATES");		
		for (TiXmlElement* state_el = states_el->FirstChildElement("STATE");
				state_el != nullptr;
				state_el = state_el->NextSiblingElement("STATE")) {
			
			bool end = state_el->Attribute("end") != nullptr && std::string(state_el->Attribute("end")) == "true";
			bool start = state_el->Attribute("start") != nullptr && std::string(state_el->Attribute("start")) == "true";
			int ID = add_state(XML_Cast<State>(state_el->Attribute("name")), end);
			if (start) {
				q0 = ID;
			}
		}
		
		// Parse transitions
		auto transitions_el = root->FirstChildElement("TRANSITIONS");
		for (TiXmlElement* transition_el = transitions_el->FirstChildElement("TRANSITION");
				transition_el != nullptr;
				transition_el = transition_el->NextSiblingElement("TRANSITION")) {
			this->read_transition(transition_el);
		}
	}
	
	virtual void read_transition(TiXmlElement* el) = 0;
	
	
	// Output
	void to_dot(std::ostream& out) {
		out << "digraph automaton {\n"
			<< "    node [shape=none]; start;\n";
		
		for (auto f_state: this->F) {
			out << "    node [shape=doublecircle]; \"" << realState(f_state) << "\";\n";
		}
		
		out << "    node [shape=circle];\n"
			<< "    start -> \"" << realState(this->q0) << "\";\n";
		
		for (auto it_from: this->d_data) {
			// it_from.first == start state
			// it_from.second == map[symbol] DeltaResult
			for (auto it_symb: it_from.second) {
				// it_symb.first == symbol
				// it_symb.second == to (type DeltaResult)
				this->state_to_dot(out, it_from.first, it_symb.first, it_symb.second);
			}
		}
		
		out << "}\n";
	}
	
	virtual void state_to_dot(std::ostream& out, int from, Symbol symbol, DeltaResult to) = 0;
};

//END

//BEGIN ----[ DFA ]-------------------------
template<	typename StateT,
			typename SymbolT>
class DFA: public FSM<StateT, SymbolT, int> {
public:

	// some typedefs
	typedef StateT State;
	typedef std::vector<State> States;  // only used in constructor
	typedef std::set<int> StateIDs;
	typedef SymbolT Symbol;
	typedef std::set<Symbol> Alphabet;
	typedef int DeltaResult;
	typedef std::map<int, std::map<Symbol, DeltaResult>> Delta;
	typedef boost::bimap<int, State> Bimap;
	
	
	DFA(States states, Alphabet _sigma, int _q0, StateIDs _F):
		FSM<StateT, SymbolT, int>(states, _sigma, _q0, _F) {}
	
	DFA(): FSM<StateT, SymbolT, int>() {}
	
	// puts the (old) IDs of all the used states in used_states
	// DFA version
	void _calc_used_states(int s_ID, StateIDs& used_states) {
		// if s_ID in used_states { return; }
		if (used_states.find(s_ID) != used_states.end()) {
			return;
		}
		
		// add to used_states
		used_states.insert(s_ID);
		
		// if s_ID is defined in d_data
		if (this->d_data.find(s_ID) != this->d_data.end()) {
			std::map<Symbol, DeltaResult>& d_data_second = this->d_data[s_ID];
			// for all symbols in d_data[s_ID]
			for (auto symb_it: d_data_second) {
				// add the state
				this->_calc_used_states(symb_it.second, used_states);

			}
		}
	}
	
	
	// add old_ID (and its transitions) to d_data and map.
	// this is the DFA version
	void clean_add(int old_ID, State s, Delta& new_d_data, Bimap& new_map, std::map<int, int>& new_stateID) {
		int new_ID = new_stateID[old_ID];
		
		// first, d_data.
		// for all symbols...
		for (auto symb_it: this->d_data[old_ID]) {
			// symb_it.first = symbol,  symb_it.second = DeltaResult [=int]
			// all transitions from s_ID should be used, otherwise this function
			// shouldn't be called!
			new_d_data[ new_ID ][ symb_it.first ] = new_stateID[symb_it.second];
		}
		
		// then, map.
		new_map.left.insert(typename Bimap::left_value_type(new_ID, s));
	}
	
	
	// Print state to dot format
	void state_to_dot(std::ostream& out, int from, Symbol symbol, DeltaResult to) {
		out << "    \"" << this->realState(from) << "\" -> \"" << this->realState(to) << "\" [label=\"" << symbol << "\"];\n";
	}
	
	// Read a transition
	void read_transition(TiXmlElement* el) {
		int from = this->ID(XML_Cast<State>(el->Attribute("from")));
		Symbol symbol = XML_Cast<Symbol>(el->Attribute("symbol"));
		int to = this->ID(XML_Cast<State>(el->Attribute("to")));
		this->d_data[from][symbol] = to;
	}
};

//END


//BEGIN ----[ NFA ]-------------------------

template<	typename StateT,
			typename SymbolT>
class NFA: public FSM<StateT, SymbolT, std::set<int>> {
public:
	// some typedefs
	typedef StateT State;
	typedef std::vector<State> States;  // only used in constructor
	typedef std::set<int> StateIDs;
	typedef SymbolT Symbol;
	typedef std::set<Symbol> Alphabet;
	typedef std::set<int> DeltaResult;
	typedef std::map<int, std::map<Symbol, DeltaResult>> Delta;
	typedef boost::bimap<int, State> Bimap;
	
	NFA(States states, Alphabet _sigma, int _q0, StateIDs _F):
		FSM<StateT, SymbolT, std::set<int>>(states, _sigma, _q0, _F) {}
	
	NFA(): FSM<StateT, SymbolT, std::set<int>>() {}

	// puts the (old) IDs of all the used states in used_states
	// NFA version
	virtual void _calc_used_states(int s_ID, StateIDs& used_states) {
		// if s_ID in used_states { return; }
		if (used_states.find(s_ID) != used_states.end()) {
			return;
		}
		
		// add to used_states
		used_states.insert(s_ID);
		
		// if s_ID is defined in d_data
		if (this->d_data.find(s_ID) != this->d_data.end()) {
			std::map<Symbol, DeltaResult>& d_data_second = this->d_data[s_ID];
			// for all symbols in d_data[s_ID]
			for (auto symb_it: d_data_second) {
				// for all states in d_data[s_ID][symb_it.first]
				for (auto state_ID: symb_it.second) {
					// add the state
					this->_calc_used_states(state_ID, used_states);
				}
			}
		}
	}
	
	// add old_ID (and its transitions) to d_data and map.
	// this is the NFA version.
	void clean_add(int old_ID, State s, Delta& new_d_data, Bimap& new_map, std::map<int, int>& new_stateID) {
		int new_ID = new_stateID[old_ID];
		
		// first, d_data.
		// for all symbols...
		for (auto symb_it: this->d_data[old_ID]) {
			// symb_it.first = symbol,  symb_it.second = DeltaResult [=vector<int>]
			// all transitions from s_ID should be used, otherwise this function
			// shouldn't be called!
			for (auto state: symb_it.second) {
				new_d_data[ new_ID ][ symb_it.first ].insert(new_stateID[state]);
			}
		}
		
		// then, map.
		new_map.left.insert(typename Bimap::left_value_type(new_ID, s));
	}
	
	// Print state to dot format
	virtual void state_to_dot(std::ostream& out, int from, Symbol symbol, DeltaResult to) {
		for (auto substate_to: to) {
			out << "    \"" << this->realState(from) << "\" -> \"" << this->realState(substate_to) << "\" [label=\"" << symbol << "\"];\n";
		}
	}
	
	// Read a transition
	virtual void read_transition(TiXmlElement* el) {
		int from = this->ID(XML_Cast<State>(el->Attribute("from")));
		Symbol symbol = XML_Cast<Symbol>(el->Attribute("symbol"));
		int to = this->ID(XML_Cast<State>(el->Attribute("to")));
		this->d_data[from][symbol].insert(to);
	}
};

//END


//BEGIN ----[ eNFA ]------------------------

template<	typename StateT,
			typename SymbolT,
			SymbolT epsilon>
class eNFA: public NFA<StateT, SymbolT> {
public:
		// some typedefs
	typedef StateT State;
	typedef std::vector<State> States;  // only used in constructor
	typedef std::set<int> StateIDs;
	typedef SymbolT Symbol;
	typedef std::set<Symbol> Alphabet;
	typedef std::set<int> DeltaResult;
	typedef std::map<int, std::map<Symbol, DeltaResult>> Delta;
	typedef boost::bimap<int, State> Bimap;
	
	eNFA(States states, Alphabet _sigma, int _q0, StateIDs _F):
		NFA<StateT, SymbolT>(states, _sigma, _q0, _F) {}
	
	eNFA(): NFA<StateT, SymbolT>() {}
	
		
	std::set<int> ECLOSE(int s_ID) {
		std::set<int> eclosure;
		
		// This function handles recursion much better
		_ECLOSE(s_ID, eclosure);
		
		return eclosure;
	}
	
	// puts the (old) IDs of all the used states in used_states
	// eNFA version
	void _calc_used_states(int s_ID, StateIDs& used_states) {
		// if s_ID in used_states { return; }
		if (used_states.find(s_ID) != used_states.end()) {
			return;
		}
		
		// add to used_states
		used_states.insert(s_ID);
		
		// if s_ID is defined in d_data
		if (this->d_data.find(s_ID) != this->d_data.end()) {
			std::map<Symbol, DeltaResult>& d_data_second = this->d_data[s_ID];
			// for all symbols in d_data[s_ID]
			for (auto symb_it: d_data_second) {
				// for all states in d_data[s_ID][symb_it.first]
				for (auto state_ID: symb_it.second) {
					// add the state
					for (auto e_state_ID: ECLOSE(state_ID)) {
						this->_calc_used_states(e_state_ID, used_states);
					}
				}
			}
		}
	}
	
	void state_to_dot(std::ostream& out, int from, Symbol symbol, DeltaResult to) {
		for (auto substate_to: to) {
			out << "    \"" << this->realState(from) << "\" -> \"" << this->realState(substate_to) << "\" [label=\"";
			if (symbol == epsilon) {
				out << "&epsilon;\"];\n";
			} else {
				out << symbol << "\"];\n";
			}
		}
	}
	
	// Read a transition
	void read_transition(TiXmlElement* el) {
		int from = this->ID(XML_Cast<State>(el->Attribute("from")));
		std::string symbol_str = el->Attribute("symbol");
		Symbol symbol;
		if (symbol_str == "eps") {
			symbol = epsilon;
		} else {
			symbol = XML_Cast<Symbol>(symbol_str); // may still be == epsilon
		}
		int to = this->ID(XML_Cast<State>(el->Attribute("to")));
		this->d_data[from][symbol].insert(to);
	}
	
	void _ECLOSE(int s_ID, DeltaResult& eclosure) {
		if (eclosure.find(s_ID) != eclosure.end()) {
			return;
		}
		eclosure.insert(s_ID);
		
		// if the state s is defined in the delta function
		if (this->d_data.find(s_ID) != this->d_data.end()) {
			std::map<Symbol, DeltaResult>& d_second = this->d_data[s_ID];
			
			if (d_second.find(epsilon) != d_second.end()) {
				// there is an epsilon transition! yay
				
				for (auto estate_ID: d_second[epsilon]) {
					_ECLOSE(estate_ID, eclosure);
				}
			}
		}
	}
	
};

//END
