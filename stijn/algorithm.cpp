/*
 * algorithm.cpp
 *
 *  Created on: 5 Mar 2015
 *      Author: stijn
 */

#include "lib/tinyxml.h"
#include "algorithm.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "assert.h"

Automaton::Automaton() {}

Automaton::Automaton(TiXmlDocument& doc){
	TiXmlElement* root = doc.RootElement();
	name = readAttribute(root, "name");
	TiXmlElement* current_el = root->FirstChildElement();
	TiXmlElement* child, *child2;
	State* p = NULL;
	while (current_el != NULL) {
		if (current_el->ValueTStr() == "ALPHABET") {
			child = current_el->FirstChildElement();
			while (child != NULL) {
				alphabet.push_back(readElement(child));
				//std::cout << readElement(child) << std::endl;
				child = child->NextSiblingElement();
			}
		} else {
			if (current_el->ValueTStr() == "STATES") {
				child = current_el->FirstChildElement();
				while (child != NULL) {
					if (child->ValueTStr() == "START") {
						child2 = child->FirstChildElement();
						while (child2 != NULL) {
							std::string name = readAttribute(child2, "name");
							/*if (p != NULL) {
								std::cout << "Deleting: " << p->get_name() << "\n";
								delete p;
							}*/
							p = new State(name);
							//*p = State(name);
							//State s (name);
							//State* p = &s;
							start.push_back(p);
							total_states.push_back(p);
							//std::cout << p->get_name() << std::endl;

							child2 = child2->NextSiblingElement();

						}
					} else {
						if (child->ValueTStr() == "END") {
							child2 = child->FirstChildElement();
							while (child2 != NULL) {
								std::string name = readAttribute(child2, "name");
								/*if (p != NULL) {
									std::cout << "Deleting: " << p->get_name() << "\n";
									delete p;
								}*/
								p = new State(name);
								//*p = State(name);
								//State s (name);
								//State* p = &s;
								end.push_back(p);
								total_states.push_back(p);
								//std::cout << p->get_name() << std::endl;
								child2 = child2->NextSiblingElement();
							}
						} else {
							if (child->ValueTStr() == "STATE") {
								std::string name = readAttribute(child, "name");
								/*if (p != NULL) {
									std::cout << "Deleting: " << p->get_name() << "\n";
									delete p;
								}*/
								p = new State(name);
								//*p = State(name);
								//State s (name);
								//State* p = &s;
								total_states.push_back(p);
								//std::cout << p->get_name() << std::endl;
							}
						}
					}
					child = child->NextSiblingElement();
				}
			}
			else {
				if (current_el->ValueTStr() == "TRANSITIONS") {
					child = current_el->FirstChildElement();
					while (child != NULL) {
						//std::cout << readElement(child) << std::endl;
						std::string name_from = readAttribute(child, "from");
						std::string name_to = readAttribute(child, "to");
						std::string label = readAttribute(child, "symbol");
						//std::cout << "from: " << name_from << std::endl;
						//std::cout << "to: " << name_to << std::endl;
						State *p_from, *p_to;
						bool from = false;
						bool to = false;
						//std::cout << total_states.size() << std::endl;
						//for (std::vector<State*>::iterator i = total_states.begin(); i != total_states.end(); i++) {
						for (unsigned int i = 0; i != total_states.size(); ++i) {
							State* p = total_states.at(i);
							//std::cout << "current: " << p->get_name() << std::endl;
							if (!from && p->get_name() == name_from) {
								//std::cout << p->get_name() << "\n";
								//p_from = new State;
								//*p_from = *p;
								p_from = p;
								from = true;
								//std::cout << p_from->get_name() << "\n";
							}
							if (!to && p->get_name() == name_to) {
								//p_to = new State;
								//*p_to = *p;
								p_to = p;
								//std::cout << p_to->get_name();

								to = true;
							}
							continue;
						}
						//std::cout << p_from->get_name() << std::endl;
						//std::cout << p_to->get_name() << std::endl;
						//std::cout << "label: " << t.get_label() << std::endl;
						//std::cout << "from: " << t.get_from()->get_name() << std::endl;
						//std::cout << "to: " << t.get_to()->get_name() << std::endl;

						//Transition t (p_from, label, p_to);
						Transition* p = new Transition(p_from, label, p_to);
						//*p = t;
						//std::cout << *p << std::endl;
						transitions.push_back(p);
						child = child->NextSiblingElement();
					}
				} else {
					std::cerr << "Error: Tag " << current_el->ValueTStr().c_str() <<  " not defined.\n";
				}
			}
		}
		current_el = current_el->NextSiblingElement();
	}
}
std::string& Automaton::get_name() {
	return name;
}

Transition::Transition(State* a, std::string label, State* b) {
	_a = a;
	_b = b;
	_label = label;
}


