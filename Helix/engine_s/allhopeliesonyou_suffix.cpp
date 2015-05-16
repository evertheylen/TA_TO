
#include <iostream>
#include <fstream>
#include <string>

#include "allhopeliesonyou_suffix.h"

// Lelijk
int_and_node::int_and_node(int _pos, Node3* _node):
		pos(_pos), node(_node) {}


void generate_dot(Suffix3& s, std::string name, int i) {
	std::cout << "called!!!!!!!!!!!!!\n";
	name += "_";
	name += std::to_string(i);
	name += ".dot";
	std::ofstream f(name);
	std::cout << name << "\n";
	f << s;
	f.close();
}

void Node3::add_child(Node3* c) {
	children.push_back(c);
}

Node3::Node3(std::string _tag):
		tag(_tag), children() {}  // children een size meegeven


Suffix3::Suffix3(std::string& s) {
	int len = s.length();
	root = new Node3("root");
	
	// first run, insert in root
	Node3* ptr = new Node3(s);
	root->add_child(ptr);
	
	//generate_dot(*this, "blabla", 0);
	
	// for each substring in s
	for (int i=1; i<len; i++) {
		std::string subs = s.substr(i);  // TODO optimalisation!
		std::cout << "\n--------------------------------\n";
		std::cout << subs << "\n";
				
		// Step 1: find head and tail and locus?
		Node3* current = root;
		int end_head = 0;  // head = subs[0:end_head[
		
		while (true) {
			// for each child
			for (Node3* child: current->children) {
				std::cout << "start for\n";
				// compare char by char the tag of the child
					// 3 cases:
						// differs on first char --> check other child
						// differs on some other char --> split! (end_head+++)
						// does not differ --> current = child; end_head+++; break
				if (child->tag[0] != subs[end_head]) {
					// check other child
					std::cout << "case 1, continue\n";
					continue;
				}
				
				int current_pos=1;
				for (; current_pos<child->tag.length(); current_pos++) {
					std::cout << "checking " << child->tag[current_pos] << " != " << subs[end_head+current_pos] << "\n";
					if (child->tag[current_pos] != subs[end_head+current_pos]) {
						// update stuff
						end_head += current_pos;
						current = child;
						
						// split!
						// add children, subs[end_head:] (aka tail) and child->tag[current_pos:]
						Node3* parent_tail = new Node3(current->tag.substr(current_pos));
						parent_tail->children = current->children;
						Node3* new_tail = new Node3(subs.substr(end_head));
						
						current->children.clear();
						current->add_child(parent_tail);
						current->add_child(new_tail);
						
						// set tag
						current->tag = current->tag.substr(0, current_pos);
						
						std::cout << "case 2, end_while\n";
						goto end_while;
					}
				}
				
				current = child;
				end_head += current_pos;
				std::cout << "case 3, continue_while\n";
				goto continue_while;
			}
			
			// extension of case 1: all childs have been checked
			std::cout << subs.length() << "\n";
			std::cout << end_head << "\n";
			current->add_child(new Node3(subs.substr(end_head)));  // add tail [end_head, end...[
			goto end_while;
			
			continue_while:;
		}
		
		end_while:;
		
		// Step 2: find locus and split if necessary (insert other stuff)  <-- tree may contain node with 1 child
		
		// Step 3: insert tail
		
		// Step 4: SsuUuUUUuUuuuUUUuuUUuuuuFfffFffFFffFFFFfFFfFFffFfFfIiIiiiiIIIIIiIiIiIiiiiXXxxXXXxXXXXXXxxX links
		
	}
	
	//generate_dot(*this, "end", 0);
	
	std::cerr << "Done.\n";
	std::cerr << root->children[0]->tag << "\n";
}

std::vector<int> Suffix3::search_string(std::string& str) {
	std::vector<int> result;
	Node3* current_node = root;
	for (int i = 0; i < str.length(); i++){
		for (auto child: current_node->children) {
			//std::cout << " Searching " << child->get_tag() << "...\n";
			if (str[i] != child->tag[0]) {
				continue;
			} else {
				//std::cout << "Match with " << child->get_tag()[0] << " at first position\n";
				for (int j = 0; j < child->tag.length() && i < str.length(); j++) {
					if (str[i] != child->tag[j]) {
					//	std::cerr << i << "th position in " << str << " doesn't match " << j << "th position in " << child->get_tag() << ".\n";
					} else {
						i++;
					}
				}
				if (i < str.length()) {
					current_node = child;
					i--;
					//std::cout << current_node->get_tag() << " is the tag of the next current_node.\n";
					break;
				} else {
					current_node = child;
					/*std::list<int> leaves;
					get_leaves(current_node, leaves);
					for (int k = 0; k < leaves.size(); k++) {
						result.push_back(leaves.front());
						leaves.pop_front();
					}*/
					get_leaves(current_node, str.length(), result);
					/*for (int k = 0; k < result.size(); k++) {
						std::cout << result.front() << std::endl;
						result.pop_front();
					}*/
					return result;
				}
			}
		}
	}
	return result;
}

void Suffix3::get_leaves(Node3* current_node, int str_length, std::vector<int>& leaves) {
	if (!current_node->children.empty()) {
		for (auto child: current_node->children) {
			int original_length = str_length;
			int length = str_length;
			length += child->tag.length();
			get_leaves(child, length, leaves);
			length = original_length;
		}
	} else {
		leaves.push_back(str_length);
	}
}

/*
int_and_node Suffix3::find_head(std::string& subs) {
	Node3* current = root;
	int end_head = 0;
	
	while (! different char) {
		// for each child
			// compare char by char the tag of the child
	}
	
	
	return int_and_node(44, new Node3("balbal"));
}
*/

Suffix3::~Suffix3() {
	delete root;
}

Node3::~Node3() {
	for (Node3* child: children) {
		delete child;
	}
}


// Dot output

std::ostream& operator<<(std::ostream& stream, Suffix3& tree) {
	//stream << "The root of this tree is: " << tree.get_root()->get_tag() << "\n" << *tree.get_root();
	stream << "digraph suffix {\n" << "\tnode [shape = circle];\n";
	int counter;
	tree.root->to_dot(stream, counter);
	stream << '}';
	return stream;
}

void Node3::to_dot(std::ostream& stream, int& i) {
	if (! children.empty()) {
		stream << '\t' << i << " [label= " << '"' << tag << '"' << "];" << "\n";
		int parent_counter = i;
		for (auto child: children) {
			i++;
			stream << '\t' << i << " [label= " << '"' << child->tag << '"' << "];\n";
			stream << '\t' << parent_counter << " -> " << i << ";\n";
			child->to_dot(stream, i);
		}
	}
}
