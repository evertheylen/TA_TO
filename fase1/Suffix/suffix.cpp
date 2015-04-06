/*
 * suffix.cpp
 *
 *  Created on: 6 Apr 2015
 *      Author: stijn
 */

#include "suffix.h"
#include <sstream>

Node::Node(std::string tag) {
	_tag = tag;
	_firstchild = nullptr;
}

Node::~Node() {
	for (auto child: children) {
		delete child;
	}
}

void Node::set_firstchild(Node* firstchild) {
	_firstchild = firstchild;
}

std::string Node::get_tag() {
	return _tag;
}

Node* Node::get_firstchild() {
	return _firstchild;
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

void SuffixTree::fix_leaves() {

}

SuffixTree::~SuffixTree() {
	delete _root;
}

Node* SuffixTree::get_root() {
	return _root;
}

void SuffixTree::add_node(std::string tag) {
	Node current_node = *_root;
	Node* new_node;
	bool done = false;
	while (!done) {
		if (current_node.get_firstchild() == nullptr) {
			std::cout << "nullptr so I added a new firstchild\n";
			new_node = new Node(tag);
			current_node.set_firstchild(new_node);
			return;
		}
		for (auto child: current_node.children) {
			std::cout << "Looping over all the children\n";
			for (unsigned int i = 0; i < child->get_tag().length(); i++) {
				std::cout << "Looping over all the chars of " << child->get_tag() << std::endl;
				if (tag[i] == child->get_tag()[i]) {
					continue;
				} else if (i != 0) {
					std::string str1 = child->get_tag().substr(0, i);
					std::string str2 = child->get_tag().substr(i, child->get_tag().length()-1);

					std::cout << "Original string: " << child->get_tag() << "  First part: " << str1 << "  Second part: " << str2 << std::endl;
				}
			}
		}
		return;	//TODO Make a working while loop XD
	}
}

std::ostream& operator<<(std::ostream& stream, SuffixTree& tree) {
	stream << "The root of this tree is: " << *tree.get_root() << "\n";
	return stream;
}

std::ostream& operator<<(std::ostream& stream, Node& node) {
	stream << node.get_tag();
	return stream;
}