State::State(std::string name){
	_name = name;
}
State::State() {
	_name = "noname";
}

std::string& State::get_name() {
	return _name;
}

std::string Automaton::readAttribute(TiXmlElement* elem, const char* tag) {
	return std::string(elem->Attribute(tag));
}

Transition::Transition() {
	_a = nullptr;
	_b = nullptr;
	_label = "noname";
}

std::string Automaton::readElement(TiXmlElement* elem) {
	if (elem == NULL) {
		std::cerr << "Error: Tag not defined.\n";
	}
	TiXmlNode* node = elem->FirstChild();
	TiXmlText* text = node->ToText();
	return text->Value();
}
std::ostream& operator<< (std::ostream &out, Transition& transition) {
	out << "   " << transition.get_label() << "\n" << transition.get_from()->get_name() << " --> " << transition.get_to()->get_name() << std::endl;
	return out;
}

std::ostream& operator << (std::ostream& out, Automaton& automaton) {
	//std::cout << "HAAAI\n";
	Transition* t = NULL;
	if (!automaton.product) {
	out << "digraph " << automaton.get_name() << " {\n\tnode [shape=none]; start;\n"
			<< "\tnode [shape=doublecircle];";
	for (unsigned int i = 0; i != automaton.end.size(); i++){
		out << " " << automaton.end.at(i)->get_name();
	}
	if (!automaton.end.empty()) {
		out << ";\n";
	} else {
		out << "\n";
	}
	out	<< "\tnode [shape=circle];\n";
	for (unsigned int i = 0; i != automaton.start.size(); i++) {
		out << "\tstart -> " << automaton.start.at(i)->get_name() << ";\n";
	}
	for (unsigned int i = 0; i != automaton.transitions.size(); i++) {
		//t = new Transition;
		t = automaton.transitions.at(i);
		//*t = *automaton.transitions.at(i);
		out << "\t" << t->get_from()->get_name() << " -> " << t->get_to()->get_name() << " [label=" << t->get_label() << "];\n";
		//std::cout << "eeeuy\n";
	}
	out << "}\n";
	} else {
		out << "digraph " << automaton.get_name() << " {\n\tnode [shape=none]; start;\n"
				<< "\tnode [shape=doublecircle];";
		for (unsigned int i = 0; i != automaton.end.size(); i++){
			out << " \"" << automaton.end.at(i)->get_name() << "\"";
		}
		if (!automaton.end.empty()) {
			out << ";\n";
		} else {
			out << "\n";
		}
		out	<< "\tnode [shape=circle];\n";
		for (unsigned int i = 0; i != automaton.start.size(); i++) {
			out << "\tstart -> \"" << automaton.start.at(i)->get_name() << "\";\n";
		}
		for (unsigned int i = 0; i != automaton.transitions.size(); i++) {
			//t = new Transition;
			t = automaton.transitions.at(i);
			//*t = *automaton.transitions.at(i);
			out << "\t\"" << t->get_from()->get_name() << "\" -> \"" << t->get_to()->get_name() << "\" [label=" << t->get_label() << "];\n";
			//std::cout << "eeeuy\n";
		}
		out << "}\n";
	}

	return out;
}

bool Automaton::in_start(std::string name) {
	for (unsigned int i = 0; i != start.size(); i++) {
		if (start.at(i)->get_name() == name) {
			return true;
		}
	}
	return false;
}

bool Automaton::in_end(std::string name) {
	for (unsigned int i = 0; i != end.size(); i++) {
		if (end.at(i)->get_name() == name) {
			return true;
		}
	}
	return false;
}
void Automaton::set_name(std::string _name) {
	name = _name;
}

Automaton Union(Automaton& auto1, Automaton& auto2) {
	return make_product(auto1, auto2, true);
}

Automaton Intersection(Automaton& auto1, Automaton& auto2) {
	return make_product(auto1, auto2, false);
}

