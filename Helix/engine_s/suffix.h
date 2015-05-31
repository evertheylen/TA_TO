
#ifndef suffix3_H_
#define suffix3_H_

#include <string>
#include <vector>
#include <iostream>
#include <deque>

class Node3;
class Suffix3;

void generate_dot(Suffix3& s, std::string name, int i);




class Node3 {
public:
	//std::string tag;  == s[start:end[
	unsigned int start; // 4
	unsigned int end; // 4
	
	unsigned int index; // 4

	unsigned int suffix_link; // 4
	
	unsigned int leftmost_child;
	
	unsigned int right_brother;
	
	//std::vector<Node3*> children; // 24

	Node3(unsigned int _start, unsigned int _end, unsigned int _index, unsigned int _suffix_link=0, unsigned int _lchild=0, unsigned int _rbrother=0);
	
	//void add_child(Node3* c);
	
	void to_dot(std::ostream& stream, int own_index, Suffix3& suf);
	
	void print_substring(std::string& s, std::ostream& out);
	
	//int height(int);
	
	//void add_children(std::vector<Node3*>& nodes);
};

class SuffixPosition;

class Suffix3 {
public:
	Suffix3();

	//Suffix3(std::string& s);

	void build();
	
	std::string s;
	std::string filename;

	//Node3* root;
	std::deque<Node3> data;
	
	void add_char(char c);
	
	char get(SuffixPosition& pos);
	
	void get_leaves(unsigned int current, std::vector<int>& leaves);

	friend std::ostream& operator<<(std::ostream& stream, Suffix3& tree);
	
	void stats(std::ostream& out);
	
	//~Suffix3();
};

#endif
