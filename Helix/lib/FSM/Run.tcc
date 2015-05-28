
#include <string>
#include <iostream>
#include <set>
#include <typeinfo>

#include "FSM.h"

//BEGIN -----[ Walker (abstract base) ]-----------------------

// this class is not really used yet
// (and it shouldn't be for performance (because of inheritance etc))
template <typename FSMT>
class Walker {
public:
	
	FSMT& automaton;
	typename FSMT::DeltaResult current;
	
	Walker(FSMT _automaton, typename FSMT::DeltaResult _current):
			automaton(_automaton), current(_current) {}
	
	virtual void input(typename FSMT::Symbol symb) = 0;
};

//END


//BEGIN -----[ Walker for DFA ]-------------------------------

template <typename StateT, typename SymbolT>
class Walker<DFA<StateT,SymbolT>> {
public:
	using D = DFA<StateT,SymbolT>;
	
	D& automaton;
	typename D::DeltaResult current;

	Walker(D& _automaton):
			automaton(_automaton), current(_automaton.q0) {}
	
	Walker(D& _automaton, typename DFA<StateT,SymbolT>::DeltaResult _current):
			automaton(_automaton), current(_current) {}
	
	void input(typename D::Symbol symb) {
		// ATTENTION I assume it's actually a valid DFA, so you should specify each and every
		// transition possible, it will fail otherwise!
		
		current = automaton.d_data[current][symb];
	}
	
	bool accepted() {
		return automaton.isFinal(current);
	}
};

//END



//BEGIN -----[ Walker for NFA ]------------------------------

template <typename StateT, typename SymbolT>
class Walker<NFA<StateT,SymbolT>> {
public:
	using N = NFA<StateT,SymbolT>;
	
	N& automaton;
	typename N::DeltaResult current;
	
	Walker(N& _automaton):
			automaton(_automaton), current({_automaton.q0}) {}
	
	Walker(N& _automaton, typename NFA<StateT,SymbolT>::DeltaResult _current):
			automaton(_automaton), current(_current) {}
	
	void input(typename N::Symbol symb) {
		typename N::DeltaResult new_current;
		for (auto from_ID: current) {
			auto second_d_data = automaton.d_data.find(from_ID);
			if (second_d_data != automaton.d_data.end()) {
				auto third_d_data = second_d_data->second.find(symb);
				if (third_d_data != second_d_data->second.end()) {
					new_current.insert(third_d_data->second.begin(), third_d_data->second.end());
				}
			}
		}
		current = new_current;
	}
	
	bool accepted() {
		for (auto s: current) {
			if (automaton.isFinal(s)) {
				return true;
			}
		}
		return false;
	}
};

//END


template<typename T>
void setprint_(std::set<T> s) {
	std::cout << "{";
	for (auto e: s) {
		std::cout << e << ", ";
	}
	std::cout << "}";
}


//BEGIN -----[ Walker for eNFA ]----------------------------

// Does not inherit
template <typename StateT, typename SymbolT, SymbolT epsilon>
class Walker<eNFA<StateT,SymbolT,epsilon>> {
public:
	using E = eNFA<StateT,SymbolT,epsilon>;
	
	E& automaton;
	typename E::DeltaResult current;
	
	Walker(E& _automaton):
			automaton(_automaton), current(_automaton.ECLOSE(_automaton.q0)) {}
	
	Walker(E& _automaton, typename eNFA<StateT,SymbolT,epsilon>::DeltaResult _current):
			automaton(_automaton), current(_current) {}
	
	
	void input(typename E::Symbol symb) {
		typename E::DeltaResult new_current;
		_input(symb, current, new_current);
// 		std::cout << "new_current="; setprint_(new_current);
// 		std::cout << "\n";
		std::cout << "INPUT after _input " << symb; setprint_(new_current);
		
		typename E::DeltaResult new_current_eps;
		for (auto s: new_current) {
			automaton._ECLOSE(s, new_current_eps);
		}
		
		std::cout << "\nINPUT after ECLOSE: "; setprint_(new_current_eps);
		std::cout << "\n";
		
		current = new_current_eps;
	}
	
	// as always, eNFA's are a PITA
	bool _input(typename E::Symbol symb, typename E::DeltaResult const& from_states, typename E::DeltaResult& to_states) {
		// Basically the input() from the NFA
		bool status = false;
		for (auto from_ID: from_states) {
			auto second_d_data = automaton.d_data.find(from_ID);
			if (second_d_data != automaton.d_data.end()) {
				auto third_d_data = second_d_data->second.find(symb);
				if (third_d_data != second_d_data->second.end()) {
					to_states.insert(third_d_data->second.begin(), third_d_data->second.end());
					status = true; // yes, I did insert!
				}
			}
		}
		return status;
	}
	
	// What do you think is more maintainable? Some copy-pasting or templated inheritance? /s
	bool accepted() {
		for (auto s: current) {
			if (automaton.isFinal(s)) {
				return true;
			}
		}
		return false;
	}
};

//END


//BEGIN -----[ Runner (for all) ]--------------------------

template <typename FSMT>
class Runner {
public:
	FSMT& automaton;
	
	Runner(FSMT& _a): automaton(_a) {}
	
	template <typename T>
	bool process(T str) {
		Walker<FSMT> walker(automaton);
		for (auto symb: str) {
			walker.input(symb);
		}
		return walker.accepted();
	}
	
	bool process(const char* str) {
		Walker<FSMT> walker(automaton);
		const char* symb = str;
		while (*symb != 0) {
			walker.input(*symb);
			++symb;
		}
		return walker.accepted();
	}
};


//END