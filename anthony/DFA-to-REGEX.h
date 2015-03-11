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

class State{
private:
std::string name;
public:
void setName(std::string new_name);
std::string getName();
};

class Automaton{
private:
std::string name;
public:
Automaton();
std::vector<State*> start;
std::vector<State*> end;
void setName(std::string new_name);
std::string getName();
void readfromXML();
};


class Transition{
private:
State* _begin;
State* _end;
std::string _label;
public:
std::string getLabel();
std::vector<Transition*> transitions;
State* get_from();
State* get_to();
Transition(State* begin, std::string label, State* end);
};

#endif /* defined(____DFA_to_REGEX__) */
