#include "TFA.h"
#include <vector>
#include <iostream>

// for use in the table
uint table_func(uint k, uint l, uint n) {
	// n is the number of states
	
	if (k > l) {
		uint temp = l;
		l = k;
		k = temp;
	}
	
	// k <= l from here on
	
	// (n(n+1))/2 - ((n-k)(n-k+1))/2 + (l-i-1)
	// smaller:
	return (2*n*k - k*k - k + 2*l - 2)/2;
}

s_DFA TFA(s_DFA D) {
	// Not very optimized yet :)
	
	int n = D.num_states;
	int N = (n*(n+1))/2;
	
	// Step 0
	D.lazy_evaluation();
	
	// Step 1: create the table
	// I went totally crazy for this.
	std::vector<bool> table_vec(N);
	#define table(k,l) table_vec[table_func(k,l,n)]
	
	std::cout << "n = " << n << ", f(31,45,54) == " << table_func(31,45,54) << "\n";
	
	// Step 2: mark all the pairs with a final and a nonfinal state
	
	// Step 3: mark all the rest ... ?
	
	// Step 4: build the equivalent 'blocks'
	
	// Step 5: reconstruct DFA based on the new 'blocks'
	
	
	return D;
}
