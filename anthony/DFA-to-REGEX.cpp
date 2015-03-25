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
Automaton::Automaton(){/*cout << "Initializing automaton\n";*/}

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
	//cout << "The name of this automaton: "<< this->getName() << endl;
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
				if ( statename == "STATE"){
						State* pointer;
						pointer = new State(states->Attribute("name"));
						this->total.push_back(pointer);
				}
			}
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
	return 1;
}

string Automaton::StateEliminationTechnique(){
	vector<Transition*> new_trans;
	//Iterates over the vector of transitions and replaces the labels by regex
	for (unsigned int i =0; i< this->transitions.size(); i++){
		//This is a transition to itself
		bool solostate = true; //If the transition is solo("There are no more transtions to the same state)
		if (this->transitions.at(i)->get_from() == this->transitions.at(i)->get_to()){
			//cout << this->transitions.at(i)->get_from()->getName() << "=" << this->transitions.at(i)->get_to()->getName() <<"	" << this->transitions.at(i)->getLabel()<< " end\n";
			bool isPresent = false; //Helps to verify if there are more transitions to the same state
			if (new_trans.size() == 0){		//First item of vector
				new_trans.push_back(this->transitions.at(i));
				continue;
			}
			for (unsigned int j =0; j< new_trans.size(); j++){
				if (transitions.at(i)->get_from() == new_trans.at(j)->get_from() and transitions.at(i)->get_to() == new_trans.at(j)->get_to()){
					std::string newlabel;
					//cout << new_trans.at(j)->getLabel() << "		" << transitions.at(i)->getLabel() << endl;
					if (new_trans.at(j)->getLabel() == transitions.at(i)->getLabel()){
						newlabel = new_trans.at(j)->getLabel();
						new_trans.at(j)->setLabel(newlabel);
						isPresent =true;
						continue;
					}
					if (new_trans.at(j)->getLabel() != transitions.at(i)->getLabel()){
						newlabel = new_trans.at(j)->getLabel()+ "+" +transitions.at(i)->getLabel();
						new_trans.at(j)->setLabel(newlabel);
						isPresent = true;
						continue;
					}
				}
				if ((j == new_trans.size()-1 and isPresent == false) or (j == new_trans.size() and isPresent == false)){
					new_trans.push_back(this->transitions.at(i));
					continue;
				}
			}
			continue;
		}
		//Als hetgeen transitie naar zichzelf is
		for (unsigned int z=0; z < new_trans.size(); z++){
			if (this->transitions.at(i)->get_from() == new_trans.at(z)->get_from() and this->transitions.at(i)->get_to() == new_trans.at(z)->get_to()){
				solostate = false;
				string newlabel;
				if (this->transitions.at(i)->getLabel() == new_trans.at(z)->getLabel()){
					newlabel =new_trans.at(z)->getLabel();
					new_trans.at(z)->setLabel(newlabel);
					break;
				}
				if (this->transitions.at(i)->getLabel() != new_trans.at(z)->getLabel()){
					newlabel = new_trans.at(z)->getLabel() + "+" + this->transitions.at(i)->getLabel();
					new_trans.at(z)->setLabel(newlabel);
					break;
				}
				continue;
			}
		}
		if (solostate == true){//If its a transition with a single label and not in newtrans
			new_trans.push_back(this->transitions.at(i));
		}
	}
	//Scaffolding
	cout <<"\n" <<this->transitions.size()<< " == Het aantal transities before" << endl;
	this->transitions = new_trans;
	cout << this->transitions.size()<< " == Het aantal transities after" << endl;
	for (unsigned int i =0; i< this->transitions.size(); i++){
	cout << this->transitions.at(i)->get_from()->getName() << "==>" << this->transitions.at(i)->get_to()->getName() <<"	label:" << this->transitions.at(i)->getLabel()<< "\n";
	}
	
	return "REGEX EXPRESSION HERE";
	};


//The class State
void State::setName(string new_name){name = new_name;}

string State::getName(){return name;}

State::State(string new_name){name = new_name;}
State::State(){};

//The class Transition
std::string Transition::getLabel(){return _label;}

Transition::~Transition(){cout << "The transition is being deleted\n";}

void Transition::setLabel(std::string newlabel){_label = newlabel;};

State* Transition::get_from(){return _begin;}

State* Transition::get_to(){return _end;}

Transition::Transition(State* begin, string label, State* end){
	_begin = begin;
	_label = label;
	_end = end;
}