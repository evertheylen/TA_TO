
#ifndef suffix3_H_
#define suffix3_H_

#include <string>
#include <vector>
#include <iostream>

class Node3;
class Suffix3;

void print_substring(std::string& s, Node3* n, std::ostream& out);

void generate_dot(Suffix3& s, std::string name, int i);


// Helper structs

struct Path {
	int errors;
	int pos_in_node;


	Node3* node;
	
	Path(int _errors, int _p, Node3* _node);

	friend std::ostream& operator<<(std::ostream& out, Path& p);
};


class Node3 {
public:
	//std::string tag;  == s[start:end[
	int start;
	int end;
	
	int index;

	Node3* suffix_link;

	std::vector<Node3*> children;
	

	Node3(int _start, int _end, int _index);
	
	void add_child(Node3* c);
	
	void to_dot(std::ostream& stream, int& i, std::string& s);
	
	int height(int);
	
	~Node3();
};

class SuffixPosition;

class Suffix3 {
public:
	Suffix3();

	//Suffix3(std::string& s);

	void build();
	
// 	int_and_node find_head(std::string& subs);
	
	std::string s;

	Node3* root;
	
	void add_char(char c);
	
	char get(SuffixPosition& pos);
	
	std::vector<int> search_string(std::string& str);
	/*
	 * Returns a vector of indeces of where the substring occurs in your text. Returns an empty list if the string doesn't occur in the text.
	 */
	
	std::vector<int> search_string(std::string& std, int errors);
	
	void get_leaves(Node3* current, /*int length, */std::vector<int>& leaves);

	friend std::ostream& operator<<(std::ostream& stream, Suffix3& tree);
	
	~Suffix3();
};

#endif
