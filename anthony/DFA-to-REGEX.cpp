/*
//Description: Source code for DFA to REGEX
//Author: Anthony Hermans
//Date: 11/03/15
*/
#include "DFA-to-REGEX.h"
#include "TinyXML/tinyxml.h"
#include <iostream>
#include <string>
using namespace std;

//The class Automaton
Automaton::Automaton(){cout << "Initializing automaton\n";}

void Automaton::setName(string new_name){name = new_name;}

string Automaton::getName(){return name;}

int Automaton::readXML(TiXmlDocument input_automaton){
	TiXmlElement* root = input_automaton.FirstChildElement();
	if (root == NULL){
	    cerr << "The file is not valid: there is no root element.\n";
	    input_automaton.Clear();
	    return 0;
	}
	this->setName(root->Attribute("name"));
	cout << "The name of this automaton: "<< this->getName() << endl;
	for (TiXmlElement* automaton= root->FirstChildElement(); automaton!= NULL ; automaton = automaton->NextSiblingElement()) {
		string elemName = automaton->Value();
		if (elemName == "ALPHABET"){continue;} //Niet nodig voor mijn implementatie??
		if (elemName == "STATES"){
			for (TiXmlElement* states = automaton->FirstChildElement(); states != NULL; 	states = states->NextSiblingElement()){
				string statename = states->Value();
				if (statename == "START"){
					for (TiXmlElement* startstates = states->FirstChildElement(); startstates != NULL; 	startstates = startstates->NextSiblingElement()){
						State* pointer;
						pointer = new State(startstates->Attribute("name"));
						this->start.push_back(pointer);
						this->total.push_back(pointer);
					}
				}
				if (statename == "END"){
					for (TiXmlElement* endstates = states->FirstChildElement(); endstates != NULL; 	endstates = endstates->NextSiblingElement()){
						State* pointer;
						pointer = new State(endstates->Attribute("name"));
						this->end.push_back(pointer);
						this->total.push_back(pointer);
					}

				}
				else{
				for (TiXmlElement* normalstates = states->FirstChildElement(); normalstates != NULL; 	normalstates = normalstates->NextSiblingElement()){
						State* pointer;
						pointer = new State(normalstates->Attribute("name"));
						this->total.push_back(pointer);
					}
				}
			}
			cout <<"Aantal eindstaten"<< endl;
			cout << this->end.size() << endl;
			cout <<"Aantal gewone staten"<< endl;
			cout << this->total.size() - this->end.size() - this->start.size()<< endl;
			cout <<"Aantal beginstaten" << endl;
			cout << this->start.size() << endl;

		}
		if (elemName == "TRANSITIONS"){
			for (TiXmlElement* transitions = automaton->FirstChildElement(); transitions != NULL; 	transitions = transitions->NextSiblingElement()){
				string from = transitions->Attribute("from");
				string label = transitions->Attribute("symbol");
				string to = transitions->Attribute("to");
				State* begin;
				State* end;
				for (unsigned int i = 0; i < this->total.size(); i++){
					if (from == this->total.at(i)->getName()){
						//cout << "found:" << from << "==" << this->total.at(i)->getName() << endl;
						begin = total.at(i);
					}
					if (to == this->total.at(i)->getName()){
						//cout << "found:" << to << "==" << this->total.at(i)->getName() << endl;
						end = total.at(i);
					}
				}
				Transition* pointer;
				pointer = new Transition(begin, label, end);
				this->transitions.push_back(pointer);

			}
		}
	}
	cout << "Het aantal transities: " << this->transitions.size() << endl;
	return 1;
}

string Automaton::StateEliminationTechnique(){

	return "This will be the regex";
	};


//The class State
void State::setName(string new_name){name = new_name;}

string State::getName(){return name;}

State::State(string new_name){name = new_name;}
State::State(){};

//The class Transition
std::string Transition::getLabel(){return _label;}

State* Transition::get_from(){return _begin;}

State* Transition::get_to(){return _end;}

Transition::Transition(State* begin, string label, State* end){
	_begin = begin;
	_label = label;
	_end = end;
}