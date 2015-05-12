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

Node2::Node2(int start): e(start, -1), firstchild(nullptr) {}

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

SuffixTree2::SuffixTree2(std::fstream& file): _file(file){
	char input;
	_loc = 0;
	std::stringstream comments;
	_root = new Node2(-2);
	while (file.good()) {
		file.get(input);
		if (file.eof()) {
			break;
		}
		if (input != '>' && input != '\n') {
			add_suffix(input);
			_loc++;
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
	resolve(_root);
}

SuffixTree2::~SuffixTree2() {
	delete _root;
}

void SuffixTree2::add_suffix(char tag) {
	Node2* n = new Node2(_loc);
	_root->add_child(n);
}

void SuffixTree2::resolve(Node2* current_node) {
	if (current_node->firstchild != nullptr) {
		for (auto child: current_node->children) {
			resolve(child);
		}
	}
	std::cout << "Edge: From " << current_node->e.a << " to ";
	if (current_node->e.b == -1) {
		current_node->e.b = _loc;
	}
	std::cout << current_node->e.b << std::endl;
	if (current_node->e.a != -2) {
		int length = current_node->e.b - current_node->e.a-1;
		char * buffer = new char [length];
		_file.seekg(current_node->e.a);
		_file.read(buffer, length);
		std::string tag = buffer;
		std::cout << length <<std::endl;
		delete[] buffer;
		current_node->tag = tag;
	} else {
		current_node->tag = "root";
	}
	std::cout << current_node->tag << std::endl;
}

std::ostream& operator<< (std::ostream& out, SuffixTree2& tree) {
	out << tree._root;
	return out;
}
std::ostream& operator<<(std::ostream& out, Node2& n) {
	if (n.firstchild != nullptr) {
		for (auto child: n.children) {
			out << child;
		}
	} else {
	}
	return out;
}

