/*
 * ukkonen_suffix.cpp
 *
 *  Created on: 12 May 2015
 *      Author: stijn
 */

#include "ukkonen_suffix.h"
#include <sstream>
#include <fstream>
#include <iostream>

Edge::Edge(int _a, int _b) {
	a = _a;
	b = _b;
}

Node2::Node2(int start, int end): e(start, end), firstchild(nullptr), suffix_link(nullptr) {}

void Node2::add_child(Node2* child) {
	if (firstchild == nullptr) {
		firstchild = child;
		children.push_back(child);
	} else {
		children.push_back(child);
	}
}

Node2::~Node2() {
	for (auto child: children) {
		delete child;
	}
}

SuffixTree2::SuffixTree2(std::fstream& file, std::string _filename): _file(file), filename(_filename){
	char input;
	_loc = 0;
	remainder = 0;
	std::stringstream comments;
	_root = new Node2(-2);
	active.node = _root;
	active.edge = '\0x';
	active.length = 0;
	active.p = nullptr;
	while (file.good()) {
		file.get(input);
		if (file.eof()) {
			break;
		}
		if (input != '>' && input != '\n') {
			remainder++;
			_loc++;
			add_suffix(input);

			/*if (_loc == 20) {
				resolve(_root);
				return;
			}*/
		} else if (input == '>'){
			while (input != '\n') {
				file.get(input);
				comments << input;
				_loc++;
			}
		} else {
			_loc++;
		}
	}
	remainder++;
	//add_suffix('$');
	//_loc++;
	resolve(_root);
}

SuffixTree2::~SuffixTree2() {
	delete _root;
}

void SuffixTree2::add_suffix(char tag) {
	static bool add_remainder = true;
	static Node2* suffix1;
	if (add_remainder) {
		remaining_suffix += tag;
	}
	add_remainder = true;
	std::cout << "The remaining suffix is " << remaining_suffix << std::endl;
	int present = check_presence(tag);
	std::ifstream file(filename.c_str());
	if (present == 1 && active.length < 1) {
		active.edge = tag;
		active.length++;
		return;
	} else if (present == 1) {
		active.length++;
		return;
	} else if (present == 2) {
		return;
	}
	if (active.node == _root && active.length == 0 && remainder == 1) {
		Node2* n = new Node2(_loc);
		_root->add_child(n);
		remainder--;
		remaining_suffix.erase(0,1);
		active.edge = remaining_suffix[0];
	} else {
		for (auto child: active.node->children) {
			if (child->e.a == active.p->a && child->e.b == active.p->b) {
				Node2* n1 = new Node2(child->e.a+active.length);
				bool add = true;
				for (auto child2: child->children) {
					if (get_tag(child2) == get_tag(n1)) {
						add = false;
					}
				}
				if (add) {
					child->add_child(n1);
				}
				Node2* n2;
				/*if (child->e.b != -1) {
					n2 = new Node2(child->e.b+active.length);
				} else {*/
					n2 = new Node2(_loc);
				//}
				child->add_child(n2);
				std::cout << "Splitting " << get_tag(child) << " into root: ";
				if (child->e.b > child->e.a+active.length) {
					std::cout << "WEIRD SHIT WATCH OUT!!!!\n" << _loc-(child->e.b-child->e.a - active.length) << std::endl;
					delete n2;
					n2 = new Node2(_loc-(child->e.b-child->e.a - active.length));
					child->e.b = child->e.b -(child->e.b-child->e.a-active.length);
				} else {
					child->e.b = child->e.a+active.length;
				}
				std::cout << get_tag(child) << " and children " << get_tag(n1) << " and " << get_tag(n2) << std::endl;
				remainder--;
				//std::cout << "[] The remaining suffix is " << remaining_suffix << std::endl;
				remaining_suffix.erase(0,1);
				if (suffix1 != nullptr) {
					suffix1->suffix_link = child;
					std::cout << "Setted up a suffix link between " << suffix1 << " and " << child << std::endl;
				}
				suffix1 = child;
				if (active.node == _root) {
					active.length--;
					active.edge = remaining_suffix[0];

				} else {
					if (active.node->suffix_link != nullptr) {
						std::cout << "Following the suffix link from " << get_tag(active.node);
						active.node = active.node->suffix_link;
						std::cout << " to " << get_tag(active.node) << std::endl;
						std::cout << "New active edge: " << active.edge << " and length: " << active.length << std::endl;
					} else {
						active.node = _root;
					}
				}
				for (auto child2: active.node->children) {
					if (get_tag(child2)[0] == active.edge) {
						active.p = &child2->e;
						break;
					}
				}
				add_remainder = false;

				break;
			}
		}
	} if (remainder != 0) {

		/*for (auto child: active.node->children) {
			file.seekg(child->e.a+active.length);
			char * buffer = new char [1];
			file.read(buffer, 1);
			std::string tagstr;
			tagstr += buffer[0];
			//std::cout << "In the file i found  " << tagstr << std::endl;
			delete[] buffer;
			if (tagstr[0] == active.edge) {
				active.p = &child->e;
				break;
				//std::cout << tag << " is already in " << child->e.a+1 << std::endl;
			}
		}*/
		//tag = remaining_suffix[0];
		add_suffix(tag);
	}
	suffix1 = nullptr;
	file.close();
}

