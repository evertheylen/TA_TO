#include "FSM.tcc"

#ifndef _FSM
#define _FSM

template <>
char XML_Cast<char>(std::string s);

typedef DFA<std::string, char> s_DFA;
typedef NFA<std::string, char> s_NFA;
typedef eNFA<std::string, char, 'e'> s_eNFA;

#endif
