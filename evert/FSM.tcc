#ifndef _FSM
#define _FSM

#include <string>
#include <iostream>
#include <set>
#include <unordered_map>
#include <type_traits>

template <typename T>
std::ostream& operator<<(std::ostream& out, std::set<T> const& v) {
	out << "{ just another set }";
	return out;
}

// Make sure to specialize this whenever an irritating type shows up
// while parsing the xml file.
// It's just to make C++ happy, will never be used.
template <typename From, typename To>
To irritating_cast(From f) {
	return To(f);
}

//BEGIN -----[ AUTOMATON ]-----------------------------

#include <string>
#include <iostream>
#include <set>
#include <vector>
#include <unordered_map>
#include "tinyxml.h"


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
	
	//TODO void lazy_evaluation();
	
	// Input
	void from_xml(TiXmlDocument& doc);
	virtual void read_transition(TiXmlElement* el);
	
	// Output
	void to_dot(std::ostream& out);
	virtual void state_to_dot(std::ostream& out, State from, Symbol symbol, DT to);
};

// simple typedefs are for losers.
typedef Automaton<std::string, std::string, std::string> str_DFA;

template<typename DTT, typename StateT, typename SymbolT>
Automaton<DTT, StateT, SymbolT>::Automaton(States _Q,
							std::set<SymbolT> _sigma,
							std::unordered_map<StateT, std::unordered_map<SymbolT, DTT>> _d,
							State _q0,
							std::set<StateT> _F):
		Q(_Q),
		sigma(_sigma),
		d(_d),
		q0(_q0),
		F(_F)
		{}

template<typename DTT, typename StateT, typename SymbolT>
Automaton<DTT, StateT, SymbolT>::Automaton():
		Q(),
		sigma(),
		d(),
		q0(),
		F()
		{}



// General function to print to dot
template<typename DTT, typename StateT, typename SymbolT>
void Automaton<DTT, StateT, SymbolT>::to_dot(std::ostream& out) {
	out << "digraph automaton {\n"
		<< "    node [shape=none]; start;\n";
	
	for (auto f_state: this->F) {
		out << "    node [shape=doublecircle]; " << f_state << ";\n";
	}
	
	out << "    node [shape=circle];\n"
		<< "    start -> " << this->q0 << ";\n";
	
	for (auto it_from = this->d.begin(); it_from != this->d.end(); ++it_from) {
		// it_from->first == start state
		// it_from->second == map[symbol] DT
		for (auto it_symb = it_from->second.begin(); it_symb != it_from->second.end(); ++it_symb) {
			// it_symb->first == symbol
			// it_symb->second == FSM::DT
			this->state_to_dot(out, it_from->first, it_symb->first, it_symb->second);
		}
	}
	
	out << "}\n";
}


// Prints FSM::DT
template<typename DTT, typename StateT, typename SymbolT>
void Automaton<DTT, StateT, SymbolT>::state_to_dot(std::ostream& out, StateT from, SymbolT symbol, DTT to) {
	// Problem: http://ideone.com/YyiGUP
	// Solution: the fact that you override that virtual function doesn't mean the other one can be broken
	//           --> provide operator<< for std::set
	out << "    " << from << " -> " << to << " [label=\"" << symbol << "\"];\n";
}


