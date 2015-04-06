/*
 * compatible_algorithm.cpp
 *
 *  Created on: 29 Mar 2015
 *      Author: stijn
 */

#include "compatible_algorithm.h"

s_DFA Union(s_DFA& auto1, s_DFA& auto2) {
	return make_product(auto1, auto2, true);
}

s_DFA Intersection(s_DFA& auto1, s_DFA& auto2) {
	return make_product(auto1, auto2, false);
}

s_DFA make_product(s_DFA& auto1, s_DFA& auto2, bool end_union) {
	assert(auto1.sigma == auto2.sigma); // Make sure the alphabets of the 2 automata are the same,
	s_DFA product;
	//product.set_name("Temporary Product Automaton");
	//product.product = true;

	/*
	 * For the product automaton I use stringstreams to create the names of the states. I create the combination of both names.
	 */

	// Create the alphabet of the product automaton.

	product.sigma = auto1.sigma;

	/*bool already_in_alphabet = false;
	for (unsigned int i = 0; i != auto2.alphabet.size(); i++) {
		for (unsigned int j = 0; j != auto1.alphabet.size(); j++) {
			if (auto2.alphabet.at(i) == auto1.alphabet.at(j)) {
				already_in_alphabet = true;
			}
		}
		if (!already_in_alphabet) {
			std::cerr << "Alphabets aren't equal!\n";
			assert("alphabets must be equal");
			return product;
		}
		already_in_alphabet = false;
	}*/

	// Create the states of the product automaton.
	std::stringstream startname;
	std::string name;
	for (int i = 0; i < auto1.num_states; i++) {
		for (int j = 0; j < auto2.num_states; j++) {
			startname << auto1.realState(i) << "/" << auto2.realState(j);
			name = startname.str();
			startname.str("");	// Clears the stringstream

			bool end;
			// end = is het origineel een eindstaat? En is het union of intersection?
			product.add_state(name, end);
			// fix het pushen van startstate?
			/*if (auto1.in_start(auto1.total_states.at(i)->get_name()) && auto2.in_start(auto2.total_states.at(j)->get_name())) {
				product.start.push_back(p);
			}
			if (!end_union) {
				if (auto1.in_end(auto1.total_states.at(i)->get_name()) && auto2.in_end(auto2.total_states.at(j)->get_name())) {
					product.end.push_back(p);
				}
			} else {
				if (auto1.in_end(auto1.total_states.at(i)->get_name()) || auto2.in_end(auto2.total_states.at(j)->get_name())) {
					product.end.push_back(p);
				}
			}*/

		}
	}

	// Construct the transitions of the product automaton via the Cartesian Product Construction.
/*
	for (unsigned int i = 0; i != product.num_states; i++) {

		State* p1_from = auto1.get_state(product.get_first_name(product.total_states.at(i)->get_name()));
		State* p2_from = auto2.get_state(product.get_second_name(product.total_states.at(i)->get_name()));
		std::stringstream name_to;
		std::string name;
		for (unsigned int j = 0; j != product.alphabet.size(); j++) {
			Transition* t1 = auto1.get_trans(p1_from->get_name(), product.alphabet.at(j));
			Transition* t2 = auto2.get_trans(p2_from->get_name(), product.alphabet.at(j));
			State* p1_to = t1->get_to();
			State* p2_to = t2->get_to();
			name_to << p1_to->get_name() << "/" << p2_to->get_name();
			name = name_to.str();
			name_to.str("");
			Transition* t = new Transition(product.total_states.at(i), product.alphabet.at(j), product.get_state(name));
			product.transitions.push_back(t);
		}
	}*/
	return product;
}

