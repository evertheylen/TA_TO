#include <iostream>
#include <string>
#include <list>
#include <fstream>

#include "RE-eNFA/RE-eNFA.h"
#include "Product/product.h"
#include "TFA/TFA.h"
#include "eNFA-DFA/eNFA-DFA.h"

#include "FSM.h"
#include "tinyxml.h"

#include "suffix.h"

#include "file.h"
#include "search.h"

using namespace std;

TiXmlDocument read(string name) {
	TiXmlDocument doc;
	doc.LoadFile(name);
	return doc;
}

template <typename R, typename S, typename T>
void write_dot(FSM<R,S,T>* F, string name) {
	std::ofstream output_file;
	output_file.open(name);
	F->to_dot(output_file);
	output_file.close();
}

std::string get_file_contents(std::string& filename) {
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	if (in) {
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return contents;
	}
	throw errno;
}


int main(int argc, char const* argv[]) {
	if (argc < 3) {
		cout << "Not enough arguments given.\n";
		return 1;
	}
	
	string mode = string(argv[1]);
	string arg = string(argv[2]);
	
	if (mode == "RE-eNFA") {
		if (argc < 4) {
			cout << "Not enough arguments given for creating the regex automaton.\n";
			return 1;
		}
		string filename = string(argv[3]);
		
		eNFA <std::string, char, 'e'> N = RE_to_eNFA<std::string, char, 'e'>(arg);
		
		write_dot(&N, filename);
	} else if (mode == "TFA") {
		s_DFA D;
		auto doc = read(arg);
		D.from_xml(doc);
		write_dot(&D, arg + ".orig.dot");
		
		s_DFA opt_D = TFA(D, true);
		
		write_dot(&opt_D, arg + ".dot");
	} else if (mode == "eNFA-DFA") {
		std::cout << "MSSC\n";
		s_eNFA N;
		auto doc = read(arg);
		N.from_xml(doc);
		
		s_DFA D = MSSC(N);
		
		write_dot(&N, arg+".org_dot");
		write_dot(&D, arg+".dot");
	} else if (mode == "union" || mode == "intersection") {
		if (argc < 5) {
			cout << "Not enough arguments given for creating the product automaton.\n";
			return 1;
		}
		string arg2 = string(argv[3]);
		string filename = string(argv[4]);
		
		s_DFA D1;
		//DFA <std::string, char> D1;
		//DFA <std::string, char> D2;
		auto doc = read(arg);
		D1.from_xml(doc);
		
		s_DFA D2;
		doc = read(arg2);
		D2.from_xml(doc);

		DFA <std::string, char> P = product(D1, D2, mode == "intersection");  // true --> intersection
		write_dot(&P, filename);
		std::cout << "written dot to " << filename << "\n";
	} else if (mode == "suffix" || mode == "suffixsearch") {
		File f(arg);
		if (mode == "suffix") {
			std::cout << "suffixtree stats:\n";
			f.suffixtree->stats(std::cout);
			std::ofstream stream;
			std::cout << "gaps: \n";
			for (Gap& g: f.gaps) {
				std::cout << " - gap: " << g.position << ", " << g.length << "\n";
			}
			stream.open(arg+".suffix");
			stream << *(f.suffixtree);
			stream.close();
		} else if (argc > 3) { // suffixsearch
			string pat(argv[3]);
			Query q(pat,0,0,0,0,0);
			q.search(&f, 0);
			std::cout << q.results_per_file[0].summary() << "\n";
		}
	} else if (mode == "suffixload") {
		std::cout << "Loading the suffixtree and saving to file...\n";
		File f(arg);
		std::ofstream stream;
		stream.open(arg+".suffix");
		f.save(stream);
		//generate_dot(f.suffixtree, "original", 0);
		stream.close();
	} else {
		cout << "I don't understand " << mode << endl;
	}
	
}
