/*
//Description: Source code for main
//Author: Anthony Hermans
//Date: 11/03/15
*/
#include <stdio.h>
#include <iostream>
#include "DFA-to-REGEX.h"
#include "TinyXML/tinyxml.h"
using namespace std;
int main(int argc, char const* argv[]){
	cout << "Converting a DFA to REGEX.\n";
	TiXmlDocument DFA;
	if (argc >= 2){
		cerr << "U need to specify an input DFA.\n";
		return 0;
	}
	if (DFA.LoadFile(argv[1]) == false){
		cerr << "Problems with the loading of the DFA ==> shut down.\n";
		return 0;
	}
	
}

