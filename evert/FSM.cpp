#include <string>
#include <iostream>
#include <set>
#include <unordered_map>
#include "FSM.h"

// -----[ AUTOMATON ]------------------

template<typename DTT, typename StateT, typename SymbolT>
Automaton<DTT, StateT, SymbolT>::Automaton(std::set<StateT> _Q,
							std::set<SymbolT> _sigma,
							std::unordered_map<StateT, std::unordered_map<SymbolT, DTT>> _d,
							DTT _q0,
							std::set<StateT> _F,
							StateT _epsilon):
		Q(_Q),
		sigma(_sigma),
		d(_d),
		q0(_q0),
		F(_F),
		epsilon(_epsilon)
		{}

template<typename DTT, typename StateT, typename SymbolT>
Automaton<DTT, StateT, SymbolT>::Automaton():
		Q(),
		sigma(),
		d(),
		q0(),
		F(),
		epsilon("eps")
		{}

// -----[ RUN ]------------------------

template <typename FSM, bool checkEpsilon>
Run<FSM, checkEpsilon>::Run(FSM _automaton):
	automaton(_automaton), current(_automaton.q0)
	{}

template <typename FSM, bool checkEpsilon>
bool Run<FSM, checkEpsilon>::input(typename FSM::Symbol symbol) {
	if (this->automaton.sigma.find(symbol) == this->automaton.sigma.end()) {
		return false;
	}
	
	this->current = this->automaton.d[current][symbol];
	
	return true;
}


template <typename FSM, bool checkEpsilon>
bool Run<FSM, checkEpsilon>::isFinal() {
	if (this->automaton.F.find(this->current) != this->automaton.F.end()) {
		return true;
	} else {
		return false;
	}
}


// --- NFA specialization ---

template <typename StateT, typename SymbolT, bool checkEpsilon>
Run<NFA<StateT, SymbolT>, checkEpsilon>::Run(NFA<StateT, SymbolT> _automaton):
	automaton(_automaton), current(_automaton.q0)
	{}

template <typename StateT, typename SymbolT, bool checkEpsilon>
bool Run<NFA<StateT, SymbolT>, checkEpsilon>::input(SymbolT symbol) {
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


template <typename StateT, typename SymbolT, bool checkEpsilon>
bool Run<NFA<StateT, SymbolT>, checkEpsilon>::isFinal() {
	if (this->automaton.F.find(this->current) != this->automaton.F.end()) {
		return true;
	} else {
		return false;
	}
}
