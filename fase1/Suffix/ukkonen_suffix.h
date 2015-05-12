/*
 * ukkonen_suffix.h
 *
 *  Created on: 12 May 2015
 *      Author: stijn
 */


#include <fstream>
#include <list>

#ifndef SUFFIX_UKKONEN_SUFFIX_H_
#define SUFFIX_UKKONEN_SUFFIX_H_

class Edge {
public:
	Edge(int _a, int _b);
	int a;
	int b;
};

class Node {
public:
	Node(int start, Node* child);
	Edge e;
	Node* firstchild;
	std::list <Node*> children;
};

class SuffixTree2 {
public:
	SuffixTree2(std::fstream& file);
	~SuffixTree2();

	void add_suffix(char tag);

private:
	Node* _root;
	int _loc;
};



#endif /* SUFFIX_UKKONEN_SUFFIX_H_ */
