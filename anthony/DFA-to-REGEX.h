/*
//Description: Header for DFA to REGEX
//Author: Anthony Hermans
//Date: 11/03/15
*/
#ifndef ____DFA_to_REGEX__
#define ____DFA_to_REGEX__

#include <stdio.h>
#include <string>
#include <vector>
#include "TinyXML/tinyxml.h"

class State{
private:
std::string name;
public:
void setName(std::string new_name);
std::string getName();
State(std::string name);
State();
};


class Transition{
private:
State* _begin;
State* _end;
std::string _label;
public:
std::string getLabel();
void setLabel(std::string newlabel);
State* get_from();
State* get_to();
Transition(State* begin, std::string label, State* end);
~Transition();
};

class Automaton{
private:
std::string name;
public:
Automaton();
std::vector<State*> start;
std::vector<State*> end;
std::vector<State*> total;
std::vector<Transition*> transitions;
void setName(std::string new_name);
std::string getName();
int readXML(TiXmlDocument input_automaton);
std::string StateEliminationTechnique();
};


#endif /* defined(____DFA_to_REGEX__) */
