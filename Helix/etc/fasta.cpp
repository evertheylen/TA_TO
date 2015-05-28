/*
 *	Author: Anthony Hermans
 *	Date: 26/05/2015
 *	Information: All the functions about fasta
*/

#include "fasta.h"
#include <string>
#include <vector>
#include <iostream>

using namespace std;

vector<string> fastaReplace(string str){
	vector<string> returnVal;
	string new_str;
	for (char c: str){
		switch (c) {
			case ';':
				returnVal.push_back(new_str);
				new_str.clear();
				break;
			case 'R':
				new_str += "(A+G)";
				break;
			case 'Y':
				new_str += "(C+T+U)";
				break;
			case 'K':
				new_str += "(G+T+U)";
				break;
			case 'M':
				new_str += "(A+C)";
				break;
			case 'S':
				new_str += "(C+G)";
				break;
			case 'W':
				new_str += "(A+T+U)";
				break;
			case 'B':
				new_str += "(C+G+T+U)";
				break;
			case 'D':
				new_str += "(A+G+T+U)";
				break;
			case 'H':
				new_str += "(A+C+T+U)";
				break;
			case 'V':
				new_str += "(A+C+G)";
				break;
			case 'N':
				new_str += "(A+C+G+T+U)";
				break;
			default:
				new_str += c;
		}
	}
	returnVal.push_back(new_str);
	return returnVal;
};
