
#include "gtest/gtest.h"

// Thoroughly test the search algorithm for DFA+Suffix+Errors!

// Probably the hardest to do...

// Code from stijn, might be useful, but probably not :p
/*

bool File::test(std::string search, int error) {
	std::vector<int> result =  .... ? old search_string function is gone, use Query::Search
	if (error == 0) {
		for (int i = 0; i < result.size(); i++) {
			for (int j = 0; j < search.length(); j++) {
				if (search[j] != suffixtree->s[result.at(i)+j]) {
					std::cerr << "Error: " << search << " doesn't equal " << suffixtree->s[result.at(i)] << " in the file at position " << result.at(i) << std::endl;
					return false;
				}
			}
		}
	} else {
		for (int i = 0; i < result.size(); i++) {
		int k = 0;
			for (int j = 0; j < search.length(); j++) {
				if (search[j] != suffixtree->s[result.at(i)+j]) {
					//std::cerr << "Error: " << search << " doesn't equal " << content[result.at(i)] << " in the file at position " << result.at(i) << std::endl;
					k++;
					if (k > error) {
						std::cerr << "Error: " << search << " doesn't equal " << suffixtree->s[result.at(i)] << " in the file at position " << result.at(i) << std::endl;
						return false;
					}
				}
			}
		}
	}
	return true;
}

*/

