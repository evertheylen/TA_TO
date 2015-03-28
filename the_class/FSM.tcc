
#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <boost/bimap.hpp>


//BEGIN ----[ FSM ]-------------------------

// Main class, but also to be used as a DFA.

template <	typename StateT,              // State type
			typename SymbolT,             // Symbol type
			typename DeltaResultT = int>  // Delta type (difference between NFA and DFA)
			                              // you should be using int or std::vector<int> for this.
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
		Alphabet sigma;       // should NOT used while running (errors otherwise)
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
		int i=0;
		for (auto state: states) {
			map.left.insert(typename Bimap::left_value_type(i, state));
			++i;
		}
		num_states = i;
	}
	
	FSM() = default;
	
	int ID(State state) {
		return map.right.at(state);
	}
	
	State realState(int _ID) {
		return map.left.at(_ID);
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
	void add_state(State state, bool end) {
		num_states++;
		map.left.insert(Bimap::left_value_type(num_states, state));
		if (end) {
			F.insert(num_states);
		}
	}
	
	// TODO Lazy evaluation
	
	// TODO input from xml
	// TODO output to dot
};

//END


//BEGIN ----[ NFA ]-------------------------

template<	typename StateT,
			typename SymbolT>
class NFA: public FSM<std::vector<int>, StateT, SymbolT> {
public:
	// some typedefs
	typedef StateT State;
	typedef std::vector<State> States;  // only used in constructor
	typedef std::set<int> StateIDs;
	typedef SymbolT Symbol;
	typedef std::set<Symbol> Alphabet;
	typedef std::vector<int> DeltaResult;
	typedef std::map<int, std::map<Symbol, DeltaResult>> Delta;
	typedef boost::bimap<int, State> Bimap;
	
	NFA(States states, Alphabet _sigma, int _q0, StateIDs _F):
		FSM<std::vector<int>, StateT, SymbolT>(states, _sigma, _q0, _F) {}
	
	NFA() = default;
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
	typedef std::vector<int> DeltaResult;
	typedef std::map<int, std::map<Symbol, DeltaResult>> Delta;
	typedef boost::bimap<int, State> Bimap;
	
	eNFA(States states, Alphabet _sigma, int _q0, StateIDs _F):
		NFA<StateT, SymbolT>(states, _sigma, _q0, _F) {}
	
	eNFA() = default;
};

//END
