#include "tinyxml/tinyxml.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "FSM.h"

std::string readAttribute(TiXmlElement* elem, const char* tag) {
	return std::string(elem->Attribute(tag));
}

std::string readElement(TiXmlElement* elem) {
	if (elem == NULL) {
		std::cerr << "Error: Tag not defined.\n";
	}
	TiXmlNode* node = elem->FirstChild();
	TiXmlText* text = node->ToText();
	return text->Value();
}

//class State {
// 	std::string name;

	State::State(std::string _name):
		name(_name) {
	}

	State::State(TiXmlElement* el) {
		name = readAttribute(el, "name");
	}
//};

// erm, not used
//class Transition {
// 	State* from;
// 	std::string symbol;
// 	State* to;

	Transition::Transition(State* _from, std::string _symbol, State* _to):
		from(_from),
		symbol(_symbol),
		to(_to) {
	}

	bool Transition::is_epsilon() {
		return symbol == "eps";  // change if needed
	}
//};

//class Automaton {
// 	std::string name;
// 
// 	std::vector<std::string> alphabet;
// 	std::map<std::string, State*> states;  // search on name
// 	std::map<State*, std::map<std::string, State*>> transitions;
// 	State* start;
// 	std::vector<State*> accepting;

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

	Automaton::Automaton(TiXmlDocument& doc) {
		TiXmlElement* root = doc.RootElement();
		name = readAttribute(root, "name");

		TiXmlElement* current_el = root->FirstChildElement();
		TiXmlElement* child;
		TiXmlElement* child2;
		State* state;
		
		std::map<std::string, std::map<std::string, std::vector<std::string>>> temp_transitions;

		while (current_el != NULL) {
			if (current_el->ValueTStr() == "ALPHABET") {
				child = current_el->FirstChildElement();
				while (child != NULL) {
					alphabet.push_back(readElement(child));
					//std::cout << readElement(child) << std::endl;
					child = child->NextSiblingElement();
				}
			} else if (current_el->ValueTStr() == "STATES") {
				child = current_el->FirstChildElement();
				while (child != NULL) {
					if (child->ValueTStr() == "START") {
						child2 = child->FirstChildElement();
						state = new State(child2);
						states[readAttribute(child2, "name")] = state;
						start = state;
					} else if (child->ValueTStr() == "END") {
						child2 = child->FirstChildElement();
						while (child2 != NULL) {
							state = new State(child2);
							states[readAttribute(child2, "name")] = state;
							accepting.push_back(state);
							
							child2 = child2->NextSiblingElement();
						}
					} else {
						if (child->ValueTStr() == "STATE") {
							state = new State(child);
							std::string name = readAttribute(child, "name");
// 							std::cout << "adding state with name=" << name << ", pointing to " << state << std::endl;
							states[name] = state;
						}
					}
					
					child = child->NextSiblingElement();
				}
			} else if (current_el->ValueTStr() == "TRANSITIONS") {
				child = current_el->FirstChildElement();
				while (child != NULL && child->ValueTStr() == "TRANSITION") {
					std::string from_name = readAttribute(child, "from");
					std::string to_name = readAttribute(child, "to");
					std::string symbol = readAttribute(child, "symbol");
					
					std::cout << "adding transition from " << from_name << " with symbol " << symbol << " to " << to_name << "\n";
					
// 					State* from = states[from_name];
// 					State* to = states[to_name];
					
					//std::cout << "adding transition from " << from->name << " with symbol " << symbol << " to " << to->name << "\n";
					
					temp_transitions[from_name][symbol].push_back(to_name);
					
					child = child->NextSiblingElement();
				}
			} else {
				std::cerr << "Error: Tag " << current_el->ValueTStr().c_str() <<  " not defined.\n";
			}
			
			current_el = current_el->NextSiblingElement();
		}
				
		// Convert temp_transitions to proper transitions
		for (auto i = temp_transitions.begin(); i != temp_transitions.end(); ++i) {			
			State* from = states[i->first];
			if (from == nullptr) {
				std::cout << "Couldn't find valid state with name " << i->first << ", skipping...\n";
				continue;
			}
			
			for (auto j = i->second.begin(); j != i->second.end(); ++j) {
				std::string symbol = j->first;
				
				//for (auto k = j->second.begin(); k != j->second.end(); ++k) {
				for (int k=0; k<j->second.size(); ++k) {
					std::string to_name = j->second.at(k);
					
					//State* to = states[to_name];
					transitions[i->first][symbol] = to_name;
					std::cout << "adding transition from " << from->name << " with symbol " << symbol << " to " << to_name << "\n"; 
				}
			}
		}
		
// 		std::cout << "the end?\n";
	}
	
	Automaton::Automaton() {}
	
	Automaton::~Automaton() {
		// delete all the states
		for(auto iterator = states.begin(); iterator != states.end(); ++iterator) {
			delete iterator->second;
		}
	}
//};
