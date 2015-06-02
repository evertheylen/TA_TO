#include "FSM.h"
#include <string>
#include <cmath>
#include <assert.h>
#include <vector>
#include <iostream>
#include <set>
#include <map>

#include "Run.h"



template<typename T>
void print(std::set<T> s) {
	std::cout << "{";
	for (auto e: s) {
		std::cout << e << ", ";
	}
	std::cout << "}\n";
}


template<	typename StateT,
			typename SymbolT,
			SymbolT epsilon>
DFA<StateT, SymbolT> MSSC(eNFA<StateT, SymbolT, epsilon>& N) {
	DFA<StateT, SymbolT> D;
	
	D.sigma = N.sigma;
	D.q0 = 0;  // by definition the first state
	
	std::map<std::set<int>, int> states;
	std::set<int> start = N.ECLOSE(N.q0);
	states[start] = 0;
	recursive_add(D, N, states, start);
	
	D.num_states = states.size();
// 	std::cout << "states.size = " << states.size() << "\n";
	
	/*
	for (auto state: states) {
		D.map.left.insert(typename DFA<StateT,SymbolT>::Bimap::left_value_type(state.second, "whatever"));
		std::cout << state.second << "\n";
	}
	*/
	
	typedef typename DFA<StateT, SymbolT>::Bimap::value_type insert_type;
	
	for (auto it: states) {
		bool final = false;
		std::string s = "{";
		for (auto subs: it.first) {
			s += N.realState(subs) + ",";
			if (N.isFinal(subs)) {
				final = true;
			}
		}
		s += "}";
		D.map.insert(insert_type(it.second, s));
		
		if (final) D.F.insert(it.second);
	}
	
	return D;
}


template<	typename StateT,
			typename SymbolT,
			SymbolT epsilon>
void recursive_add(DFA<StateT, SymbolT>& D, eNFA<StateT, SymbolT, epsilon>& N,
		std::map<std::set<int>, int>& states, std::set<int>& current) {
	Walker<eNFA<StateT, SymbolT, epsilon>> w(N, current);
	
	for (SymbolT symb: D.sigma) {
		w.input(symb);
		//print(w.current);
		auto wtf = states.find(w.current);
		if (wtf == states.end()) {
			states[w.current] = states.size()-1; // this -1 is a *bit* weird
			recursive_add(D, N, states, w.current);
		} else {
			//print(wtf->first);
		}
		
		D.set_delta(states.at(current), symb, states.at(w.current));
		
		w.current = current; // reset
	}
}



