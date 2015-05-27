
#include <iostream>
#include <string>
#include <list>

#include <vector>
#include <unordered_map>

#include "FSM.h"
#include "CompactDFA.h"

#include "suffix.h"
#include "allhopeliesonyou_suffix.h"

//
// Table on DFA+Suffix stuff:
//
// -------+        |                    D F A                    |
// SUFFIX  \       |         stay         |       advance        |
//          +------+----------------------+----------------------+
//   stay          | /  /  /  /  /  /  /  |                      |
// (can't be OK    |/  /  /  /  /  /  /  /|        Skip          |
//     or not OK)  |  /  /  /  /  /  /  / |                      |
// ----------------+----------------------+----------------------+
//                 |                      |                      |
//            OK   |      Repetition      |       Perfect!       |
//                 |                      |                      |
//  advance - - - -+- - - - - - - - - - - + - - - - - - - - - - -+
//                 |                      |                      |
//          NOT OK |   Ignore / FakeRep   |         Fake         |
//                 |                      |                      |
//  ---------------+----------------------+----------------------+
//

struct Error {
	int location;

	Error(int _loc):
		location(_loc) {}
};

typedef Error Fake;
typedef Error Skip;
typedef Error Repetition;
typedef Error Ignore; // aka FakeRepetition

// For more advanced suffix tree states
class SuffixPosition {
public:
	Node3* node;
	int pos_in_node;

	SuffixPosition(Node3* _node, int _pos_in_node):
		node(_node), pos_in_node(_pos_in_node) {}

	std::vector<SuffixPosition> branch(Suffix3& suf) {
		if (pos_in_node < (node->end - node->start)) {
			return {SuffixPosition(node, pos_in_node+1)};
		}

		std::vector<SuffixPosition> result;
		for (Node3* child: node->children) {
			result.emplace_back(SuffixPosition(child, 0));
		}
		return result;
	}
};


class DFAPosition {
public:
	int state;
	char prev_char;
	
	DFAPosition(int s, char c):
		state(s), prev_char(c) {}
	
	std::vector<DFAPosition> branch(CompactDFA& D) {
		std::vector<DFAPosition> result;
		for (int i_a=0; i_a<D.sigma.size(); i_a++) {
			result.emplace_back(DFAPosition(D.d_data[state][i_a], D.sigma[i_a]));
		}
		return result;
	}
};


class FancyPath {
public:
	std::vector<Fake> fakes;
	std::vector<Skip> skips;
	std::vector<Repetition> reps;
	std::vector<Ignore> ignores;
	
	// where are we in the Suffixtree?
	SuffixPosition suf_pos;

	// where are we in the DFA?
	DFAPosition dfa_pos;
	
	// total text so far?
	std::string text;
	
	FancyPath(SuffixPosition s, DFAPosition d):
			suf_pos(s), dfa_pos(d) {}
	
	void add_fake() {
		fakes.push_back(text.size());
	}
	
	void add_skip() {
		skips.push_back(text.size());
	}
	
	void add_rep() {
		reps.push_back(text.size());
	}
	
	void add_ignore() {
		ignores.push_back(text.size());
	}
	
	friend std::ostream& operator<<(std::ostream& out, FancyPath& p) {
		out << "Text is: " << p.text << "\n";
		for (Fake& f: p.fakes)
			out << "  Fake on location: " << f.location << "\n";
		for (Skip& s: p.ignores)
			out << "  Skip on location: " << s.location << "\n";
		for (Repetition& r: p.reps)
			out << "  Repetition on location: " << r.location << "\n";
		for (Ignore& i: p.ignores)
			out << "  Ignore on location: " << i.location << "\n";
		
		return out;
	}
};


class Query {
public:
	int max_fakes;
	int max_skips;
	int max_reps;
	int max_ignores;

	int max_total;

	Query(int f, int s, int r, int i, int m):
			max_fakes(f), max_skips(s), max_reps(r), max_ignores(i), max_total(m) {}

