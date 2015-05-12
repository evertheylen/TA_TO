/*
 * file.h
 *
 *  Created on: 12 May 2015
 *      Author: stijn
 */

#include <string>

#ifndef FILE_H_
#define FILE_H_

class File {
public:
	/*
	 * Raises exception when file not found.
	 */
	File(std::string filename);

	/*
	 * Returns all FAST comments (lines starting with '>' or ';').
	 */
	const std::string& get_description() const;

	/*
	 * Returns the filename.
	 */
	const std::string& get_name() const;
};


#endif /* FILE_H_ */
