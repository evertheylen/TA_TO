
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <limits>
#include <stdlib.h>

#include "allhopeliesonyou_suffix.h"

// Lelijk
Path::Path(int _errors, int _p, Node3* _node):
		errors(_errors), pos_in_node(_p), node(_node) {}

std::ostream& operator<<(std::ostream& out, Path& p) {
	out << "Path at location [" << p.node->start << ":" << p.node->end << "[ " << " position " << p.pos_in_node << " and " << p.errors << " errors.\n";
	return out;
}

void print_substring(std::string& s, Node3* n, std::ostream& out) {
	//std::cout << "                " << n->end - n->start << "\n";
	out << s.substr(n->start, n->end - n->start);
}

bool eq(std::string& s, int a, int b) {
	return (*(s.c_str()+a) == *(s.c_str()+b));
}

bool eq(const char* cstr, int a, int b) {
	return (*(cstr+a) == *(cstr+b));
}


void generate_dot(Suffix3& s, std::string name, int i) {
	////std::cout << "called!!!!!!!!!!!!!\n";
	name += "_";
	name += std::to_string(i);
	name += ".dot";
	std::ofstream f(name);
	////std::cout << name << "\n";
	f << s;
	f.close();
}


void Node3::add_child(Node3* c) {
	children.push_back(c);
}

Node3::Node3(int _start, int _end):
		start(_start), end(_end) {}  // children een size meegeven? TODO

int Node3::height(int above) {
	int max = above;
	for (Node3* child: children) {
		int childheight = child->height(above+1);
		if (childheight > max) max = childheight;
	}
	return max;
}


Suffix3::Suffix3(std::string& _s):
		s(_s) {
	const char* cstr = s.c_str();
	int len = s.length();
	root = new Node3(0,0); // for the search_string( , int)
	
	// first run, insert in root
	root->add_child(new Node3(0,len));
	
	// for each substring in s
	for (int i=1; i<len; i++) {
		//std::cout << "----------------------------------\n";
		//generate_dot(*this, "blabl", i);
		
		//std::string subs = s.substr(i);  // TODO optimalisation!
		// subs = s[i:[
		//std::cout << "subs = " << s.substr(i, len-i) << "\n";
		
		Node3* current = root;
		int end_head = 0;  // head = subs[i:i+end_head[
		//std::cout << "head = " << s.substr(i,end_head) << "\n";
		
		while (true) {
			// for each child
			for (Node3* child: current->children) {
				// compare char by char the tag of the child
					// 3 cases:
						// differs on first char --> check other child
						// differs on some other char --> split! (end_head+++)
						// does not differ --> current = child; end_head+++; break
				//if (child->tag[0] != subs[end_head]) {
				if (! eq(cstr, child->start, i+end_head)) {
					// check other child
					//std::cout << "check other child\n";
					continue;
				}
				
				int current_pos=1;
				int child_len = child->end - child->start;
				for (; current_pos<child_len; current_pos++) {
					//if (child->tag[current_pos] != subs[end_head+current_pos]) {
					if (! eq(cstr, child->start+current_pos, i+end_head+current_pos)) {
						// update stuff
						end_head += current_pos;
						//std::cout << "  new head = " << s.substr(i,end_head) << "\n";
						current = child;
						
						// split!
						// add children, subs[end_head:] (aka tail) and child->tag[current_pos:]
						//Node3* parent_tail = new Node3(current->tag.substr(current_pos), current->index_start);
						Node3* parent_tail = new Node3(current->start+current_pos, current->end);
						//std::cout << "  parent_tail = "; print_substring(s, parent_tail, //std::cout); //std::cout << "\n";
						parent_tail->children = current->children;
						//Node3* new_tail = new Node3(subs.substr(end_head), i);
						// ......i.....[ subs.substr(end_head) ]
						// ............(i+end_head).............
						// ......[         subs                ]
						// .....................................(len)
						Node3* new_tail = new Node3(i+end_head, len-i-end_head);
						//std::cout << "  new_tail = "; print_substring(s, new_tail, //std::cout); //std::cout << "\n";
						
						current->children.clear();
						current->add_child(parent_tail);
						current->add_child(new_tail);
						
						// set tag
						//current->tag = current->tag.substr(0, current_pos);
						current->end = current->start + current_pos;
						//std::cout << "  new current = "; print_substring(s, current, //std::cout); //std::cout << "\n";
						//std::cout << current->end - current->start << " == " << current_pos+1 << "\n";
						
						goto end_while;
					}
				}
				
				current = child;
				end_head += current_pos;
				//std::cout << " head = " << s.substr(i,end_head) << "\n";
				////std::cout << "case 3, continue_while\n";
				goto continue_while;
			}
			
			// extension of case 1: all childs have been checked
			////std::cout << subs.length() << "\n";
			////std::cout << end_head << "\n";
			//current->add_child(new Node3(subs.substr(end_head), i));  // add tail [end_head, end...[
			current->add_child(new Node3(i+end_head, len));
			//std::cout << "add child = " << s.substr(i+end_head, end_head) << "\n";
			goto end_while;
			
			continue_while:;
		}
		end_while:;
	}
	
	//generate_dot(*this, "end", 0);
	
	//std::cerr << "Done.\n";
	//std::cerr << root->children[0]->tag << "\n";
}

