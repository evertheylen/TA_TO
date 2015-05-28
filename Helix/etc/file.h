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
#include "allhopeliesonyou_suffix.h"

class File {
public:
	/*
	 * Raises exception when file not found.
	 */
	File(std::string filename);

	/*
	 * Returns all FASTA comments (lines starting with '>' or ';').
	 */
	const std::string& get_description() const;

	/*
	 * Returns the filename.
	 */
	const std::string& get_name() const;

	Suffix3* suffixtree;
private:
	std::ifstream file;
	std::string content;
	std::string name;
};


#endif /* FILE_H_ */
