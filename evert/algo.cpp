
#include "FSM.tcc" // guess what, templates
#include "FSM.h"

#include "conv.tcc"
#include "conv.h"


#include <set>
#include <string>
#include <iostream>
#include <fstream>
#include "tinyxml.h"

using namespace std;

int main(int argc, char const* argv[]) {
	if (argc < 2) {
		return -1;
	}
	
	str_eNFA N("eps");
	
	TiXmlDocument doc;
	
	doc.LoadFile(argv[1]);
	
	N.from_xml(doc);
	
	std::ofstream output_file;
	output_file.open(std::string(argv[1]) + ".eNFA.dot");
	N.to_dot(output_file);
	output_file.close();
	
	auto D = MSSC<str_eNFA, str_DFA>(N);
	
	output_file.open(std::string(argv[1]) + ".DFA.dot");
	D.to_dot(output_file);
	output_file.close();
}


int altmain() {
	cout << "--- DFA: ---\n";
	
	str_DFA D(
		{"q0", "q1", "q2"},
		{"1", "0"},
		str_DFA::Delta(),
		"q0",
		{"q2"}
	);
	
	D.d["q0"]["1"] = "q2";
	
	D.to_dot(cout);
	
	Run<str_DFA> r(D);
	
	cout << r.walker.current << endl;
	
	r.process({"1"});
	cout << "after input 1:\n";
	
	cout << r.walker.current << endl;
	
	
	
	cout << "\n--- NFA: ---\n";
	
	str_NFA N(
		{"q0", "q1", "q2", "q3", "q4"},
		{"1", "0"},
		str_NFA::Delta(),
		"q0",
		{"q2"}
	);
	
	N.d["q0"]["1"] = {"q1", "q2"};
	N.d["q1"]["0"] = {"q3", "q4"};
	
	#define print_current(r)\
		for (auto s: r.walker.current) {\
			cout << s << endl;\
		}
	
	N.to_dot(cout);
	
	Run<str_NFA> rn(N);
	
	print_current(rn);
	
	rn.process({"1"});
	cout << "after input 1:\n";
	
	print_current(rn);
	
	rn.process({"0"});
	cout << "after input 0:\n";
	
	print_current(rn);
	
	
	
	cout << "\n--- eNFA: ---\n";
	
	str_eNFA M(
		{"q0", "q1", "q2", "q3", "q4"},
		{"1", "0"},
		str_eNFA::Delta(),
		"q0",
		{"q2"},
		"eps"
	);
	
	M.d["q0"]["eps"] = {"q4", "q5"};
	M.d["q0"]["1"] = {"q1", "q2"};
	M.d["q1"]["0"] = {"q3", "q4"};
	
	//M.state_to_dot(cout, "q0", "eps", M.d["q0"]["eps"]);
	M.to_dot(cout);
	
	Run<str_eNFA> rm(M);
	
	print_current(rm);
	
	rm.process({"1"});
	cout << "after input 1:\n";
	
	print_current(rm);
	
	rm.process({"0"});
	cout << "after input 0:\n";
	
	print_current(rm);
	
	
}
//*/
