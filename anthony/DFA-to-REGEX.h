/*
 *Description: Header for DFA to REGEX
 *Author: Anthony Hermans
 *Date: 26/03/15
 */
#ifndef ____DFA_to_REGEX__
#define ____DFA_to_REGEX__

#include <stdio.h>
#include <string>
#include <vector>
#include "TinyXML/tinyxml.h"

//CLASS STATE
class State{
private:
std::string name;	//Name of the state
public:
//Getters
void setName(std::string new_name);
std::string getName();
//Constructors
State(std::string name);
State();
};

//CLASS TRANSITION
class Transition{
private:
State* _begin;		//Beginstate of the transition
State* _end;		//Endstate of the transition
std::string _label;	//Label of the transition
public:
//Getters and setters
std::string getLabel();
State* get_from();
State* get_to();
void setLabel(std::string newlabel);
//Constructor and destructor
Transition(State* begin, std::string label, State* end);
~Transition();
};

//CLASS AUTOMATON
class Automaton{
private:
std::string name;						//Name of the automaton
public:
//Constructor
Automaton();
//Public attributes
std::vector<State*> start;				//Startstates of the automaton
std::vector<State*> end;				//Endstates of the automaton
std::vector<State*> total;				//Total of states of the automaton
std::vector<Transition*> transitions;	//Total of transition of the automaton
//Getters and setters
void setName(std::string new_name);
std::string getName();
//Read from XML and put information in automaton
int readXML(TiXmlDocument input_automaton);
//The conversion of the DFA to the regex expression
std::string StateEliminationTechnique();
};


#endif /* defined(____DFA_to_REGEX__) */
