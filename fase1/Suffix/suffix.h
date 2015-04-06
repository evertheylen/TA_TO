/*
 * suffix.h
 *
 *  Created on: 6 Apr 2015
 *      Author: stijn
 */

#include <string>
#include <iostream>
#include <list>

#ifndef SUFFIX_SUFFIX_H_
#define SUFFIX_SUFFIX_H_

class Node {
public:
	Node(char tag);
	~Node();
	Node* get_firstchild();
	char get_tag();

	std::list <Node*> children;

	friend std::ostream& operator<<(std::ostream& stream, Node& node);

private:
	Node* _firstchild;
	char _tag;
	//Node* next_child;
};

class SuffixTree {
public:
	SuffixTree(std::string text);
	~SuffixTree();
	void add_node(char tag);
	Node* get_root();

	friend std::ostream& operator<<(std::ostream& stream, SuffixTree& tree);

private:
	Node* _root;
};

#endif /* SUFFIX_SUFFIX_H_ */
