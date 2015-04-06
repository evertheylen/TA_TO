/*
 * compatible_algorithm.h
 *
 *  Created on: 29 Mar 2015
 *      Author: stijn
 */

#include "../the_class/FSM.h"

#ifndef COMPATIBLE_ALGORITHM_H_
#define COMPATIBLE_ALGORITHM_H_

s_DFA make_product(s_DFA& auto1, s_DFA& auto2, bool end_union);
s_DFA Union(s_DFA& auto1, s_DFA& auto2);
s_DFA Intersection(s_DFA& auto1, s_DFA& auto2);

#endif /* COMPATIBLE_ALGORITHM_H_ */
