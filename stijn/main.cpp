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

	dot_output.open("test_automaton1.gv");
	dot_output << auto1;
	dot_output.close();

	dot_output.open("test_automaton2.gv");
	dot_output << auto2;
	dot_output.close();
	//auto1 *= auto2;
	Automaton product = auto1 * auto2;
	product.set_name("End Product Automaton");
	dot_output.open("test_product_automaton1.gv");
	dot_output << product;
	dot_output.close();
}






