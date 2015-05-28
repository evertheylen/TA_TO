
#ifndef _H_Extra
#define _H_Extra

#include "FSM/FSM.h"
#include "./eNFA-DFA/eNFA-DFA.h"
#include "./Product/product.h"
#include "./RE-eNFA/RE-eNFA.h"
#include "./TFA/TFA.h"

template<	typename StateT,
			typename SymbolT,
			SymbolT epsilon>
DFA<StateT, SymbolT> to_DFA(std::string& regex) {
	auto E = RE_to_eNFA<StateT, SymbolT, epsilon>(regex);
	auto bad_D = MSSC(E);
	// auto good_D = TFA(bad_D);
	return bad_D;
}

#endif
