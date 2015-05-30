#ifndef _H_TFA
#define _H_TFA

#include "FSM.h"
#include <vector>
#include <iostream>
using namespace std;

struct Table {
	int num_states;
	std::vector<bool> data;
	
	Table(int num);
	
	Table() = default;
	
	int loc(int k, int l);
	
	bool get(int k, int l);
	
	void set(int k, int l, bool val);
	
	void print(std::ostream& out, s_DFA& D);
	
	friend bool operator==(Table& lhs, Table& rhs);
	friend bool operator!=(Table& lhs, Table& rhs);
};

struct block {
    int state1;
    int state2;
    bool dist = false;
    block(int a, int b) : state1(a), state2(b) {}
    bool operator==(block const& b);
    bool hasFinal(s_DFA D);
};

s_DFA TFA(s_DFA D);

#endif
