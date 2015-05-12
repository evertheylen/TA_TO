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
	int loc = 0;
    if (f.good()) {
    	//std::fstream suffixfile;
    	//suffixfile.open(filename.c_str());
    	std::stringstream comments;
    	//std::stringstream suffix;
    	std::string line;
    	char input;
    	SuffixTree s;
    	while (f.good()) {
    		f.get(input);
    		if (f.eof()) {
    			break;
    		}
    		if (input != '>' && input != '\n') {
    			s.create(input, loc);
    			loc++;
    		} else if (input == '>'){
    			while (input != '\n') {
    				f.get(input);
    				comments << input;
    				loc++;
    			}
    		} else {
    			loc++;
    		}
    	}
    	/*while (std::getline(suffixfile, line)) {
    		if (line[0] != '>') {
    			suffix << line << std::endl;
    		} else {
    			comments << line << std::endl;
    		}
    	}*/

    	//SuffixTree s(f);
		std::ofstream output_file;
		output_file.open("Commentsofthefastafile.txt");
		output_file << "comments: \n" << comments.str();
		//output_file << "suffix: \n" << suffix.str();
		output_file.close();
		output_file.open("fastasuffixtree.gv");
		output_file << s;
		output_file.close();
    } else {
        throw 1;
    }
    f.close();

}

