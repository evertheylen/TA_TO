/*
//Description: Source code for main
//Author: Anthony Hermans
//Date: 11/03/15
*/
#include <stdio.h>
#include <iostream>
#include <fstream>
#include "DFA-to-REGEX.h"
#include "TinyXML/tinyxml.h"
using namespace std;

int main(int argc, char const* argv[]){
	cout << "Converting a DFA to REGEX using the state elimination technique.\n";
	TiXmlDocument inputDFA;
	if (argc == 1){
		cerr << "U need to specify an input DFA.\n";
		return 0;
	}
	if (inputDFA.LoadFile(argv[1]) == false){
		cerr << "Problems with the loading of the DFA ==> shut down.\n";
		return 0;
	}
	
	Automaton DFA;
	DFA.readXML();

	ofstream regex_out;
	regex_out.open("REGEX.txt");
	regex_out << "This text file contains the regex expression of the specified inputDFA:\n";
	regex_out << DFA.StateEliminationTechnique();
	regex_out.close();
	
}

