/*
//Description: Source code for DFA to REGEX
//Author: Anthony Hermans
//Date: 11/03/15
*/
#include "DFA-to-REGEX.h"
#include <iostream>
#include <string>
using namespace std;

//The class Automaton
Automaton::Automaton(){cout << "Initializing automaton\n";}

void Automaton::setName(string new_name){name = new_name;}

string Automaton::getName(){return name;}

//The class State
void State::setName(string new_name){name = new_name;}

string State::getName(){return name;}

//The class Transition
std::string Transition::getLabel(){return _label;}

State* Transition::get_from(){return _begin;}

State* Transition::get_to(){return _end;}

Transition::Transition(State* begin, string label, State* end){
	_begin = begin;
	_label = label;
	_end = end;
}