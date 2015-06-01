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
#include "suffix.h"

class File {
public:
	/*
	 * Raises exception when file not found.
	 */
	File(std::string filename, int _ID);
	
	File(std::ifstream& f);
	
	void save(std::ofstream& f);
	
	/*
	 * Returns all FASTA comments (lines starting with '>' or ';').
	 */
	const std::string& get_description() const;

	/*
	 * Returns the filename.
	 */
	const std::string& get_name() const;
	
	// Use this order to save and read
	std::string name;
	std::string comments;
	std::string* content;
	std::string path;
	int ID;
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
