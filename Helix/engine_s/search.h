
#ifndef _H_Search
#define _H_Search

#include <iostream>
#include <string>
#include <list>

#include <vector>
#include <unordered_map>

#include "FSM.h"
#include "Run.h"
#include "CompactDFA.h"

#include "suffix.h"
#include "allhopeliesonyou_suffix.h"

// table about DFA+suffix is in search.cpp

enum class Status: char {
	Perfect,
	Fake,
	Skip,
	Repetition,
	Ignore
};

struct FChar {
	char c;
	Status s;
	
	FChar(char _c, Status _s);
	
	friend std::ostream& operator<<(std::ostream& out, FChar fc);
};

std::ostream& operator<<(std::ostream& out, std::vector<FChar>& v);

// For more advanced suffix tree states
class SuffixPosition {
public:
	std::vector<Node3*> parents;
	Node3* node;
	int pos_in_node;

	SuffixPosition(Node3* _node, int _pos_in_node, std::vector<Node3*> _parents = {});

	std::vector<SuffixPosition> branch(Suffix3& suf);
	
	void print(std::ostream& out,  Suffix3& tree);
};


class DFAPosition {
public:
	int state;
	char prev_char;
	
	DFAPosition(int s, char c);
	
	// Never branches to dead states!
	std::vector<DFAPosition> branch(CompactDFA& D);
};


class FancyPath {
public:
	// Also contains errors
	std::vector<FChar> text;
	
	int fakes;
	int skips;
	int reps;
	int ignores;
	
	// where are we in the Suffixtree?
	SuffixPosition suf_pos;

	// where are we in the DFA?
	DFAPosition dfa_pos;
	
	FancyPath(SuffixPosition s, DFAPosition d, FancyPath& old);
			
	FancyPath(SuffixPosition s, DFAPosition d);
	
	void add_perfect(char c);
	
	void add_fake(char c);
	
	void add_skip(char c);
	
	void add_rep(char c);
	
	void add_ignore(char c);
	
	friend std::ostream& operator<<(std::ostream& out, FancyPath& p);
	
	int get_total_errors();
	
	// More info than operator<<
	void print(std::ostream& out, Suffix3& tree);
};


class Query {
public:
	CompactDFA D;
	
	int max_fakes;
	int max_skips;
	int max_reps;
	int max_ignores;

	int max_total;

	Query(CompactDFA D, int f, int s, int r, int i, int m);

	std::vector<FancyPath> search(Suffix3& suf);
};

#endif

