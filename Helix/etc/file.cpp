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
    	std::fstream suffixfile;
    	suffixfile.open(filename.c_str());
    	std::stringstream comments;
    	std::stringstream suffix;
    	char input;
    	while (f.good()) {
    		f.get(input);
    		if (f.eof()) {
    			break;
    		}
    		if (input != '>' && input != '\n') {
    			suffix << input;
    		} else if (input == '>'){
    			while (input != '\n') {
    				f.get(input);
    				comments << input;
    			}
    		}
    	}
    	std::string str = suffix.str();
    	suffixtree = new Suffix3(str);
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

