
#ifndef suffix3_H_
#define suffix3_H_

#include <string>
#include <vector>
#include <iostream>
#include <deque>

#include <cstdlib>

class Node3;
class Suffix3;

void generate_dot(Suffix3* s, std::string name, int i);

template<typename T>
class FancyVector {
public:
	std::vector<T> cons_data; // 'conservative data'
	std::deque<T> extra;
	
	bool vector_full;
	int _size;
	int _cons_cap;
	
	
	FancyVector(int cap=0):
			vector_full(false), _size(0), _cons_cap(cap) {
		if (cap == 0) {
			vector_full = true;
		} else {
			cons_data = std::vector<T>(cap, T());
		}
		std::cout << "cted with cap " << cap << "\n";
	}
	
	T& operator[](unsigned int loc) {
		if (loc < _cons_cap) {
			return cons_data[loc];
		} else {
			return extra[loc-_cons_cap];
		}
	}
	
	// returns whether previous pointers may be invalid
	bool push_back(T obj) {
		_size++;
		if (_size > _cons_cap) {
			vector_full = true;
		}
		if (!vector_full) {
			cons_data[_size-1] = obj;
			return false;
		} else {
			extra.push_back(obj);
			return true;
		}
	}
	
	// :3
	bool emplace_back(T obj) {
		return push_back(obj);
	}
	
	T& back() {
		return operator[](_size-1);
	}
	
	unsigned int size() {
		return _size;
	}
	
	unsigned int cons_cap() {
		return _cons_cap;
	}
	
	void print(std::ostream& out, int num) {
		out << "size: " << size();
		out << " {";
		out << operator[](0);
		for (int i=1; i<num; i++) {
			out << ", " << operator[](i);
		}
		out << "}\n";
	}
};




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

	Node3(unsigned int _start=0, unsigned int _end=0, unsigned int _index=0, unsigned int _suffix_link=0, unsigned int _lchild=0, unsigned int _rbrother=0);
	
	//void add_child(Node3* c);
	
	void to_dot(std::ostream& stream, int own_index, Suffix3& suf);
	
	void print_substring(std::string* s, std::ostream& out);
	
	void print(std::ostream& out);
	
	
	int height(int, Suffix3&);
	
	//void add_children(std::vector<Node3*>& nodes);
};

class SuffixPosition;

class Suffix3 {
public:
	Suffix3(std::string* s);

	//Suffix3(std::string& s);
	
	void fuck_you(std::string s);

	void build();
	
	std::string* s;
	std::string filename;

	//Node3* root;
	FancyVector<Node3> data;
	
	void add_char(char c);
	
	char get(SuffixPosition& pos);
	
	void get_leaves(unsigned int current, std::vector<int>& leaves);

	friend std::ostream& operator<<(std::ostream& stream, Suffix3& tree);
	
	void stats(std::ostream& out);
	
	//~Suffix3();
};

#endif
