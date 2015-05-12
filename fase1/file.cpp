/*
 * file.cpp
 *
 *  Created on: 12 May 2015
 *      Author: stijn
 */

#include "file.h"
#include "suffix.h"
#include <fstream>
#include <sstream>
#include <iostream>

File::File(std::string filename) {
	std::fstream f(filename.c_str());
    if (f.good()) {
    	std::fstream suffixfile;
    	suffixfile.open(filename.c_str());
    	std::stringstream comments;
    	std::stringstream suffix;
    	std::string line;
    	while (std::getline(suffixfile, line)) {
    		std::cout << line << std::endl;
    		if (line[0] != '>') {
    			suffix << line << std::endl;
    		} else {
    			comments << line << std::endl;
    		}
    	}

    	//SuffixTree s(f);
		std::ofstream output_file;
		output_file.open("filetestsuffix.gv");
		output_file << "comments: \n" << comments.str();
		output_file << "suffix: \n" << suffix.str();
		output_file.close();
    } else {
        throw 1;
    }
    f.close();

}

