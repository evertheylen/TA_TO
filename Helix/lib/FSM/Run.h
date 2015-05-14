
#ifndef _Run
#define _Run

#include "FSM.h"
#include "Run.tcc"

typedef Walker<s_DFA> s_DFA_Walker;
typedef Walker<s_NFA> s_NFA_Walker;
typedef Walker<s_eNFA> s_eNFA_Walker;

typedef Runner<s_DFA> s_DFA_Runner;
typedef Runner<s_NFA> s_NFA_Runner;
typedef Runner<s_eNFA> s_eNFA_Runner;


#endif
