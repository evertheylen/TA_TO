
#include "RE-eNFA.h"

bool isInAlphabet(std::string str, char c){
	for (string::iterator it = str.begin(); it!= str.end(); it++){
		if (*it == c){
			return true;
		}
		continue;
	}
	return false;
}
