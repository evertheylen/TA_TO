
#include <iostream>
#include <set>
#include <list>
#include <map>
#include <string>
#include <cassert>

// A CFG is represented with a 4-tuple:
//  G = (V,T,P,S)
//       V == set of variables
//       T == set of terminals (aka alphabet)
//       P == production rules, for example:
//              P --> 0P1
//              P --> 1P0
//              P --> 0
//       S == start symbol, \in V
//

template <	typename VarT,
			typename TermT,
			typename SymbolT>  	// Common parent of VarT and TermT, or all three equal
								// if not equal, they have to be able to dynamic_cast!
class CFG {
public:
	typedef VarT Variable;
	typedef TermT Terminal;
	typedef SymbolT Symbol;
	typedef std::set<Variable> VarSet;
	typedef std::set<Terminal> TermSet;
	
	typedef std::map<Variable, std::set<std::basic_string<Symbol>>> Productions;
	
	VarSet V;
	TermSet T;
	Productions P;
	Variable S;
	
	CFG(VarSet _V, TermSet _T, Variable _S):
			V(_V), T(_T), S(_S) {}
	
	void set_production(Variable from, std::set<std::basic_string<Symbol>> prod) {
		assert(V.find(from) != V.end());
		P[from] = prod;
	}
};


// A PDA is represented with a 7-tuple (sometimes 6-tuple, empty 7th component)
// P = (Q, Sigma, Gamma, delta, q_0, Z_0, F)
//      Q == set of states
//      Sigma == alphabet for the input string
//      Gamma == alphabet for the stack
//      delta == (q: Q, a: Gamma or eps, X: Gamma) --> {(
//      q0 == start state, \in Q
//      Z0 == start symbol for the stack
//      F == end states, \subset Q (if accept by states <-> accept by empty stack)

template <	typename SigmaSymbolT,
			typename GammaSymbolT>
class PDA {
public:
	typedef SigmaSymbolT SigmaSymbol;
	typedef GammaSymbolT GammaSymbol;
	typedef int State;
	typedef std::set<SigmaSymbol> SigmaT;
	typedef std::set<GammaSymbol> GammaT;
	
	int num_states; // Q = [0, num_states[
	SigmaT Sigma;
	GammaT Gamma;
	State q0;
	GammaSymbol Z0;
	SigmaT F;
	
	PDA(int _num_states, SigmaT _Sigma, GammaT _Gamma, State _q0, GammaSymbol _Z0, SigmaT _F):
			num_states(_num_states), Sigma(_Sigma), Gamma(_Gamma), q0(_q0), Z0(_Z0), F(_F) {}
	
};
	