int SuffixTree2::check_presence(char tag) {
	std::ifstream file(filename.c_str());
	for (auto child: active.node->children) {
		std::cout << "Checking: " << get_tag(child) << std::endl;
		if (active.length == 0) {
			file.seekg(child->e.a/*+active.length*/);
			char * buffer = new char [1];
			file.read(buffer, 1);
			std::string tagstr;
			tagstr += buffer[0];
			delete[] buffer;
			if (tagstr[0] == tag) {
				active.p = &child->e;
				int end = child->e.b;
				if (end == -1) {
					end = _loc;
				}
				//std::cout << "End: " << end-1 << " start+length: " << child->e.a+active.length+1 << std::endl;
				if (child->e.a+active.length == end) {
					//std::cout << tag << " is at the end of the edge!\n";
					active.node = child;
					active.edge = '\0x';
					active.length = 0;
					return 2;
				}
				//std::cout << tag << " is already in " << child->e.a+1 << std::endl;
				file.close();
				return 1;
			}
		} else {
			file.seekg(child->e.a/*+active.length*/);
			char * buffer = new char [1];
			file.read(buffer, 1);
			std::string tagstr;
			tagstr += buffer[0];
			delete[] buffer;
			if (tagstr[0] == active.edge) {
				/*if (active.length > get_tag(child).length()) {
					active.node = child;
					active.length -= get_tag(child).length();
					active.p = &child->e;
					return check_presence(tag);
				}*/
				file.seekg(child->e.a+active.length);
				char * buffer = new char [1];
				file.read(buffer, 1);
				std::string tagstr2;
				tagstr2 += buffer[0];
				delete[] buffer;
				int end = child->e.b;
				if (end == -1) {
					end = _loc;
				}
				/*file.seekg(child->e.a);
				file.read(buffer, end - child->e.a);
				buffer = new char [end - child->e.a];
				std::string tagstr3;
				for (int i = 0; i < end - child->e.a; i++) {
					tagstr3 += buffer[i];
				}*/
				bool ending = true;
				for (int i = 0; i < get_tag(child).length(); i++) {
					if (get_tag(child)[i] != remaining_suffix[i]) {
						ending = false;
						break;
					}
				}
				//delete[] buffer;
				if (ending) {
					//std::cout << tag << " is at the end of the edge!\n";
					active.node = child;
					active.edge = '\0x';
					active.length = 0;
					return 2;
				}
				if (tagstr2[0] == tag) {
					active.p = &child->e;
					int end = child->e.b;
					if (end == -1) {
						end = _loc;
					}
					//std::cout << "End: " << end-1 << " start+length: " << child->e.a+active.length+1 << std::endl;
					/*if (child->e.a+active.length+1 == end) {
						//std::cout << tag << " is at the end of the edge!\n";
						active.node = child;
						active.edge = '\0x';
						active.length = 0;
						return 2;
					}*/
					//std::cout << tag << " is already in " << child->e.a+1 << std::endl;
					file.close();
					return 1;
				}
				file.close();
				return 0;
			}
		}
	}
	file.close();
	return 0;
}

void SuffixTree2::resolve(Node2* current_node) {
	if (current_node->firstchild != nullptr) {
		for (auto child: current_node->children) {
			resolve(child);
		}
	}
	//std::cout << "Edge: From " << current_node->e.a << " to ";
	if (current_node->e.b == -1) {
		current_node->e.b = _loc;
	}
	//std::cout << current_node->e.b << std::endl;
	if (current_node->e.a != -2) {
		std::ifstream file(filename.c_str());
		int length = current_node->e.b - current_node->e.a;
		char * buffer = new char [length];
		file.seekg(current_node->e.a);
		file.read(buffer, length);
		std::string tag;
		for (int i = 0; i < length; i++) {
			tag += buffer[i];
		}
		//std::cout << length << std::endl;
		delete[] buffer;
		current_node->tag = tag;
		file.close();
	} else {
		current_node->tag = "root";
	}
	//std::cout << current_node->tag << std::endl;
}

std::ostream& operator<< (std::ostream& out, SuffixTree2& tree) {
	out << "digraph suffix {\n" << "\tnode [shape = circle];\n";
	out << *tree._root;
	out << '}';
	return out;
}
std::ostream& operator<<(std::ostream& out, Node2& n) {
	//if (n.suffix_link != nullptr) {
		//std::string tag = n.suffix_link->tag;
		//out << n.tag << " has a suffix link to " << n.suffix_link->tag << std::endl;
	//}
	int static counter = 0;

	if (n.firstchild != nullptr) {
		//for (auto child: n.children) {
	//		out << n.tag << " points to ";
			//out << *child;
		//}
		out << '\t' << counter << " [label= " << '"' << n.tag << '"' << "];" << "\n";
		int parent_counter = counter;
		for (auto child: n.children) {
			counter++;
			out << '\t' << counter << " [label= " << '"' << child->tag << '"' << "];\n";
			out << '\t' << parent_counter << " -> " << counter << ";\n";
			out << *child;
		}
	} else {
	//	out << n.tag << " end of branch!" << std::endl;
		return out;
	}
	return out;
}
std::string SuffixTree2::get_tag(Node2* n) {
	std::ifstream file(filename.c_str());
	file.seekg(n->e.a);
	int end = n->e.b;
	if (end == -1) {
		end = _loc;
	}
	int length = end- n->e.a;
	if (length < 1) {
		length = 1;
	}
	char * buffer = new char [length];
	file.read(buffer, length);
	std::string tag;
	for (int i = 0 ; i < length; i++) {
		tag += buffer[i];
	}
	delete[] buffer;

	file.close();
	return tag;
}
