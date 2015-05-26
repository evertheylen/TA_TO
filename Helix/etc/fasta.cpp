/*
 *	Author: Anthony Hermans
 *	Date: 26/05/2015
 *	Information: All the functions about fasta
*/

#include "fasta.h"
#include <string>
#include <iostream>

using namespace std;

string fastaReplace(string str){
	string returnVal;
	
	for (char c: str){
		switch (c) {
			case 'R':
				returnVal += "(A+G)";
				break;
			case 'Y':
				returnVal += "(C+T+U)";
				break;
			case 'K':
				returnVal += "(G+T+U)";
				break;
			case 'M':
				returnVal += "(A+C)";
				break;
			case 'S':
				returnVal += "(C+G)";
				break;
			case 'W':
				returnVal += "(A+T+U)";
				break;
			case 'B':
				returnVal += "(C+G+T+U)";
				break;
			case 'D':
				returnVal += "(A+G+T+U)";
				break;
			case 'H':
				returnVal += "(A+C+T+U)";
				break;
			case 'V':
				returnVal += "(A+C+G)";
				break;
			case 'N':
				returnVal += "(A+C+G+T+U)";
				break;
			default:
				returnVal += c;
		}
	}
	
	return returnVal;
};
