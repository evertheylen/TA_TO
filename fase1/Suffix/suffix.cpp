/*
 * suffix.cpp
 *
 *  Created on: 6 Apr 2015
 *      Author: stijn
 */

#include "suffix.h"

Node::Node(char tag) {
	_tag = tag;
	_firstchild = nullptr;
}

Node::~Node() {
	for (auto child: children) {
		delete child;
	}
}

char Node::get_tag() {
	return _tag;
}

Node* Node::get_firstchild() {
	return _firstchild;
}

SuffixTree::SuffixTree(std::string text) {
	_root = nullptr;

	for(char& c: text) {
	    add_node(c);
	}
}

SuffixTree::~SuffixTree() {
	delete _root;
}

Node* SuffixTree::get_root() {
	return _root;
}

void SuffixTree::add_node(char tag) {
	if (_root == nullptr) {
		_root = new Node(tag);
		return;
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
