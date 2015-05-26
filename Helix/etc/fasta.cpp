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
	
	for (string::iterator it = str.begin(); it != str.end(); it++){
		if (*it == 'A' or *it == 'C' or *it == 'U' or *it == 'T' or *it == 'G'){
			returnVal += *it;
			continue;
		}
		if (*it == 'R'){
			returnVal += "(A+G)";
			continue;
		}
		if (*it == 'Y'){
			returnVal += "(C+T+U)";
			continue;
		}
		if (*it == 'K'){
			returnVal += "(G+T+U)";
			continue;
		}
		if (*it == 'M'){
			returnVal += "(A+C)";
			continue;
		}
		if (*it == 'S'){
			returnVal += "(C+G)";
			continue;
		}
		if (*it == 'W'){
			returnVal += "(A+T+U)";
			continue;
		}
		if (*it == 'B'){
			returnVal += "(C+G+T+U)";
			continue;
		}
		if (*it == 'D'){
			returnVal += "(A+G+T+U)";
			continue;
		}
		if (*it == 'H'){
			returnVal += "(A+C+T+U)";
			continue;
		}
		if (*it == 'V'){
			returnVal += "(A+C+G)";
			continue;
		}
		if (*it == 'N'){
			returnVal += "(A+C+G+T+U)";
			continue;
		}
		continue;
	}
	
	return returnVal;
};
