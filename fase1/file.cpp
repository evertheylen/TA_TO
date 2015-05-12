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
	std::ifstream f(filename.c_str());
    if (f.good()) {
    	f.open(filename);
    	std::stringstream comments;
    	std::stringstream suffix;
    	for( std::string line; getline(f, line ); ){
    		if (line[0] == '>') {
    			comments << line << std::endl;
    		} else {
    			suffix << line << std::endl;
    		}
    	}

    	//SuffixTree s(suffix);
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

