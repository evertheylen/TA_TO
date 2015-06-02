/*
 * file.h
 *
 *  Created on: 12 May 2015
 *      Author: stijn
 */

#ifndef FILE_H_
#define FILE_H_

#include <string>
#include <fstream>
#include <vector>
//#include "suffix.h"

class Suffix3;

struct Gap {
	unsigned int position;
	unsigned int length;
	Gap(unsigned int p=0, unsigned int l=0);
};

class File {
public:
	/*
	 * Raises exception when file not found.
	 */
	File(std::string filename);
	
	File(std::ifstream& f);
	
	~File();
	
	void save(std::ofstream& f);
	

	/*
	 * Returns the filename.
	 */
	const std::string& get_name() const;
	
	unsigned int real_location(unsigned int loc);
	
	// Use this order to save and read
	std::string name;
	std::string comments;
	std::string* content;
	std::string path;
	std::vector<Gap> gaps;
	Suffix3* suffixtree;
	
};

template <typename T>
void write_simple(std::ofstream& file, T obj) {
	file.write((const char*) &obj, sizeof(T));
}

template <typename T>
T read_simple(std::ifstream& file) {
	T temp;
	file.read((char*) &temp, sizeof(T));
	return temp;
}

void write_string(std::ofstream& file, std::string& s);

std::string read_string(std::ifstream& file);

std::string* read_string_ptr(std::ifstream& file);

#endif /* FILE_H_ */
