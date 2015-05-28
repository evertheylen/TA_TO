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

	/*
	 * Returns all FASTA comments (lines starting with '>' or ';').
	 */
	const std::string& get_description() const;

	/*
	 * Returns the filename.
	 */
	const std::string& get_name() const;

	Suffix3* suffixtree;
	std::string comments;
	int ID;
	
private:
	std::ifstream file;
	std::string content;
	std::string name;
};


#endif /* FILE_H_ */
