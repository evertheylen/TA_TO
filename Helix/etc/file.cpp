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

#include <exception>
#include <stdexcept>

File::File(std::string filename, int _ID):
		ID(_ID) {
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
    	//std::stringstream suffix;
    	char input;
    	//std::cout << "Printing name from file  " << name << std::endl;
    	suffixtree = new Suffix3();
    	while (f.good()) {
    		f.get(input);
    		if (f.eof()) {
    			break;
    		}
    		if (input != '>' && input != ';' && input != '\n') {
    			suffixtree->add_char(input);
    		} else if (input == '>' || input == ';'){
    			while (input != '\n') {
    				f.get(input);
    				comments += input;
    			}
    			comments += '\n';
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
        throw std::runtime_error("Something went wrong while loading the file.");
    }
    f.close();
}

const std::string& File::get_name() const {
	return name;
}



