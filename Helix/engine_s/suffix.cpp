
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <limits>
#include <stdlib.h>

#include "suffix.h"

#include "search.h"

#define uint unsigned int

bool eq(std::string& s, int a, int b) {
	return (*(s.c_str()+a) == *(s.c_str()+b));
}

bool eq(const char* cstr, int a, int b) {
	return (*(cstr+a) == *(cstr+b));
}

bool eq(std::string& sa, int a, std::string& sb, int b) {
	return (*(sa.c_str()+a) == *(sb.c_str()+b));
}

bool eq(const char* cstra, int a, const char* cstrb, int b) {
	return (*(cstra+a) == *(cstrb+b));
}




void generate_dot(Suffix3* s, std::string name, int i) {
	//std::cout << "called!!!!!!!!!!!!!\n";
	name += "_";
	name += std::to_string(i);
	name += ".dot";
	std::ofstream f(name);
	////std::cout << name << "\n";
	f << *s;
	f.close();
}

// ------[ Node ]---------------------------

// void Node3::add_child(Node3* c) {
// 	children.push_back(c);
// }

Node3::Node3(uint _start, uint _end, uint _index, uint _suffix_link, uint _lchild, uint _rbrother):
		start(_start), end(_end), index(_index), suffix_link(_suffix_link), leftmost_child(_lchild), right_brother(_rbrother) {
	
}

void Node3::print(std::ostream& out) {
	out << "address: " << this << "\n";
	out << "start: " << start << "\n";
	out << "end: " << end << "\n";
	out << "index: " << index << "\n";
	out << "suffix_link: " << suffix_link << "\n";
	out << "leftmost_child: " << leftmost_child << "\n";
	out << "right_brother: " << right_brother << "\n\n";
}


int Node3::height(int above, Suffix3& tree) {
	int max = above;
	for (uint child_i=leftmost_child; child_i != 0; child_i = tree.data[child_i].right_brother) {
		int childheight = tree.data[child_i].height(above+1, tree);
		if (childheight > max) max = childheight;
	}
	return max;
}

// void Node3::add_children(std::vector< Node3* >& nodes) {
// 	for (Node3* child: children) {
// 		nodes.push_back(child);
// 		child->add_children(nodes);
// 	}
// }





// ------[ Suffix ]---------------------------

Suffix3::Suffix3(std::string* content):
		s(content),
		data(content->length()*1.6) {
    //std::cout << filename << std::endl;
	build();
}

Suffix3::Suffix3(std::string* content, std::ifstream& f):
		s(content), data(f) {}


void Suffix3::save(std::ofstream& f) {
	// FancyVector<Node3> data;
	data.save(f);
}



void Suffix3::add_char(char c) {
	s += c;
}

