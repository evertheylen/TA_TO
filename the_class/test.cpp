#include "FSM.h"
#include <iostream>

using namespace std;

int main() {
	s_DFA D({"q0", "q1"},  // states (numbered)
			{'a', 'b', 'c'},  // alphabet
			0, {1});  // from -> {to}
	
	// the delta function must be manually set.
	D.set_delta(D.ID("q0"), 'b', D.ID("q1"));
	
	cout << D.realState(1);
}
	