#include "filecompare.h"
#include "filecompare.cpp"

#include <iostream>
#include <string>

int main(int argc, const char* argv[]) {
	if (argc != 3) return 1;
	
	std::string arg1(argv[1]);
	std::string arg2(argv[2]);
	
	std::cout << "comparing " << arg1 << " with " << arg2 << ": " << fileCompare(arg1, arg2) << "\n";
}
