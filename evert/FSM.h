#include <string>
#include <iostream>
#include <set>
#include <vector>
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
	typedef DTT DT;
	typedef std::unordered_map<State, std::unordered_map<Symbol, DT>> Delta;
	
	// Definition of Finite Automaton
	//    {Q, Σ, δ, q0, F}
	//    == {Q, sigma, d, q0, F}
	
	States Q;       
		// States are implicit, but can, if required, be generated.
		
	Alphabet sigma;
		// sigma aka Σ
		
	Delta d;
		// d aka δ: (QxΣ) → DT (probably P(Q) or Q)
		
	State q0;
		// Starting state.
		
	States F;
		// F ⊆ Q
	
	
	Automaton(States _Q, Alphabet _sigma, Delta _d, State _q0, States _F);
	
	Automaton();
};


// simple typedefs are for losers.
typedef Automaton<std::string, std::string, std::string> str_DFA;

// let's do templated ones
template <typename StateT, typename SymbolT>
using NFA = Automaton<std::set<StateT>, StateT, SymbolT>;

typedef NFA<std::string, std::string> str_NFA;


// Now let's do templated inheritance

template <typename StateT, typename SymbolT>
class eNFA: public NFA<StateT, SymbolT> {
public:
	// some typedefs
	typedef StateT State;
	typedef std::set<State> States;
	typedef SymbolT Symbol;
	typedef std::set<Symbol> Alphabet;
	typedef std::set<StateT> DT;
	typedef std::unordered_map<State, std::unordered_map<Symbol, DT>> Delta;


	StateT epsilon;  // what to check for
	
	
	eNFA(States _Q, Alphabet _sigma, Delta _d, State _q0, States _F, StateT epsilon);
	
	std::set<StateT> ECLOSE(StateT s);
};

typedef eNFA<std::string, std::string> str_eNFA;


// A walker can walk though states. For more advanced usage, see Run.

template <typename FSM>
class Walker {
public:
	FSM automaton; // can be a reference (?)
	typename FSM::DT current; // current state
	
	Walker(FSM _automaton);
	
	bool input(typename FSM::Symbol symbol);
};

// NFA specialization
template <typename StateT, typename SymbolT>
class Walker<NFA<StateT, SymbolT>> {
public:
	NFA<StateT, SymbolT> automaton; // can be a reference (?)
	typename NFA<StateT, SymbolT>::DT current; // current state
	
	Walker(NFA<StateT, SymbolT> _automaton);
	
	bool input(SymbolT symbol);
};

// eNFA specialization
template <typename StateT, typename SymbolT>
class Walker<eNFA<StateT, SymbolT>> {
public:
	eNFA<StateT, SymbolT> automaton; // can be a reference (?)
	typename eNFA<StateT, SymbolT>::DT current; // current state
	
	Walker(eNFA<StateT, SymbolT> _automaton);
	
	bool input(SymbolT symbol);
};


// Run contains a Walker.
// Partially good design, partially me not wanting to specialize each and every method.

template <typename FSM>
class Run {
public:
	Walker<FSM> walker;
	
	Run(FSM _automaton);
	
	void process(std::vector<typename FSM::Symbol> str);
	
	bool isFinal();
};

