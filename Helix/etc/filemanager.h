
#include "file.h"

#include <map>
#include <string>
#include <iostream>

class Filemanager {
public:
	
	Filemanager(bool _offload=false);
	
	~Filemanager();
	
	bool offload;
	int next_ID = 0;
	
	// Offload mode
	std::map<int, std::string> filenames;
	File* current;
	int current_ID;
	
	// Memory mode
	std::map<int, File*> files;
	
	void enable_offloading();
	void disable_offloading();
	
	File* get_file(int ID);
	
	File* add_file(std::string filename);
	
};
