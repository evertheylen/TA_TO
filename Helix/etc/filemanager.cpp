
#include "filemanager.h"

Filemanager::Filemanager(bool _offload):
		offload(_offload) {}


File* Filemanager::add_file(std::string filename) {
	bool is_binary = (filename.length() > 6 && filename.substr(filename.length()-7) == ".suffix");
	int ID = next_ID;
	next_ID++;
	if (is_binary) {
		if (offload) {
			std::cout << "adding binary file [" << ID << "] offloaded. filename is " << filename << "\n";
			filenames[ID] = filename;
			return get_file(ID);
		} else {
			std::cout << "adding binary file [" << ID << "] to RAM. filename is " << filename << "\n";
			std::ifstream file;
			file.open(filename);
			files[ID] = new File(file);
			// also put it in the filenames map
			filenames[ID] = filename;
			return files[ID];
		}
	} else {
		if (offload) {
			std::cout << "adding fasta file [" << ID << "] and saving to file. filename is " << filename << "\n";
			if (current != nullptr) delete current;
			std::string saved_filename = filename + ".suffix";
			std::cout << "    saving on " << saved_filename << "\n";
			current = new File(filename);
			std::ofstream file;
			file.open(saved_filename);
			current->save(file);
			file.close();
			filenames[ID] = saved_filename;
			
			current_ID = ID;
			return current;
		} else {
			std::cout << "adding fasta file [" << ID << "] and saving to RAM. filename is " << filename << "\n";
			files[ID] = new File(filename);
			return files[ID];
		}
	}
}


void Filemanager::enable_offloading() {
	// write all files to disk
	if (!offload) {
		for (int ID=0; ID<next_ID; ID++) {
			if (filenames.find(ID) == filenames.end()) {
				std::string saved_filename = files[ID]->get_name() + ".suffix";
				std::cout << "(enable) saving " << ID << " to " << saved_filename << "\n";
				std::ofstream file;
				file.open(saved_filename);
				files[ID]->save(file);
				file.close();
				filenames[ID] = saved_filename;
			}
			std::cout << "(enable) deleting from RAM: " << ID << "\n";
			delete files[ID];
		}
		
		files.clear();
		offload = true;
		current = nullptr;
		current_ID = -1;
	}
}

void Filemanager::disable_offloading() {
	// load all files from disk
	if (offload) {
		for (int ID=0; ID<next_ID; ID++) {
			std::ifstream file;
			file.open(filenames[ID]);
			std::cout << "(disable) loading file " << filenames[ID] << " to file with ID: " << ID << "\n";
			files[ID] = new File(file);
			file.close();
		}
		
		current = nullptr;
		current_ID = -1;
		
		//filenames.clear();
		offload = false;
	}
}

File* Filemanager::get_file(int ID) {
	if (offload) {
		if (current_ID == ID) {
			std::cout << "loading current file in RAM with ID: " << current_ID << "\n";
		} else {
			delete current;
			std::ifstream file;
			file.open(filenames[ID]);
			current = new File(file);
			current_ID = ID;
			file.close();
			std::cout << "loading file in RAM with ID: " << current_ID << "\n";
		}
		return current;
	} else {
		std::cout << "loading file directly from RAM with ID: " << ID << "\n";
		return files[ID];
	}
}

Filemanager::~Filemanager() {
	for (auto it: files) {
		delete it.second;
	}
	if (offload && current != nullptr) {
		delete current;
	}
}