	// Is this a path to the Dark Side?
	bool valid(FancyPath& p) {
		int total = p.fakes.size() + p.skips.size() + p.reps.size() + p.ignores.size();
		return (total <= max_total)
		       && (p.fakes.size() <= max_fakes)
		       && (p.skips.size() <= max_skips)
		       && (p.reps.size() <= max_reps)
		       && (p.ignores.size() <= max_ignores);
	}
};


char get(Suffix3& suf, SuffixPosition& pos) {
	return suf.s[pos.node->start + pos.pos_in_node];
}

std::vector<FancyPath> fucking_fancy_search(Suffix3& suf, CompactDFA& D, Query& q) {
	std::vector<FancyPath> result;

	std::vector<FancyPath> working_set = {FancyPath(SuffixPosition(suf.root, 0), DFAPosition(D.q0, '\0'))};
	
	int run = 0;
	int limit = 2;
	
	while (working_set.size() > 0) {
		std::vector<FancyPath> new_set;
		
		std::cout << "\nstarting while with working_set:\n";
		for (auto p: working_set) {
			std::cout << p;
		}
		
		for (auto it=working_set.begin(); it!=working_set.end(); ++it) {
			FancyPath& path = *it;

			// ___ Suffix advance ___
			for (SuffixPosition new_suf_pos: path.suf_pos.branch()) {
				char new_char = get(suf, new_suf_pos);
				
				// ___ Suffix advance, DFA advance ___
				for (DFAPosition new_dfa_pos: path.dfa_pos.branch(D)) {
					// Perfect or Fake?
					FancyPath new_path(new_suf_pos, new_dfa_pos);
					if (new_dfa_pos.prev_char != new_char) {
						// Fake character!
						new_path.add_fake();
					} // else, it's Perfect, so no errors
					new_path.text += new_char;
					new_set.push_back(new_path);
				}
				
				// ___ Suffix advance, DFA stay ___
				FancyPath new_path(new_suf_pos, path.dfa_pos);
				if (new_char == path.dfa_pos.prev_char) {
					// Repetition
					new_path.add_rep();
				} else {
					// Ignore Suffix input completely
					new_path.add_ignore();
				}
				new_path.text += new_char;
				new_set.push_back(new_path);
			}

			// ___ Suffix stay, DFA advance ___
			for (DFAPosition new_dfa_pos: path.dfa_pos.branch(D)) {
				// Skip
				FancyPath new_path(path.suf_pos, new_dfa_pos);
				new_path.add_skip();
				new_path.text += new_dfa_pos.prev_char;
				new_set.push_back(new_path);
			}
		}
		
		working_set = new_set;

		// kill paths with too much errors or dead states
		for (auto it=working_set.begin(); it!=working_set.end(); ++it) {
			if ((! q.valid(*it)) || D.is_dead((*it).dfa_pos.state)) {
				// delete!
				working_set.erase(it);
				it--;
			}
		}

		// insert final paths into result
		for (FancyPath& path: working_set) {
			if (D.is_final(path.dfa_pos.state)) {
				result.push_back(path);
			}
		}
		
		// DEBUG ATTENTION remove me
		run++;
		if (run > limit) break;
	}
	
	return result;
}


int main() {
	s_DFA D( {"q0", "q1", "q2", "dead"}, {'T', 'A'}, 0, {2});
	D.set_delta(0, 'T', 1);
	D.set_delta(0, 'A', 3);

	D.set_delta(1, 'T', 2);
	D.set_delta(1, 'A', 1);

	D.set_delta(2, 'T', 2);  // Final stays final
	D.set_delta(2, 'A', 2);

	D.set_delta(3, 'T', 3);
	D.set_delta(3, 'A', 3);  // Dead stays dead

	CompactDFA CD(D);

	std::string s = "TAAATTAATATT";
	Suffix3 suf;
	suf.s = s;
	suf.build();
	
	Query q(1, 0, 0, 0, 1);
	std::vector<FancyPath> paths = fucking_fancy_search(suf, CD, q);
	
	for (auto p: paths) {
		std::cout << p << "\n";
	}
	
}
