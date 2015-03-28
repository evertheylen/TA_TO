/*
 *Description: Source code for DFA to REGEX
 *Author: Anthony Hermans
 *Date: 26/03/15
 */

#include "DFA-to-REGEX.h"
#include "TinyXML/tinyxml.h"
#include <iostream>
#include <string>
using namespace std;

//CLASS AUTOMATON
//Constructor
Automaton::Automaton(){/*cout << "Initializing automaton\n";*/}

//Getters and setters
void Automaton::setName(string new_name){name = new_name;}
string Automaton::getName(){return name;}

////This function read from an xml file and put the information in an automaton
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

//This function will transfer the DFA to a regex expression
string Automaton::StateEliminationTechnique(){
	//Temporary vector of transitions
	vector<Transition*> new_trans;
	//Iterates over the vector of transitions and replaces the labels by regex
	for (unsigned int i =0; i< this->transitions.size(); i++){
		//This is a transition to itself
		bool solostate = true; //If the transition is solo("There are no more transtions to the same state)
		if (this->transitions.at(i)->get_from() == this->transitions.at(i)->get_to()){
			bool isPresent = false; //Helps to verify if there are more transitions to the same state
			if (new_trans.size() == 0){		//First item of vector
				new_trans.push_back(this->transitions.at(i));
				continue;
			}
			//Check if transition is already in new_trans
			for (unsigned int j =0; j< new_trans.size(); j++){
				if (transitions.at(i)->get_from() == new_trans.at(j)->get_from() and transitions.at(i)->get_to() == new_trans.at(j)->get_to()){
					std::string newlabel;
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
		//If it isn't a transition to itself
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
		//If it isn't a transition to itself and isn't already in new_trans
		if (solostate == true){
			new_trans.push_back(this->transitions.at(i));
		}
	}
	//Scaffolding
	//cout <<"\n" <<this->transitions.size()<< " == Het aantal transities before" << endl;
	//this->transitions = new_trans;
	//cout << this->transitions.size()<< " == Het aantal transities after" << endl;
	//for (unsigned int i =0; i< this->transitions.size(); i++){
	//cout << this->transitions.at(i)->get_from()->getName() << "==>" << this->transitions.at(i)->get_to()->getName() <<"	label:" << this->transitions.at(i)->getLabel()<< "\n";
	//}
	
	//The state elimination itself and generating of the final regex expression
	this->transitions = new_trans;
	string ultimate_regex;//This will be the final regex expression
	for (unsigned int i=0; i < this->transitions.size(); i++){
		for (unsigned int j=0; j < this->end.size(); j++){
			if (this->transitions.at(i)->get_to() == this->end.at(j)){//If it's a transition to an end state
				unsigned int z=0;
				string new_regex;
				while (this->transitions.at(z)->get_from() != this->end.at(j)){
					if (this->transitions.at(z)->get_from() == this->transitions.at(z)->get_to()){//Transition to itself
						new_regex = new_regex + "(" + this->transitions.at(z)->getLabel() + ")*";
					}
					if (this->transitions.at(z)->get_from() != this->transitions.at(z)->get_to()){//Isn't a transition to the same state
						if (this->transitions.at(z)->getLabel().size() == 1){//If it's a label with only 1 character
							new_regex += this->transitions.at(z)->getLabel();
						}
						else{//If it's a label with more characters
							new_regex += "(" + this->transitions.at(z)->getLabel() + ")";
						}
					}
					z++;
					if (z != this->transitions.size()){
						continue;
					}
					break;
				}
				if (ultimate_regex != ""){//If ultimate regex is empty don't add a "+"
					ultimate_regex = ultimate_regex + "+" + new_regex;
				}
				else{
					ultimate_regex += new_regex;
				}
			}
			continue;
		}
	}
	return ultimate_regex;
	};


//CLASS STATE
//Getters and setters
void State::setName(string new_name){name = new_name;}
string State::getName(){return name;}

//Constructors
State::State(string new_name){name = new_name;}
State::State(){};

//CLASS TRANSITION
//Getters and setters
void Transition::setLabel(std::string newlabel){_label = newlabel;};
std::string Transition::getLabel(){return _label;}
State* Transition::get_from(){return _begin;}
State* Transition::get_to(){return _end;}

//Constructor and destructor
Transition::Transition(State* begin, string label, State* end){
	_begin = begin;
	_label = label;
	_end = end;
}
Transition::~Transition(){cout << "The transition is being deleted\n";}