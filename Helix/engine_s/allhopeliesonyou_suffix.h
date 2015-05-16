
#include <string>
#include <vector>
#include <iostream>

class Node3;
class Suffix3;

// Helper structs

struct int_and_node {
	int pos;
	Node3* node;
	
	int_and_node(int _pos, Node3* _node);
};


class Node3 {
public:
	std::string tag;
	
	std::vector<Node3*> children;
	
	Node3(std::string _tag);
	
	void add_child(Node3* c);
	
	void to_dot(std::ostream& stream, int& i);
	
	~Node3();
};


class Suffix3 {
public:
	Suffix3(std::string& s);
	
// 	int_and_node find_head(std::string& subs);
	
	int str_length;

	Node3* root;
	
	std::vector<int> search_string(std::string& str);
	/*
	 * Returns a vector of indeces of where the substring occurs in your text. Returns an empty list if the string doesn't occur in the text.
	 */

	void get_leaves(Node3* current, int length, std::vector<int>& leaves);

	friend std::ostream& operator<<(std::ostream& stream, Suffix3& tree);
	
	~Suffix3();
};

