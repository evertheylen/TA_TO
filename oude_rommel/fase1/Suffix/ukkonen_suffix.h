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

class Node2 {
public:
	Node2(int start, int end = -1);
	std::string tag = "not resolved yet!";
	Edge e;
	Node2* suffix_link;
	Node2* firstchild;
	std::list <Node2*> children;

	~Node2();

	void add_child(Node2* child);

	friend std::ostream& operator<< (std::ostream& out, Node2& n);
};

struct Activepoint {
	Node2* node;
	char edge;
	int length;
	Edge* p;
};

class SuffixTree2 {
public:
	SuffixTree2(std::fstream& file, std::string filename);
	~SuffixTree2();

	void resolve(Node2* current_node);

	void add_suffix(char tag);

	std::string get_tag(Node2* n);

	int check_presence(char tag);

	friend std::ostream& operator<< (std::ostream& out, SuffixTree2& tree);

private:
	Activepoint active;
	std::fstream& _file;
	std::string filename;
	std::string remaining_suffix;
	Node2* _root;
	int _loc;
	int remainder;
};

#endif /* SUFFIX_UKKONEN_SUFFIX_H_ */
