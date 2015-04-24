#include <iostream>
#include <string>
#include <list>
#include <fstream>

#include "RE-eNFA.h"
#include "product.h"
#include "TFA.h"
#include "FSM.h"
#include "tinyxml.h"
#include "eNFA-DFA.h"
#include "suffix.h"

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
		
		s_DFA opt_D = TFA(D);
		
		write_dot(&opt_D, arg + ".dot");
	} else if (mode == "eNFA-DFA") {
		s_eNFA N;
		auto doc = read(arg);
		N.from_xml(doc);
		
		s_DFA D = MSSC(N);
		
		write_dot(&D, arg+".dot");
	} else if (mode == "union" || mode == "intersection") {
		if (argc < 4) {
			cout << "Not enough arguments given for creating the product automaton.\n";
			return 1;
		}
		string arg2 = string(argv[3]);
		
		s_DFA D1;
		//DFA <std::string, char> D1;
		//DFA <std::string, char> D2;
		auto doc = read(arg);
		D1.from_xml(doc);
		
		s_DFA D2;
		doc = read(arg2);
		D2.from_xml(doc);

		DFA <std::string, char> P = product(D1, D2, mode == "intersection");  // true --> intersection
		write_dot(&P, "product_"+arg+"_"+arg2+".dot");
	} else if (mode == "suffix") {
		string arg2 = string(argv[3]);

		SuffixTree s(arg);

		ofstream output_file;
		output_file.open(arg+".gv");
		output_file << s;
		output_file.close();

		list<int> index = s.search_string(arg2);
		if (index.size() == 0) {
			cout << "No matches where found with string " << arg2 << " in text " << arg <<".\n";
		}
		for (std::list<int>::const_iterator i = index.begin(); i != index.end(); i++) {
			cout << "Match at index " << *i << endl;
		}
	} else {
		cout << "I don't understand " << mode << endl;
	}
	
}
