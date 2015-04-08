/*
 * suffix.cpp
 *
 *  Created on: 6 Apr 2015
 *      Author: stijn
 */

#include "suffix.h"
#include <sstream>
#include <vector>

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

SuffixTree::SuffixTree(std::string text) {
	_root = new Node("root");
	std::stringstream sstr1, sstr2;
	std::string str = "\0";
	std::string prev = "\0";
	for (int x = text.length()-1; x >=0; x--){
		sstr1 << text[x];
		sstr2 << sstr1.str() << prev;
		prev = sstr2.str();
		sstr2 << "$" << x;
		str = sstr2.str();
		sstr1.str("");
		sstr2.str("");
		std::cout << str << std::endl;
		add_node(str);
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
			if ('$' == s) {
				std::string str1 = current_node->get_tag().substr(0, i);
				std::string str2 = current_node->get_tag().substr(i+1, current_node->get_tag().length()-1);
				current_node->set_tag(str1);
				Node* new_node = new Node(str2);
				current_node->add_child(new_node);
				return;
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

void SuffixTree::add_node(std::string tag) {
	Node& current_node = *_root;
	Node* new_node;
	bool done = false;
	while (!done) {
		if (current_node.get_firstchild() == nullptr) {
			std::cout << "nullptr so I added a new firstchild\n";
			new_node = new Node(tag);
			current_node.add_child(new_node);
			return;
		}
		for (auto child: current_node.children) {
			std::cout << "Looping over all the children\n";
			for (unsigned int i = 0; i < child->get_tag().length(); i++) {
				if (tag[i] == child->get_tag()[i]) {
					continue;
				} else if (i != 0) {
					std::string str1 = child->get_tag().substr(0, i);
					std::string str2 = child->get_tag().substr(i, child->get_tag().length()-1);

					current_node = *child;
					current_node.set_tag(str1);
					new_node = new Node(str2);
					current_node.add_child(new_node);

					std::string str3 = tag.substr(i, tag.length()-1);
					new_node = new Node(str3);
					current_node.add_child(new_node);

					std::cout << "Branch " << child->get_tag() << " is splitted!  Root: " << str1 << "  Original child: " << str2 << "  Newly added child: " << str3 << std::endl;
					return;
				} else {
					break;
				}
			}
		}
		new_node = new Node(tag);
		current_node.add_child(new_node);
		std::cout << "New branch added for " << tag << std::endl;
		return;	//TODO Make a working while loop XD
	}
}

std::ostream& operator<<(std::ostream& stream, SuffixTree& tree) {
	stream << "The root of this tree is: " << *tree.get_root() << "\n";
	stream << "it's children are: ";
	for (auto child: tree.get_root()->children) {
		stream << child->get_tag() << ", ";
	}
	stream << "\nNext row:\n";
	for (auto child: tree.get_root()->children) {
		for (auto child2: child->children) {
			stream << child2->get_tag() << ", ";
		}
	}
	return stream;
}

std::ostream& operator<<(std::ostream& stream, Node& node) {
	stream << node.get_tag();
	return stream;
}
