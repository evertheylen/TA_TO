/*
#include <string>
#include <iostream>
#include <set>
#include <unordered_map>
#include <type_traits>

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

//END*/