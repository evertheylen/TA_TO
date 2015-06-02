
#ifndef _H_Extra
#define _H_Extra

#include "FSM/FSM.h"
#include "./eNFA-DFA/eNFA-DFA.h"
#include "./Product/product.h"
#include "./RE-eNFA/RE-eNFA.h"
#include "./TFA/TFA.h"

template <typename R, typename S, typename T>
void writedot(FSM<R,S,T>* F, string name) {
	std::ofstream output_file;
	output_file.open(name);
	F->to_dot(output_file);
	output_file.close();
}

template<	typename StateT,
			typename SymbolT,
			SymbolT epsilon>
DFA<StateT, SymbolT> to_DFA(std::string& regex) {
	auto E = RE_to_eNFA<StateT, SymbolT, epsilon>(regex);
// 	writedot(&E, "eNFA.doot");
	auto bad_D = MSSC(E);
// 	writedot(&bad_D, "DFA.doot");
	auto good_D = TFA(bad_D, false);
// 	writedot(&good_D, "opt_DFA.doot");
	return good_D;
}

#endif
