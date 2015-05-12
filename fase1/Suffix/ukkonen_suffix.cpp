/*
 * ukkonen_suffix.cpp
 *
 *  Created on: 12 May 2015
 *      Author: stijn
 */

#include "ukkonen_suffix.h"
#include <sstream>

Edge::Edge(int _a, int _b) {
	a = _a;
	b = _b;
}

Node::Node(int start, Node* _child): e(start, -1), firstchild(nullptr) {}

SuffixTree2::SuffixTree2(std::fstream& file) {
	char input;
	_loc = 0;
	std::stringstream comments;
	_root = new Node(-2, nullptr);
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
}

SuffixTree2::~SuffixTree2() {
	delete _root;
}

void SuffixTree2::add_suffix(char tag) {

}

