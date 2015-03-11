#include <iostream>
#include "tinyxml/tinyxml.h"

#include "FSM.h"

int main(int argc, char** argv) {
	TiXmlDocument doc;

	if(argc <= 1) {
		std::cerr << "Not enough arguments given.\n";
		return 1;
	}
	
	std::cout << "Starting...\n";
	
	bool doc_loaded = doc.LoadFile(argv[1]);
	
	if (!doc_loaded) {
		std::cerr << "Error loading board.\n";
	}
	
	Automaton N(doc);
	
	std::cout << "About to print stuff...\n";
	
	std::cout << N.transitions[N.start].size() << "\n";
	
	std::cout << N.transitions[N.start][N.alphabet[0]][0]->name << "\n";

}
