#ifndef _H_TFA
#define _H_TFA

#include "FSM.h"
#include <vector>
#include <iostream>
using namespace std;

struct block{
    int state1;
    int state2;
    bool dist = false;
    block(int a, int b) : state1(a), state2(b) {}
    bool operator==(block const& b);
    bool hasFinal(s_DFA D);
};

s_DFA TFA(s_DFA D);

#endif