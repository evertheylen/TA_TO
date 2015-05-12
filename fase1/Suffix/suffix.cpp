/*
 * suffix.cpp
 *
 *  Created on: 6 Apr 2015
 *      Author: stijn
 */

#include "suffix.h"
#include <sstream>
#include <vector>
#include <fstream>
#include <streambuf>

Node::Node(std::string tag) {
	_tag = tag;
	_firstchild = nullptr;
}

Node::~Node() {
	for (auto child: children) {
		delete child;
	}
}

void Node::add_child(Node* child) {
	if (_firstchild == nullptr) {
		_firstchild = child;
		children.push_back(child);
	} else {
		children.push_back(child);
	}
}

std::string Node::get_tag() {
	return _tag;
}

Node* Node::get_firstchild() {
	return _firstchild;
}

void Node::set_tag(std::string tag) {
	_tag = tag;
}

SuffixTree::SuffixTree() {
	_root = new Node("root");
}

void SuffixTree::create(char c, int x) {
	std::stringstream sstr1;
	std::string input;
	sstr1 << c << "$" << x;
	input = sstr1.str();
	add_node(input, *_root);
}

SuffixTree::SuffixTree(std::ifstream& file) {
	_root = new Node("root");
	int length;
	std::stringstream sstr;
	sstr << file.rdbuf();
	_text = sstr.str();
	std::stringstream sstr1, sstr2;
	std::string str = "\0";
	std::string prev = "\0";


	for (int x = _text.length()-1; x >=0; x--){
		sstr1 << _text[x];
		sstr2 << sstr1.str() << prev;
		prev = sstr2.str();
		sstr2 << "$" << x;
		str = sstr2.str();
		sstr1.str("");
		sstr2.str("");
		//std::cout << str << std::endl;
		add_node(str, *_root);
	}
	fix_leaves();
}
SuffixTree::SuffixTree(std::string& text) {
	_root = new Node("root");
	_text = text;
	std::cout << _text << std::endl;
	std::stringstream sstr1, sstr2;
	std::string str = "\0";
	std::string prev = "\0";

	for (int x = text.length()-1; x >=0; x--){
		sstr1 << _text[x];
		sstr2 << sstr1.str() << prev;
		prev = sstr2.str();
		sstr2 << "$" << x;
		str = sstr2.str();
		sstr1.str("");
		sstr2.str("");
		//std::cout << str << std::endl;
		add_node(str, *_root);
	}
	fix_leaves();
}

void SuffixTree::fix_leaves(Node* current_node) {
	if (current_node == nullptr) {
		current_node = _root;
	}
	if (current_node->get_firstchild() != nullptr) {
		for (auto child: current_node->children) {
			this->fix_leaves(child);
		}
	} else {
		for (unsigned int i = 0; i < current_node->get_tag().length(); i++) {
			char s = current_node->get_tag()[i];
			if ('$' == s && i != 0) {
				std::string str1 = current_node->get_tag().substr(0, i);
				std::string str2 = current_node->get_tag().substr(i+1, current_node->get_tag().length()-1);
				current_node->set_tag(str1);
				Node* new_node = new Node(str2);
				current_node->add_child(new_node);
				return;
			}
			else if ('$' == s) {
				std::string str1 = current_node->get_tag().substr(1, current_node->get_tag().length()-1);
				current_node->set_tag(str1);
			}
		}
	}
}

SuffixTree::~SuffixTree() {
	delete _root;
}

Node* SuffixTree::get_root() {
	return _root;
}

void SuffixTree::add_node(std::string tag, Node& current_node) {
	Node* new_node;
	if (current_node.get_firstchild() == nullptr) {
		//std::cout << "nullptr so I added a new firstchild\n";
		new_node = new Node(tag);
		current_node.add_child(new_node);
		return;
	}
	Node* remembered = nullptr;
	for (auto child: current_node.children) {
		//std::cout << "Looping over all the children\n";
		for (unsigned int i = 0; i < child->get_tag().length(); i++) {
			if (tag == "agtacgt$0") {
				//std::cout << child->get_tag() << std::endl;
				//std::cout << tag[i] << "\n";
			}
			if (tag[i] == child->get_tag()[i] && i != child->get_tag().length() - 1) {

				continue;
			} else if (i != 0 && child->get_firstchild() == nullptr) {
				std::string str1 = child->get_tag().substr(0, i);
				std::string str2 = child->get_tag().substr(i, child->get_tag().length()-1);
				/*std::ofstream output_file;
				output_file.open("Before splitting " + child->get_tag() + ".txt");
				output_file << *this;
				output_file.close();*/
				Node& new_current_node = *child;
				new_current_node.set_tag(str1);
				new_node = new Node(str2);
				new_current_node.add_child(new_node);

				std::string str3 = tag.substr(i, tag.length()-1);
				new_node = new Node(str3);
				new_current_node.add_child(new_node);

				//std::cout << "Branch " << child->get_tag() << " is splitted!  Root: " << str1 << "  Original child: " << str2 << "  Newly added child: " << str3 << std::endl;
				/*output_file.open("After splitting " + child->get_tag() + ".txt");
				output_file << *this;
				output_file.close();*/
				return;
			} else if ( i != 0 || (i == child->get_tag().length() - 1 && tag[i] == child->get_tag()[i])) {
				remembered = child;
				//std::cout << "Remembering: " << child->get_tag() << std::endl;
				break;
			} else {
				break;
			}
		}
	}
	if (remembered != nullptr) {
		for (unsigned int i = 0; i < remembered->get_tag().length(); i++) {
			if ( i != 0 || i == remembered->get_tag().length() - 1) {
				//std::cout << "Going down to next child for " << tag  << " Parent: " << remembered->get_tag() << "\n";
				std::string str = tag.substr(i+1, tag.length());
				add_node(str, *remembered);
				return;
			}
		}
	}
	new_node = new Node(tag);
	current_node.add_child(new_node);
	//std::cout << "New branch added for " << tag << std::endl;
	return;
}

