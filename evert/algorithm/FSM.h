#include "tinyxml/tinyxml.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>

#ifndef _FSM
#define _FSM

std::string readAttribute(TiXmlElement* elem, const char* tag);

std::string readElement(TiXmlElement* elem);


class State {
public:
	std::string name;

	State(std::string _name);

	State(TiXmlElement* el);
};

// erm, not used
class Transition {
public:
	State* from;
	std::string symbol;
	State* to;

	Transition(State* _from, std::string _symbol, State* _to);

	bool is_epsilon();
};

class Automaton {
public:
	std::string name;

	std::vector<std::string> alphabet;
	std::map<std::string, State*> states;  // search on name
	std::map<std::string, std::map<std::string, std::string>> transitions;  // transitions[from_name][symbol] = to_name
	State* start;
	std::vector<State*> accepting;

	/*
	Automaton(std::vector<std::string> _alphabet,
	          std::map<std::string, State> _states,
	          std::map<State*, std::map<std::string, State*>> _transitions,
	          State* _start,
	          std::vector<State*> _accepting):
		alphabet(_alphabet),
		states(_states),
		//transitions(_transitions),  // wtf?
		start(_start)
		//accepting(_accepting)   // wtf?
		{
		
		transitions = _transitions;
		accepting = _accepting;
	}
	*/

	Automaton(TiXmlDocument& doc);
	
	Automaton();
	
	~Automaton();
};

#endif
