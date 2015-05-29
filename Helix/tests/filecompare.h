

#include <string>
#include <fstream>
#include "FSM.h"
/**
Auxiliary function for file manipulation in unit tests.
*/
bool fileCompare(std::string leftFileName, std::string rightFileName);

template <typename R, typename S, typename T>
void write_dot(FSM<R,S,T>* F, std::string name) {
	std::ofstream output_file;
	output_file.open(name);
	F->to_dot(output_file);
	output_file.close();
};

