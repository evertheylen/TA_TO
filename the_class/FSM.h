#include "FSM.tcc"

#ifndef _FSM
#define _FSM

typedef FSM<std::string, char> s_DFA;
typedef NFA<std::string, char> s_NFA;
typedef eNFA<std::string, char, 'e'> s_eNFA;

#endif