// Import from XML
template<typename DTT, typename StateT, typename SymbolT>
void Automaton<DTT, StateT, SymbolT>::from_xml(TiXmlDocument& doc) {
	TiXmlElement* root = doc.RootElement();
	//name = readAttribute(root, "name");
	
	// Parse sigma
	auto alphabet_el = root->FirstChildElement("ALPHABET");
	for (TiXmlElement* symbol_el = alphabet_el->FirstChildElement("SYMBOL");
			symbol_el != nullptr;
			symbol_el = symbol_el->NextSiblingElement("SYMBOL")) {
		this->sigma.insert(symbol_el->GetText());
	}
	
	// Parse states
	auto states_el = root->FirstChildElement("STATES");
	
	// Parse start states (will ignore more than one begin state)
	auto start_states_el = states_el->FirstChildElement("START");
	this->q0 = State(start_states_el->FirstChildElement("STATE")->Attribute("name"));
	this->Q.insert(this->q0);
	
	// Parse 'normal' states
	for (TiXmlElement* state_el = states_el->FirstChildElement("STATE");
			state_el != nullptr;
			state_el = state_el->NextSiblingElement("STATE")) {
		this->Q.insert(State(state_el->Attribute("name")));
	}
	
	auto end_states_el = states_el->FirstChildElement("END");
	if (end_states_el != nullptr) {
		for (TiXmlElement* end_state_el = end_states_el->FirstChildElement("STATE");
				end_state_el != nullptr;
				end_state_el = end_state_el->NextSiblingElement("STATE")) {
			State end_state = State(end_state_el->Attribute("name"));
			this->Q.insert(end_state);
			this->F.insert(end_state);
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


template<typename DTT, typename StateT, typename SymbolT>
void Automaton<DTT, StateT, SymbolT>::read_transition(TiXmlElement* el) {
	State from(el->Attribute("from"));
	Symbol symbol(el->Attribute("symbol"));
	DT to = irritating_cast<const char*, DT>(el->Attribute("to"));
	this->d[from][symbol] = to;
}


//END


//BEGIN -----[ NFA: Automaton ]------------------------

// Now let's do templated inheritance

template <typename StateT, typename SymbolT>
class NFA: public Automaton<std::set<StateT>, StateT, SymbolT> {
public:
	// some typedefs
	typedef StateT State;
	typedef std::set<State> States;
	typedef SymbolT Symbol;
	typedef std::set<Symbol> Alphabet;
	typedef std::set<StateT> DT;
	typedef std::unordered_map<State, std::unordered_map<Symbol, DT>> Delta;
	
	NFA(States _Q, Alphabet _sigma, Delta _d, State _q0, States _F);
	
	NFA();
	
	virtual void state_to_dot(std::ostream& out, State from, Symbol symbol, DT to);
	
	void read_transition(TiXmlElement* el);
};

typedef NFA<std::string, std::string> str_NFA;

template<typename StateT, typename SymbolT>
NFA<StateT, SymbolT>::NFA(std::set<StateT> _Q,
							std::set<SymbolT> _sigma,
							std::unordered_map<StateT, std::unordered_map<SymbolT, DT>> _d,
							StateT _q0,
							std::set<StateT> _F):
		Automaton<std::set<StateT>, StateT, SymbolT>(_Q, _sigma, _d, _q0, _F)
		{}

template<typename StateT, typename SymbolT>
NFA<StateT, SymbolT>::NFA():
		Automaton<std::set<StateT>, StateT, SymbolT>()
		{}

// Prints FSM::DT, specialized for NFA
template <typename StateT, typename SymbolT>
void NFA<StateT, SymbolT>::state_to_dot(std::ostream& out, typename NFA<StateT, SymbolT>::State from,
			typename NFA<StateT, SymbolT>::Symbol symbol, typename NFA<StateT, SymbolT>::DT to) {
	
	for (auto substate_to: to) {
		out << "    " << from << " -> " << substate_to << " [label=\"" << symbol << "\"];\n";
	}
}

template <typename StateT, typename SymbolT>
void NFA<StateT, SymbolT>::read_transition(TiXmlElement* el) {
	State from(el->Attribute("from"));
	Symbol symbol(el->Attribute("symbol"));
	State to_state = State(el->Attribute("to"));
	this->d[from][symbol].insert(to_state);
}

//END


//BEGIN -----[ eNFA: NFA ]-----------------------------

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
	
	eNFA(States _Q, Alphabet _sigma, Delta _d, State _q0, States _F, StateT _epsilon);
	
	eNFA(StateT epsilon);
	
	std::set<StateT> ECLOSE(StateT s);
	
	void state_to_dot(std::ostream& out, State from, Symbol symbol, DT to);
};

typedef eNFA<std::string, std::string> str_eNFA;

template<typename StateT, typename SymbolT>
eNFA<StateT, SymbolT>::eNFA(std::set<StateT> _Q,
							std::set<SymbolT> _sigma,
							std::unordered_map<StateT, std::unordered_map<SymbolT, DT>> _d,
							StateT _q0,
							std::set<StateT> _F,
							StateT _epsilon):
		NFA<StateT, SymbolT>(_Q, _sigma, _d, _q0, _F),
		epsilon(_epsilon)
		{}

template<typename StateT, typename SymbolT>
eNFA<StateT, SymbolT>::eNFA(StateT _epsilon):
		NFA<StateT, SymbolT>(),
		epsilon(_epsilon)
		{}


// epsilon-closure
template<typename StateT, typename SymbolT>
std::set<StateT> eNFA<StateT, SymbolT>::ECLOSE(StateT s) {
	std::set<StateT> eclosure;
	
	// if the state s is defined in the delta function (d)
	if (this->d.find(s) != this->d.end()) {
		if (this->d[s].find(epsilon) != this->d[s].end()) {
			// d(s, epsilon) is defined! yay!
			for (auto estate: this->d[s][epsilon]) {
				// all these states are definitly part of the eclosure
				// so, insert them
				eclosure.insert(estate);
				
				// now, we need to do this recursively
				for (auto sub_estate: this->ECLOSE(estate)) {
					eclosure.insert(estate);
				}
			}
		}
	}
	
	return eclosure;
}

// Prints FSM::DT, specialized for eNFA
template <typename StateT, typename SymbolT>
void eNFA<StateT, SymbolT>::state_to_dot(std::ostream& out, typename eNFA<StateT, SymbolT>::State from,
			typename eNFA<StateT, SymbolT>::Symbol symbol, typename eNFA<StateT, SymbolT>::DT to) {
	for (auto substate_to: to) {
		out << "    " << from << " -> " << substate_to << " [label=\"";
		std::cerr < "I'm aliiiiiiiiive\n";
		if (symbol == this->epsilon) {
			out << "&epsilon;\"];\n";
		} else {
			out << symbol << "\"];\n";
		}
	}
}

//END


//BEGIN -----[ Walker ]--------------------------------

template <typename FSM>
class Walker {
public:
	FSM automaton; // can be a reference (?)
	typename FSM::DT current; // current state
	
	Walker(FSM _automaton);
	
	bool input(typename FSM::Symbol symbol);
};


template <typename FSM>
Walker<FSM>::Walker(FSM _automaton):
	automaton(_automaton), current(_automaton.q0)
	{}

template <typename FSM>
bool Walker<FSM>::input(typename FSM::Symbol symbol) {
	if (this->automaton.sigma.find(symbol) == this->automaton.sigma.end()) {
		return false;
	}
	
	this->current = this->automaton.d[this->current][symbol];
	
	return true;
}


//BEGIN --- Walker: NFA specialization ---


// NFA specialization
template <typename StateT, typename SymbolT>
class Walker<NFA<StateT, SymbolT>> {
public:
	NFA<StateT, SymbolT> automaton; // can be a reference (?)
	typename NFA<StateT, SymbolT>::DT current; // current state
	
	Walker(NFA<StateT, SymbolT> _automaton);
	
	bool input(SymbolT symbol);
};

template <typename StateT, typename SymbolT>
Walker<NFA<StateT, SymbolT>>::Walker(NFA<StateT, SymbolT> _automaton):
	automaton(_automaton), current({_automaton.q0})
	{}

template <typename StateT, typename SymbolT>
bool Walker<NFA<StateT, SymbolT>>::input(SymbolT symbol) {
	typename std::set<StateT> newcurrent;
	
	if (this->automaton.sigma.find(symbol) == this->automaton.sigma.end()) {
		return false;
	}
	
	// for all states in the current multistate
	for (auto s: this->current) {
		// if d contains it
		if (this->automaton.d.find(s) != this->automaton.d.end()) {
			// find symbol
			if (this->automaton.d[s].find(symbol) != this->automaton.d[s].end()) {
				// for all states in d[s][symbol] (which is of type std::set, hence the specialization)
				for (auto new_s: automaton.d[s][symbol]) {
					newcurrent.insert(new_s);
				}
			} else {
				// symbol not found, add s, original state remains
				newcurrent.insert(s);
			}
		} else {
			// state not found, add s, original state remains
			newcurrent.insert(s);
		}
	}
	
	this->current = newcurrent;
	
	return true;
}

//END


//BEGIN --- Walker: eNFA specialization ---

// eNFA specialization
template <typename StateT, typename SymbolT>
class Walker<eNFA<StateT, SymbolT>> {
public:
	eNFA<StateT, SymbolT> automaton; // can be a reference (?)
	typename eNFA<StateT, SymbolT>::DT current; // current state
	
	Walker(eNFA<StateT, SymbolT> _automaton);
	
	bool input(SymbolT symbol);
};

template <typename StateT, typename SymbolT>
Walker<eNFA<StateT, SymbolT>>::Walker(eNFA<StateT, SymbolT> _automaton):
		automaton(_automaton), current({_automaton.q0}) {
	// Also insert ECLOSE(q0)
	for (auto estate: _automaton.ECLOSE(_automaton.q0)) {
		current.insert(estate);
	}
}

template <typename StateT, typename SymbolT>
bool Walker<eNFA<StateT, SymbolT>>::input(SymbolT symbol) {
	// Assumption: the current states in current are already ECLOSE'd
	
	typename std::set<StateT> newcurrent;
	
	if (this->automaton.sigma.find(symbol) == this->automaton.sigma.end()
		&& symbol != this->automaton.epsilon) {
		return false;
	}
	
	// for all states in the current multistate
	for (auto s: this->current) {
		// if d contains it
		if (this->automaton.d.find(s) != this->automaton.d.end()) {
			// find symbol
			if (this->automaton.d[s].find(symbol) != this->automaton.d[s].end()) {
				// for all states in d[s][symbol] (which is of type std::set, hence the specialization)
				for (auto new_s: automaton.d[s][symbol]) {
					newcurrent.insert(new_s);
					// Also insert the ECLOSE version
					for (auto eclosed_state: this->automaton.ECLOSE(new_s)) {
						newcurrent.insert(new_s);
					}
				}
			} else {
				// symbol not found, add s, original state remains
				newcurrent.insert(s);
			}
		} else {
			// state not found, add s, original state remains
			newcurrent.insert(s);
		}
	}
	
	this->current = newcurrent;
	
	return true;
}

//END

//END


//BEGIN -----[ Run ]-----------------------------------

template <typename FSM>
class Run {
public:
	Walker<FSM> walker;
	
	Run(FSM _automaton);
	
	void process(std::vector<typename FSM::Symbol> str);
	
	bool isFinal();
};

template <typename FSM>
Run<FSM>::Run(FSM _automaton):
	walker(_automaton)
	{}

template <typename FSM>
void Run<FSM>::process(std::vector<typename FSM::Symbol> str) {
	for (auto symbol: str) {
		this->walker.input(symbol);
	}
}

template <typename FSM>
bool Run<FSM>::isFinal() {
	for (auto s: this->walker.current) {
		if (this->walker.automaton.F.find(s) != this->walker.automaton.F.end()) {
			return true;
		}
	}
	return false;
}

//END

#endif

//EOF