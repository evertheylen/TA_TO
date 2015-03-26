/*
 * main.cpp
 *
 *  Created on: 5 Mar 2015
 *      Author: stijn
 */
#include <string>
#include <fstream>
#include <iostream>
#include "lib/tinyxml.h"
#include "algorithm.h"


int main(int argc, char const* argv[]){
	TiXmlDocument doc_automaton1, doc_automaton2;

	if(argc <= 2) {
		std::cerr << "Not enough arguments given.\n";
		return 1;
	}

	bool automaton1_loaded = doc_automaton1.LoadFile(argv[1]);
	bool automaton2_loaded = doc_automaton2.LoadFile(argv[2]);

	if (!automaton1_loaded) {
		std::cerr << "Error loading automaton1.\n";
	}
	if (!automaton2_loaded) {
		std::cerr << "Error loading automaton2.\n";
	}

	if (!(automaton1_loaded && automaton2_loaded)) {
		return 1;
	}

	Automaton auto1(doc_automaton1);
	Automaton auto2(doc_automaton2);

	std::ofstream dot_output;
	const char* name = (auto1.get_name() + ".gv").c_str();
	dot_output.open(name);
	dot_output << auto1;
	dot_output.close();

	name = (auto2.get_name() + ".gv").c_str();
	dot_output.open(name);
	dot_output << auto2;
	dot_output.close();
	//auto1 *= auto2;
	Automaton product = Union(auto1, auto2);
	product.set_name("product_union");

	name = (product.get_name() + ".gv").c_str();
	dot_output.open(name);
	dot_output << product;
	dot_output.close();
	Automaton product2 = Intersection(auto1, auto2);
	product2.set_name("product_intersection");

	name = (product2.get_name() + ".gv").c_str();
	dot_output.open(name);
	dot_output << product2;
	dot_output.close();
}






