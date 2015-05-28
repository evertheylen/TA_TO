
#include "filecompare.h"

/**
Auxiliary function for file manipulation in unit tests.
*/
bool fileCompare(std::string leftFileName, std::string rightFileName) {
	std::ifstream leftFile, rightFile;
	char leftRead, rightRead;
	bool result;

	// Open the two files.
	leftFile.open(leftFileName);
	if (!leftFile.is_open()) {
		return false;
	}
	rightFile.open(rightFileName);
	if (!rightFile.is_open()) {
		leftFile.close();
		return false;
	}
	result = true; // files exist and are open; assume equality unless a counterexamples shows up.
	while (result && leftFile.good() && rightFile.good()) {
		leftFile.get(leftRead);
		rightFile.get(rightRead);
		
		if (leftFile.eof() && rightFile.eof()) {
			break;
		}
		if (leftFile.eof() || rightFile.eof()) {
			result = false;
			break;
		}
		
		result = (leftRead == rightRead);
	}
	
	if (result) {
		// last read was still equal; are we at the end of both files ?
		result = (!leftFile.good()) && (!rightFile.good());
	}
	
	leftFile.close();
	rightFile.close();
	return result;
}
