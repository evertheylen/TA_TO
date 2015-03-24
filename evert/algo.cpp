
#include "FSM.cpp" // guess what, templates
#include <set>
#include <string>
#include <iostream>

using namespace std;

int main() {
	cout << "--- DFA: ---\n";
	
	str_DFA D(
		{"q0", "q1", "q2"},
		{"1", "0"},
		str_DFA::Delta(),
		"q0",
		{"q2"}
	);
	
	D.d["q0"]["1"] = "q2";
	
	cout << D.q0 << endl;
	
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
	
	M.d["q0"]["eps"] = {"q4"};
	M.d["q0"]["1"] = {"q1", "q2"};
	M.d["q1"]["0"] = {"q3", "q4"};
	
	Run<str_eNFA> rm(M);
	
	print_current(rm);
	
	rm.process({"1"});
	cout << "after input 1:\n";
	
	print_current(rm);
	
	rm.process({"0"});
	cout << "after input 0:\n";
	
	print_current(rm);
	
}
