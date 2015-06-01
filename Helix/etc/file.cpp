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

#include "suffix.h"

#include <exception>
#include <stdexcept>

File::File(std::string filename, int _ID):
		ID(_ID) {
	std::fstream f(filename.c_str());
	int loc = 0;
    if (f.good()) {
		content = new std::string;
		
    	name = filename;
    	path = filename;
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
    	while (f.good()) {
    		f.get(input);
    		if (f.eof()) {
    			break;
    		}
    		if (input != '>' && input != ';' && input != '\n') {
				//suffixtree->add_char(input);
				*content += input;
    		} else if (input == '>' || input == ';'){
    			while (input != '\n') {
    				f.get(input);
    				comments += input;
    			}
    		}
    	}
    	//suffixtree->add_char('$');
    	*content += '$';
    	suffixtree = new Suffix3(content);

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

File::File(std::ifstream& f) {
	// std::string name;
	name = read_string(f);
	// std::string comments;
	comments = read_string(f);
	// std::string* content;
	content = read_string_ptr(f);
	// std::string path;
	path = read_string(f);
	// int ID;
	ID = read_simple<int>(f);
	
	// Suffix3* suffixtree;
	suffixtree = new Suffix3(content, f);
}

void File::save(std::ofstream& f) {
	// std::string name;
	write_string(f, name);
	// std::string comments;
	write_string(f, comments);
	// std::string* content;
	write_string(f, *content);
	// std::string path;
	write_string(f, path);
	// int ID;
	write_simple<int>(f, ID);
	
	// Suffix3* suffixtree;
	suffixtree->save(f);
}

File::~File() {
	delete content;
	delete suffixtree;
}



const std::string& File::get_name() const {
	return name;
}


// Helpers

void write_string(std::ofstream& file, std::string& s) {
	write_simple<uint>(file, s.size());
	file.write(s.c_str(), s.size());
}

std::string read_string(std::ifstream& file) {
	uint size = read_simple<uint>(file);
	std::string result(size, 'x');
	file.read((char*) result.c_str(), size);
	return result;
}

std::string* read_string_ptr(std::ifstream& file) {
	uint size = read_simple<uint>(file);
	std::string* result = new std::string(size, 'x');
	file.read((char*) result->c_str(), size);
	return result;
}
