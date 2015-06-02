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

Gap::Gap(unsigned int p, unsigned int l):
		position(p), length(l) {}


File::File(std::string filename) {
	std::fstream f(filename.c_str());
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
		uint N_gap_length=0;
		uint i=0;
    	//std::cout << "Printing name from file  " << name << std::endl;
    	while (f.good()) {
    		f.get(input);
    		if (f.eof()) {
    			break;
    		}
    		
    		if (input == '\n') continue;
    		
    		if (input == '>' || input == ';') {
    			while (input != '\n') {
    				f.get(input);
    				comments += input;
    			}
    			continue;
    		}
    		
    		// check gaps
    		if (input == 'N') {
				N_gap_length++;
				continue;
			} else {
				if (N_gap_length>20) {
					gaps.push_back(Gap(i, N_gap_length));
					//std::cout << "pushed gap, loc:" << i-N_gap_length << ", len:" << N_gap_length << ", i:" << i <<"\n";
					*content += "NNNNN";
				}
				N_gap_length = 0;
			}
			
			*content += input;
    		i++;
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

unsigned int File::real_location(unsigned int loc) {
	uint real_loc = loc;
	for (Gap& g: gaps) {
		if (g.position < loc) {
			real_loc += g.length;
		} else {
			break;
		}
	}
	return real_loc;
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
	// std::vector<Gap> gaps;
	uint gap_vector_length = read_simple<int>(f);
	gaps = std::vector<Gap>(gap_vector_length);
	f.read((char*) &gaps[0], gap_vector_length*sizeof(Gap));
	
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
	// std::vector<Gap> gaps;
	write_simple<int>(f, gaps.size());
	f.write((const char*) &gaps[0], gaps.size()*sizeof(Gap));
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
