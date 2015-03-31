#include "product.h"
#include "assert.h"
#include <map>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

s_DFA product(s_DFA D1, s_DFA D2, bool intersection) {
	// Insert bug-free quality code here

	/*
	 * A Product Automaton can only be created out of 2 DFA's with the exact same alphabet.
	 * Therefore the first part of this algorithm ensures that the two alphabets are equal,
	 * and sets the alphabet of the new product automaton to be equal to one of the original alphabets.
	 */

	assert(D1.sigma == D2.sigma);
	s_DFA product;

	product.sigma = D1.sigma;

	/*
	 * I will create temporary pares of original states, to be able to check their original delta-function.
	 * At the end of the algorithm I will transform them to normal states and insert them into the product automaton.
	 */

	std::stringstream ss;
	ss << D1.realState(D1.q0) << "/" << D2.realState(D2.q0);
	std::string str = ss.str();
	ss.str("");

	for (int i = 0; i < D1.num_states; i++) {
		for (int j = 0; j < D2.num_states; j++) {
			ss << D1.realState(i) << "/" << D2.realState(j);
			std::string str = ss.str();
			ss.str("");
			if (intersection) {
				if (D1.isFinal(i) && D2.isFinal(j)) {
					product.add_state(str, true);
				} else {
					product.add_state(str, false);
				}
			} else {
				if (D1.isFinal(i) || D2.isFinal(j)) {
					product.add_state(str, true);
				} else {
					product.add_state(str, false);
				}
			}
			if (D1.q0 == i && D2.q0 == j) {
				product.q0 = product.ID(str);
			}
		}
	}

	for (int i = 0; i < D1.num_states; i++) {
		for (int j = 0; j < D2.num_states; j++) {
			for (auto k: product.sigma) {
				int D1_to = D1.delta(i, k);
				int D2_to = D2.delta(j, k);
				product.set_delta(i*D1.num_states + j, k, D1_to*D1.num_states + D2_to);
			}
		}
	}
	return product;
}
