/*
 * algorithm.h
 *
 *  Created on: 5 Mar 2015
 *      Author: stijn
 */
#include "lib/tinyxml.h"
#include <string>
#include <vector>

#ifndef ALGORITHM_H_
#define ALGORITHM_H_

class State {
private:
	std::string _name;
public:
	State(std::string name);
	State();
	//~State();
	std::string& get_name();
};

class Transition {
private:
	State* _a, *_b;
	std::string _label;
public:
	std::string& get_label();
	State* get_from();
	State* get_to();
	Transition(State* a, std::string label, State* b);
	Transition();
	//~Transition();
	friend std::ostream& operator<< (std::ostream &out, Transition& transition);
};

class Automaton {
private:
	std::string name;

public:
	bool product = false;
	std::vector<State*> start;
	std::vector<State*> end;
	std::vector<State*> total_states;
	std::vector<Transition*> transitions;
	std::vector<std::string> alphabet;

	Automaton(TiXmlDocument& doc);
	Automaton();
	~Automaton();

	std::string readAttribute(TiXmlElement* elem, const char* tag);
	std::string& get_name();
	void set_name(std::string _name);
	std::string readElement(TiXmlElement* elem, const char* tag);
	std::string readElement(TiXmlElement* elem);
	void parse_nodes(TiXmlElement* root);
	std::string get_first_name(std::string total);
	std::string get_second_name(std::string total);
	bool in_start(std::string name);
	bool in_end(std::string name);
	State* get_state(std::string name);
	Transition* get_trans(std::string name, std::string label);
	friend std::ostream& operator << (std::ostream& out, Automaton& automaton);
};
Automaton make_product(Automaton& auto1, Automaton& auto2, bool end_union);
Automaton Union(Automaton& auto1, Automaton& auto2);
Automaton Intersection(Automaton& auto1, Automaton& auto2);

#endif /* ALGORITHM_H_ */
