/*
 * file.cpp
 *
 *  Created on: 12 May 2015
 *      Author: stijn
 */

#include "file.h"
#include <fstream>
#include <iostream>

File::File(std::string filename) {
	std::ifstream file;
	file.open(filename);
}
