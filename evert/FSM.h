#include <string>
#include <iostream>
#include <set>
#include <unordered_map>

template<typename DTT, // Delta type, like so: delta: (Q x A) --> DT (==ST for DFA, std::set<DT> for NFA)
	typename StateT, // State Type
	typename SymbolT> // Alphabet Type
class Automaton {
public:
	// some typedefs
	typedef StateT State;
	typedef std::set<State> States;
	typedef SymbolT Symbol;
	typedef std::set<Symbol> Alphabet;
	typedef std::unordered_map<State, std::unordered_map<Symbol, DTT>> Delta;
	typedef DTT DT;
	
	// Definition of Finite Automaton
	//    {Q, Σ, δ, q0, F}
	//    == {Q, sigma, d, q0, F}
	
	States Q;       
		// States are implicit, but can, if required, be generated.
		
	Alphabet sigma;
		// sigma aka Σ
		
	Delta d;
		// d aka δ: (QxΣ) → DT (probably P(Q) or Q)
		
	DT q0;
		// Starting state.
		
	States F;
		// F ⊆ Q
	
	
	StateT epsilon;
		// workaround, can't use std::string values as template parameters (damn you C++!)
	
	Automaton(States _Q, Alphabet _sigma, Delta _d, DT _q0, States _F, StateT _epsilon=StateT("eps"));
		// good luck using this one
	
	Automaton();
};


// simple typedefs are for losers.
typedef Automaton<std::string, std::string, std::string> str_DFA;

// let's do templated ones
template <typename StateT, typename SymbolT>
using NFA = Automaton<std::set<StateT>, StateT, SymbolT>;

typedef NFA<std::string, std::string> str_NFA;




template <typename FSM, bool checkEpsilon=false>
class Run {
public:
	FSM automaton; // can be a reference (?)
	typename FSM::DT current; // current state
	
	Run(FSM _automaton);
	
	bool input(typename FSM::Symbol symbol);
	
	bool isFinal();
};


template <typename StateT, typename SymbolT, bool checkEpsilon>
class Run<NFA<StateT, SymbolT>, checkEpsilon> {
public:
	NFA<StateT, SymbolT> automaton; // can be a reference (?)
	typename NFA<StateT, SymbolT>::DT current; // current state
	
	Run(NFA<StateT, SymbolT> _automaton);
	
	bool input(SymbolT symbol);
	
	bool isFinal();
};