Automaton make_product(Automaton& auto1, Automaton& auto2, bool end_union) {
	assert(auto1.alphabet.size() == auto2.alphabet.size()); // Make sure the alphabets of the 2 automata are the same,
															// the first step is that they have the same size.
	Automaton product;
	product.set_name("Temporary Product Automaton");
	product.product = true;

	/*
	 * For the product automaton I use stringstreams to create the names of the states. I create the combination of both names.
	 */

	// Create the alphabet of the product automaton.
	for (unsigned int i = 0; i != auto1.alphabet.size(); i++) {
		product.alphabet.push_back(auto1.alphabet.at(i));
	}
	bool already_in_alphabet = false;
	for (unsigned int i = 0; i != auto2.alphabet.size(); i++) {
		for (unsigned int j = 0; j != auto1.alphabet.size(); j++) {
			if (auto2.alphabet.at(i) == auto1.alphabet.at(j)) {
				already_in_alphabet = true;
			}
		}
		if (!already_in_alphabet) {
			std::cerr << "Alphabets aren't equal!\n";
			assert("alphabets must be equal");
			return product;
		}
		already_in_alphabet = false;
	}

	// Create the states of the product automaton.
	std::stringstream startname;
	std::string name;
	State* p = NULL;
	for (unsigned int i = 0; i != auto1.total_states.size(); i++) {
		for (unsigned int j = 0; j != auto2.total_states.size(); j++) {
			startname << auto1.total_states.at(i)->get_name() << "/" << auto2.total_states.at(j)->get_name();
			name = startname.str();
			startname.str("");	// Clears the stringstream
			p = new State(name);
			product.total_states.push_back(p);
			if (auto1.in_start(auto1.total_states.at(i)->get_name()) && auto2.in_start(auto2.total_states.at(j)->get_name())) {
				product.start.push_back(p);
			}
			if (!end_union) {
				if (auto1.in_end(auto1.total_states.at(i)->get_name()) && auto2.in_end(auto2.total_states.at(j)->get_name())) {
					product.end.push_back(p);
				}
			} else {
				if (auto1.in_end(auto1.total_states.at(i)->get_name()) || auto2.in_end(auto2.total_states.at(j)->get_name())) {
					product.end.push_back(p);
				}
			}

		}
	}

	// Construct the transitions of the product automaton via the Cartesian Product Construction.

	for (unsigned int i = 0; i != product.total_states.size(); i++) {
		State* p1_from = auto1.get_state(product.get_first_name(product.total_states.at(i)->get_name()));
		State* p2_from = auto2.get_state(product.get_second_name(product.total_states.at(i)->get_name()));
		std::stringstream name_to;
		std::string name;
		for (unsigned int j = 0; j != product.alphabet.size(); j++) {
			Transition* t1 = auto1.get_trans(p1_from->get_name(), product.alphabet.at(j));
			Transition* t2 = auto2.get_trans(p2_from->get_name(), product.alphabet.at(j));
			State* p1_to = t1->get_to();
			State* p2_to = t2->get_to();
			name_to << p1_to->get_name() << "/" << p2_to->get_name();
			name = name_to.str();
			name_to.str("");
			Transition* t = new Transition(product.total_states.at(i), product.alphabet.at(j), product.get_state(name));
			product.transitions.push_back(t);
		}
	}
	return product;
}

Transition* Automaton::get_trans(std::string name, std::string label) {
	for (unsigned int i = 0; i != transitions.size(); i++) {
		if (transitions.at(i)->get_from()->get_name() == name && transitions.at(i)->get_label() == label) {
			return transitions.at(i);
		}
	}
	std::cerr << "No transition from " << name << " with label " << label << "!\n";
}

State* Automaton::get_state(std::string name) {
	for (unsigned int i = 0; i != total_states.size(); i++) {
		if (total_states.at(i)->get_name() == name) {
			return total_states.at(i);
		}
	}
	std::cerr << "State " << name << " not in automaton!\n";
}

std::string Automaton::get_first_name(std::string total) {
	std::stringstream name;
	int position = 0;
	char cur = total[position];
	while (cur != '/') {
		//std::cout << cur << "\n";
		name << cur;
		position ++;
		cur = total[position];
	}
	std::string result = name.str();
	name.str("");
	//std::cout << result << "\n";
	return result;
}

std::string Automaton::get_second_name(std::string total) {
	std::stringstream name;
	int position = 0;
	char cur = total[position];
	bool slash = false;
	while (cur != '\0') {
		//std::cout << cur << "\n";

		if (slash) {
			name << cur;
		}
		if (cur == '/') {
			slash = true;
		}
		position ++;
		cur = total[position];
	}
	std::string result = name.str();
	name.str("");
	//std::cout << result << "\n";
	return result;
}

/*Transition::~Transition() {
	//delete _a;
	//delete _b;
	std::cout << "Destructor called for Transition.\n";
}*/
/*State::~State(){
	std::cout << _name << "\n";
	std::cout << "Destructor called for State.\n";
}*/

Automaton::~Automaton() {
	for (unsigned int i = 0; i != total_states.size(); i++) {
		//std::cout << total_states.at(i)->get_name() << std::endl;
		delete(total_states.at(i));
		total_states.at(i) = NULL;
		//std::cout << total_states.at(i)->get_name() << std::endl;
	}
	for (unsigned int i = 0; i != transitions.size(); i++) {
		delete transitions.at(i);
	}
	//std::cout << "Destructor called for automaton " << name << "!\n";
}



std::string& Transition::get_label() {
	return _label;
}
State* Transition::get_from() {
	return _a;
}
State* Transition::get_to() {
	return _b;
}
