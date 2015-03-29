
#ifndef _FSM
#define _FSM

#include "FSM.tcc"

template <>
char XML_Cast<char>(std::string s);

typedef DFA<std::string, char> s_DFA;
typedef NFA<std::string, char> s_NFA;
typedef eNFA<std::string, char, 'e'> s_eNFA;

#endif
