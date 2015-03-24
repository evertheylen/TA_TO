#include <string>
#include <iostream>
#include <set>
#include <unordered_map>
#include "FSM.h"

//BEGIN -----[ AUTOMATON ]------------------

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

//END


//BEGIN -----[ eNFA: Automaton ]-----------------------

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

//END


//BEGIN -----[ Walker ]------------------------

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


//BEGIN -----[ Run ]---------------------------

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


//EOF