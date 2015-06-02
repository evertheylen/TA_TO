
#include <iostream>
#include <string>
#include <list>

#include <vector>
#include <map>

#include "FSM.h"
#include "Run.h"
#include "CompactDFA.h"

#include "./engine_r/eNFA-DFA/eNFA-DFA.h"
#include "./engine_r/Product/product.h"
#include "./engine_r/RE-eNFA/RE-eNFA.h"
#include "./engine_r/TFA/TFA.h"

#include "suffix.h"
#include "file.h"
#include "search.h"

#include "tinyxml.h"

TiXmlDocument _read(std::string name) {
	TiXmlDocument doc;
	doc.LoadFile(name);
	return doc;
}

template<typename T>
void setprint(std::set<T> s) {
	std::cout << "{";
	for (auto e: s) {
		std::cout << e << ", ";
	}
	std::cout << "}\n";
}

template <typename R, typename S, typename T>
void _write_dot(FSM<R,S,T>* F, std::string name) {
	std::ofstream output_file;
	output_file.open(name);
	F->to_dot(output_file);
	output_file.close();
}

int main(int argc, char* args[]) {
	
	/*
	if (argc != 3) {
		std::cout << "need two arguments\n";
	}
	
	std::string regex = args[2];
	
// 	auto E = RE_to_eNFA<std::string, char, 'e'>(regex);
// 	s_eNFA_Runner Erun(E);
// 	_write_dot(&E, "enfa.dot");
// 	
// 	std::cout << "Alphabet of E: ";
// 	for (char a: E.sigma) {
// 		std::cout << a << ", ";
// 	}
// 	std::cout << "\n";
// 	
// 	auto bad_D = MSSC(E);
// 	s_DFA_Runner Drun(bad_D);
// 	_write_dot(&bad_D, "dfa.dot");
// 	
// 	#define test(a) std::cout << "testing '" << a << "' : " << Erun.process(a) << " == " << Drun.process(a) << "\n"
// 	test("TAAT");
// 	test("TAT");
// 	test("TT");
// 	test("TAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAT");
// 	
// 	
	Query q(regex, 0, 0, 0, 0, 0);
	
	File f(std::string(args[1]), 0);
	q.search(&f);
	
	Result r = q.results_per_file[0];
	
	if (r.matches.size() == 0) {
		std::cout << "no matches :(\n";
	} else {
		for (Match& m: r.matches) {
			std::cout << m.format(f) << "\n";
		}
	}
	*/
	
	std::string arg(args[1]);
	if (arg.length() > 7 && arg.substr(arg.length()-6) == "suffix") {
		// Load file
		std::ifstream istream;
		istream.open(arg);
		File otherf(istream);
		//generate_dot(f.suffixtree, "loaded", 0);
		std::cout << otherf.comments << "\n";
		istream.close();
		
		for (Gap& g: otherf.gaps) {
			std::cout << "gap: " << g.position << ", " << g.length << "\n";
		}
	} else {
		File f(arg);
		std::ofstream stream;
		stream.open(arg+".suffix");
		f.save(stream);
		//generate_dot(f.suffixtree, "original", 0);
		stream.close();
		
		for (Gap& g: f.gaps) {
			std::cout << "gap: " << g.position << ", " << g.length << "\n";
		}
	}
	
}
