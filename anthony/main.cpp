/*
 *Description: Main for DFA to REGEX
 *Author: Anthony Hermans
 *Date: 26/03/15
 */
 
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "DFA-to-REGEX.h"
#include "TinyXML/tinyxml.h"
using namespace std;

int main(int argc, char const* argv[]){
	cout << "Converting a DFA to REGEX using the state elimination technique.\n";
	TiXmlDocument input_DFA;
	if (argc == 1){
		cerr << "U need to specify an input DFA.\n";
		return 0;
	}
	if (input_DFA.LoadFile(argv[1]) == false){
		cerr << "Problems with the loading of the DFA ==> shut down.\n";
		return 0;
	}
	
	Automaton DFA;
	DFA.readXML(input_DFA);

	cout << "The regex expression of the DFA can be found in the file: FinalRegex.txt\n";
	ofstream regex_out;
	regex_out.open("FinalRegex.txt");
	regex_out << "This text file contains the regex expression of the specified DFA:\n";
	regex_out << DFA.StateEliminationTechnique();
	regex_out.close();
	
}

