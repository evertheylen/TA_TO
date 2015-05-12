/*
 * file.cpp
 *
 *  Created on: 12 May 2015
 *      Author: stijn
 */

#include "file.h"
#include <fstream>
#include <iostream>
//#include <boost/filesystem.hpp>

File::File(std::string filename) {
	std::ifstream file;
	//boost::filesystem::path myfile(filename);
	//if (!boost::filesystem::exists(myfile)) {
		std::cerr << "File with name " << filename << " doesn't exists.\n";
		throw 1;
	//}
	file.open(filename);
}