std::ostream& operator<<(std::ostream& stream, SuffixTree& tree) {
	//stream << "The root of this tree is: " << tree.get_root()->get_tag() << "\n" << *tree.get_root();
	stream << "digraph suffix {\n" << "\tnode [shape = circle];\n";
	stream << *tree.get_root();
	stream << '}';
	return stream;
}

std::ostream& operator<<(std::ostream& stream, Node& node) {
	int static counter = 0;
	if (node.get_firstchild() != nullptr) {
		stream << '\t' << counter << " [label= " << '"' << node.get_tag() << '"' << "];" << "\n";
		int parent_counter = counter;
		for (auto child: node.children) {
			counter++;
			stream << '\t' << counter << " [label= " << '"' << child->get_tag() << '"' << "];\n";
			stream << '\t' << parent_counter << " -> " << counter << ";\n";
			stream << *child;
		}
	} else {
		return stream;
	}
}

std::list<int> SuffixTree::get_leaves(Node* current_node) {
	static std::list<int> results;
	if (current_node->get_firstchild() != nullptr) {
		for (auto child: current_node->children) {
			//std::cout << "Getting leaves from " << child->get_tag() << std::endl;
			get_leaves(child);
		}
	} else {
		//std::cout << "Got leaf: " << current_node->get_tag().c_str() << std::endl;
		int result = atoi(current_node->get_tag().c_str());
		results.push_back(result);
	}
	return results;
}

std::list<int> SuffixTree::search_string(std::string& str) {
	std::list<int> result;
	Node* current_node = _root;
	for (int i = 0; i < str.length(); i++){
		for (auto child: current_node->children) {
			//std::cout << " Searching " << child->get_tag() << "...\n";
			if (str[i] != child->get_tag()[0]) {
				continue;
			} else {
				//std::cout << "Match with " << child->get_tag()[0] << " at first position\n";
				for (int j = 0; j < child->get_tag().length() && i < str.length(); j++) {
					if (str[i] != child->get_tag()[j]) {
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
					result = get_leaves(current_node);
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

std::list<int> SuffixTree::search_string(std::string& str, int r) {
	std::list<int> result;
	std::stringstream text;
	text << str << "#" << _text;
	std::string suffixtree = text.str();
	SuffixTree s(suffixtree);
	for (int p = 0; p < _text.length(); p++) {
		int i = 0;
		int j = p + str.length() + 1;
		int n = 0;		// # errors.
		while (i < str.length()/* && n <= r*/) {
			//std::cout << "i " << i << " j " << j << " n " << n << std::endl;
			std::string w = s.longest_common_prefix(i, j);	//Should be O(1)?
			//std::cout << w << " is the longest common prefix.\n";
			if (w.length() == 0) {
				i++;				// It's a mismatch.
				j++;
				n++;
			} else {
				i += w.length();	// we have |w| matches.
				j++;
			}
		}
		if (n <= r && str.length() <= _text.substr(p, _text.size()).length()) {
			result.push_back(p);
			std::cout << str << " occurs at position " << p << " with " << n << " errors.\n";
		}
	}
	return result;
}
std::string SuffixTree::longest_common_prefix(int i, int j) {
	std::string result;
	std::string str1 = _text.substr(i, _text.size());
	std::string str2;
	if (j <= _text.size()) {
		str2 = _text.substr(j, _text.size());
	} else {
		str2 = "";
	}
	//std::cout << str1 << ", " << str2 << "\n";
	result = longest_common_prefix(str1, str2);
	return result;
}

std::string SuffixTree::longest_common_prefix(std::string& str1, std::string& str2) {
	std::stringstream result;
	for (int i = 0; i < str1.length(); i++) {
		if (str1[i] == str2[i]) {
			result << str1[i];
		} else {
			break;
		}
	}
	std::string resultstr = result.str();
	return resultstr;
}