std::vector<int> Suffix3::search_string(std::string& str) {
	std::vector<int> result; /*
	Node3* current_node = root;
	for (int i = 0; i < str.length(); i++){
		for (auto child: current_node->children) {
			////std::cout << " Searching " << child->get_tag() << "...\n";
			if (str[i] != child->tag[0]) {
				continue;
			} else {
				////std::cout << "Match with " << child->get_tag()[0] << " at first position\n";
				for (int j = 0; j < child->tag.length() && i < str.length(); j++) {
					if (str[i] != child->tag[j]) {
					//	//std::cerr << i << "th position in " << str << " doesn't match " << j << "th position in " << child->get_tag() << ".\n";
					} else {
						i++;
					}
				}
				if (i < str.length()) {
					current_node = child;
					i--;
					////std::cout << current_node->get_tag() << " is the tag of the next current_node.\n";
					break;
				} else {
					current_node = child;
					get_leaves(current_node, result);
					return result;
				}
			}
		}
	}*/
	return result;
}

// we only support max_int-1 errors :)
std::vector<int> Suffix3::search_string(std::string& str, int errors) {
	std::vector<Path> paths = {Path(0, 0, root)};
	/*
	// paths solely go forward, not backward (parent) or right or left (siblings).	
	for (int i=0; i<str.length(); i++) {
		std::vector<Path> new_paths;

		for (Path& p: paths) {
			if (p.pos_in_node+1 < p.node->tag.size()) {
				// we are still in this node
				if (str[i] != p.node->tag[p.pos_in_node+1]) {
					p.errors++;
				}
				p.pos_in_node++;
			} else {
				// we need to take a new path!

				// check all children for first character
				for (Node3* child: p.node->children) {
					new_paths.push_back(Path(p.errors+int(child->tag[0] != str[i]), 0, child));
				}
				p.errors = std::numeric_limits<int>::max();
			}
		}
		paths.insert(paths.end(), new_paths.begin(), new_paths.end());
		
		// delete paths over their error count
		for (int j=0; j<paths.size(); j++) {
			if (paths[j].errors > errors) {
				// delete!
				paths.erase(paths.begin()+j);
				j--; // next time, i is same as now, but we deleted, so next path
			}
		}
		for (Path p: paths) {
			//std::cout << p;
		}
		//std::cout << "--------\n";
	}

	*/
	std::vector<int> result;
	for (Path p: paths) {
		//std::cout << p.node << std::endl;
		get_leaves(p.node, result);
	}
	return result;
}


void Suffix3::get_leaves(Node3* current_node, /*int length,*/ std::vector<int>& leaves) {
	if (current_node->children.empty()) {
		leaves.push_back(current_node->start); // NOT THE SAME DAMNIT TODO
	} else {
		for (Node3* child: current_node->children) {
			get_leaves(child, leaves);
		}
	}
}


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
	int counter = 0;
	tree.root->to_dot(stream, counter, tree.s);
	stream << '}';
	return stream;
}


void Node3::to_dot(std::ostream& stream, int& i, std::string& s) {
	if (! children.empty()) {
		stream << '\t' << i << " [label= \"";
		print_substring(s, this, stream);
		stream << "\"];\n";
		int parent_counter = i;
		for (auto child: children) {
			i++;
			stream << '\t' << i << " [label= \"";
			print_substring(s, child, stream);
			stream << "\"];\n";
			stream << '\t' << parent_counter << " -> " << i << ";\n";
			child->to_dot(stream, i, s);
		}
	}
}