void Suffix3::build() {
	const char* cstr = s->c_str();
	uint len = s->length();
	
	// root
	data.emplace_back(Node3(0,0,0,0,1,0));
	
	// first run, insert in root
	//root->add_child(new Node3(0,len,0));
	data.emplace_back(Node3(0,len,0));
	
	// loading bar
	int progress = len/100;
	int current_progress = 0;
	std::cout << "0        10        20        30        40        50        60        70        80        90        100\n";
	
	Node3* current = &data[0];
	uint current_i = 0;
	// for each substring in s
	for (uint i=1; i<len-1; i++) {			// TODO Just for attention :) changed from len to len-1
		//generate_dot(*this, "blabl", i);
		
// 		std::cout << "\n\n--------[" << i << "]----------------\n";
// 		for (int j=0; j<data.size(); j++) {
// 			data[j].print(std::cout);
// 		}
// 		std::cout << "--------------------------------------\n\n";
		
		// subs = s[i:[
		//std::cout << "subs = " << s.substr(i, len-i) << "\n";
		uint previous_insert = 0;
		//node_it previous_insert_it = data.begin();
		
		uint end_head = 0;  // head = subs[i:i+end_head[
		//std::cout << "head = " << s.substr(i,end_head) << "\n";
		
		while (true) {
			// for each child
			// for (Node3* child: current->children) {
			uint child_i = current->leftmost_child;
			Node3* child = &data[child_i];
			while (child_i != 0) {
				// compare char by char the tag of the child
					// 3 cases:
						// differs on first char --> check other child
						// differs on some other char --> split! (end_head+++)
						// does not differ --> current = child; end_head+++; break
				//if (child->tag[0] != subs[end_head]) {
				// case 1
				if (! eq(cstr, child->start, i+end_head)) {
					// check other child
					//std::cout << "check other child\n";
					child_i = child->right_brother;
					child = &data[child_i];
					continue;
				}
				
				// case 2
				uint current_pos=1;
				uint child_len = child->end - child->start;
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
						uint parent_tail_i = data.size();
						if (data.emplace_back(Node3(current->start+current_pos, current->end, current->index)));
							// References became invalid!
							current = &data[child_i];
						Node3* parent_tail = &data.back();
						
						//std::cout << "  parent_tail = "; print_substring(s, parent_tail, //std::cout); //std::cout << "\n";
						parent_tail->leftmost_child = current->leftmost_child;
						//Node3* new_tail = new Node3(subs.substr(end_head), i);
						// ......i.....[ subs.substr(end_head) ]
						// ............(i+end_head).............
						// ......[         subs                ]
						// .....................................(len)
						uint new_tail_i = parent_tail_i+1;
						if (data.emplace_back(Node3(i+end_head, len, i, 0, 0, parent_tail_i))) // also set right brother!
							// References became invalid!
							current = &data[child_i];
						
						//std::cout << "  new_tail = "; print_substring(s, new_tail, //std::cout); //std::cout << "\n";
						
						//current->children.clear();
						//current->add_child(parent_tail);
						//current->add_child(new_tail);
						current->leftmost_child = new_tail_i;
						
						// set tag
						//current->tag = current->tag.substr(0, current_pos);
						current->end = current->start + current_pos;
						//std::cout << "  new current = "; print_substring(s, current, //std::cout); //std::cout << "\n";
						//std::cout << current->end - current->start << " == " << current_pos+1 << "\n";
						
						if (previous_insert != 0 && end_head > 1) {
							current->suffix_link = previous_insert;
						} else {
							current->suffix_link = 0;
						}
						previous_insert = child_i;  // should be the index of current by this point
						
						goto end_while;
					}
				}
				
				// case 3
				current = child;
				current_i = child_i;
				end_head += current_pos;
				//std::cout << " head = " << s.substr(i,end_head) << "\n";
				////std::cout << "case 3, continue_while\n";
				goto continue_while;
			}
			
			// extension of case 1: all childs have been checked
			////std::cout << subs.length() << "\n";
			////std::cout << end_head << "\n";
			//current->add_child(new Node3(subs.substr(end_head), i));  // add tail [end_head, end...[
			previous_insert = data.size();
			if (data.emplace_back(Node3(i+end_head, len, i, 0, 0, current->leftmost_child)))
				current = &data[current_i]; // references invalidated
			current->leftmost_child = previous_insert;
			//std::cout << "add child = " << s.substr(i+end_head, end_head) << "\n";
			goto end_while;
			
			continue_while:;
		}
		end_while:;
		current_i = current->suffix_link;
		current = &data[current->suffix_link];
		
		//generate_dot(this, "blabl_end", i);
		current_progress++;
		if (current_progress==progress) {
			std::cout << "#";
			std::cout.flush();
			current_progress=0;
		}
	}
	
	std::cout << "\nDone.\n";
	//generate_dot(this, "suffix_tree", 0);
	
// 	std::cout << "\n\n--------[ final ]----------------\n";
// 	for (int j=0; j<data.size(); j++) {
// 		data[j].print(std::cout);
// 	}
// 	std::cout << "--------------------------------------\n\n";
	
	//std::cerr << "Done.\n";
	//std::cerr << root->children[0]->tag << "\n";
}


void Suffix3::get_leaves(uint current, std::vector<int>& leaves) {
	if (data[current].leftmost_child == 0) {
		leaves.push_back(data[current].index);
	} else {
		for (uint child_i=data[current].leftmost_child; child_i != 0; child_i = data[child_i].right_brother) {
			get_leaves(child_i, leaves);
		}
	}
}


char Suffix3::get(SuffixPosition& pos) {
	return (*s)[data[pos.node].start + pos.pos_in_node];
}

void Suffix3::stats(std::ostream& out) {
	out << "Starting stats\n.";
	out << "sizeof node: " << sizeof(Node3) << "\n";
	
	double n = data.size();
	out << "total nodes: " << n << "\n";
	long long int memory = 0;
	
	memory += n*sizeof(Node3);
	
	memory += s->size()*sizeof(char);
	
	out << "string size: " << s->size() << "\n";
	out << "memory: " << memory << "\n";
// 	out << "height: " << root->height(0) << "\n";
}



// Dot output

std::ostream& operator<<(std::ostream& stream, Suffix3& tree) {
	//stream << "The root of this tree is: " << tree.get_root()->get_tag() << "\n" << *tree.get_root();
	stream << "digraph suffix {\n" << "\tnode [shape = circle];\n";
	int counter = 0;
	tree.data[0].to_dot(stream, 0, tree);
	stream << '}';
	return stream;
}

void Node3::print_substring(std::string* s, std::ostream& out) {
	out << s->substr(start, end - start);
}


void Node3::to_dot(std::ostream& stream, int own_index, Suffix3& tree) {
	stream << '\t' << own_index << " [label= \"";
	print_substring(tree.s, stream);
	stream << " (" << index << ", " << own_index << ")\"];\n";
	for (uint child_i=leftmost_child; child_i != 0; child_i = tree.data[child_i].right_brother) {
		stream << '\t' << own_index << " -> " << child_i << ";\n";
		tree.data[child_i].to_dot(stream, child_i, tree);
	}
}
