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
	Node(std::string tag);
	~Node();
	Node* get_firstchild();
	void add_child(Node* child);
	std::string get_tag();
	void set_tag(std::string tag);

	std::list <Node*> children;

	friend std::ostream& operator<<(std::ostream& stream, Node& node);

private:
	Node* _firstchild;
	std::string _tag;
	//Node* next_child;
};

class SuffixTree {
public:
	SuffixTree(std::string text);
	~SuffixTree();
	void add_node(std::string tag);
	Node* get_root();

	void fix_leaves(Node* current_node = nullptr); // TODO implement; this function removes all the $i from the tags of the nodes and adds a leaf with the index.

	friend std::ostream& operator<<(std::ostream& stream, SuffixTree& tree);

private:
	Node* _root;
};

#endif /* SUFFIX_SUFFIX_H_ */
