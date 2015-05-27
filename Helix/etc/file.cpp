/*
 * file.cpp
 *
 *  Created on: 12 May 2015
 *      Author: stijn
 */

#include "file.h"
#include <fstream>
#include <sstream>
#include <list>
#include <iostream> 
//#include "ukkonen_suffix.h"

File::File(std::string filename) {
	std::fstream f(filename.c_str());
	int loc = 0;
    if (f.good()) {
    	name = filename;
    	std::string good_name;
    	for (int i = name.length()-1; i >= 0; i--) {
            if (name[i] == '/') {    	
                break;
            } else {
                good_name += name[i];
            }
    	}
    	name = std::string(good_name.rbegin(), good_name.rend());
    	std::fstream suffixfile;
    	suffixfile.open(filename.c_str());
    	std::string comments;
    	//std::stringstream suffix;
    	char input;
    	suffixtree = new Suffix3();
    	while (f.good()) {
    		f.get(input);
    		if (f.eof()) {
    			break;
    		}
    		if (input != '>' && input != '\n') {
    			suffixtree->add_char(input);
    		} else if (input == '>'){
    			while (input != '\n') {
    				f.get(input);
    				comments += input;
    			}
    		}
    	}

    	suffixtree->build();

    	//suffixtree = new Suffix3(str);
    	/*std::string search = "sss";
    	std::vector<int> result = suffixtree->search_string(search, 1);
    	int size = result.size();
    	std::ofstream output_file;
		output_file.open("Commentsofthefastafile.txt");
		output_file << "comments: \n" << comments.str();
		//output_file << "suffix: \n" << suffix.str();
		output_file.close();
 		std::string filename = "_search_for_";
		filename += search;
 		std::ofstream f(filename);
 		f << "Occurrences of the pattern " << search << std::endl;
  		for (int k = 0; k < result.size(); k++) {
 			f << "Result found at position " << result.at(k) << "\n";
 		}*/
    } else {
        throw 1;
    }
    f.close();
}

const std::string& File::get_name() const {
	return name;
}

bool File::test(std::string search, int error) {
	std::vector<int> result = {}; //suffixtree->search_string(search, error); TODO
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

