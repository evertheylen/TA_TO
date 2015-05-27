

#include "search.h"

// ----[ FChar ]-------------

FChar::FChar(char _c, Status _s):
	c(_c), s(_s) {}

std::ostream& operator<<(std::ostream& out, FChar fc) {
	out << fc.c;
	return out;
}

std::ostream& operator<<(std::ostream& out, std::vector<FChar>& v) {
	for (FChar fc: v) {
		out << fc;
	}
	return out;
}



// -----[ SuffixPosition ]---------
// For more advanced suffix tree states

SuffixPosition::SuffixPosition(Node3* _node, int _pos_in_node, std::vector<Node3*> _parents):
	node(_node), pos_in_node(_pos_in_node), parents(_parents) {}

std::vector<SuffixPosition> SuffixPosition::branch(Suffix3& suf) {
	if (pos_in_node < (node->end - node->start - 1)) {
		//std::cout << "(branching suffix to next position in suffix)\n";
		return {SuffixPosition(node, pos_in_node+1, parents)};
	}

	std::vector<SuffixPosition> result;
	for (Node3* child: node->children) {
		//std::cout << "(branching suffix to new node: " << child << " = " << suf.s.substr(child->start, child->end-child->start) << ")\n";
		SuffixPosition extra(child, 0, parents);
		extra.parents.push_back(node);
		result.push_back(extra);
	}
	return result;
}

void SuffixPosition::print(std::ostream& out,  Suffix3& tree) {
	for (Node3* parent: parents) {
		out << tree.s.substr(parent->start, parent->end - parent->start) << ".";
	}
	out << tree.s.substr(node->start, pos_in_node+1);
}



// -----[ DFAPosition ]---------
// For more advanced positions in the DFA, obviously

DFAPosition::DFAPosition(int s, char c):
		state(s), prev_char(c) {}
	
	// Never branches to dead states!
std::vector<DFAPosition> DFAPosition::branch(CompactDFA& D) {
	std::vector<DFAPosition> result;
	for (int i_a=0; i_a<D.sigma.size(); i_a++) {
		if (!D.is_dead(D.d_data[state][i_a])) {
			result.emplace_back(DFAPosition(D.d_data[state][i_a], D.sigma[i_a]));
		}
	}
	return result;
}



// ------[ FancyPath ]------------

FancyPath::FancyPath(SuffixPosition s, DFAPosition d, FancyPath& old):
		suf_pos(s), dfa_pos(d),
		fakes(old.fakes), skips(old.skips), reps(old.reps), ignores(old.ignores),
		text(old.text) {}
			
FancyPath::FancyPath(SuffixPosition s, DFAPosition d):
		suf_pos(s), dfa_pos(d),
		fakes(0), skips(0), reps(0), ignores(0) {}

void FancyPath::add_perfect(char c) {
	text.push_back(FChar(c, Status::Perfect));
}

void FancyPath::add_fake(char c) {
	text.push_back(FChar(c, Status::Fake));
	fakes++;
}

void FancyPath::add_skip(char c) {
	text.push_back(FChar(c, Status::Skip));
	skips++;
}

void FancyPath::add_rep(char c) {
	text.push_back(FChar(c, Status::Repetition));
	reps++;
}

void FancyPath::add_ignore(char c) {
	text.push_back(FChar(c, Status::Ignore));
	ignores++;
}

int FancyPath::get_total_errors() {
	return fakes+skips+reps+ignores;
}

std::ostream& operator<<(std::ostream& out, FancyPath& p) {
	out << "Text is: " << p.text << ", DFA state: " << p.dfa_pos.state << ", total errors: " << p.get_total_errors() << "\n";
	return out;
}

// More info than operator<<
void FancyPath::print(std::ostream& out, Suffix3& tree) {
	out << "Suffix string: ";
	suf_pos.print(out, tree);
	out << ", ";
	out << *this;
}



// -----[ Query ]----------------

Query::Query(CompactDFA _D, int f, int s, int r, int i, int m):
		D(_D), max_fakes(f), max_skips(s), max_reps(r), max_ignores(i), max_total(m) {}


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


// The one and only search
std::vector<FancyPath> Query::search(Suffix3& suf) {
	std::vector<FancyPath> result;

	std::vector<FancyPath> working_set = {FancyPath(SuffixPosition(suf.root, 0), DFAPosition(D.q0, '\0'))};
	
	while (working_set.size() > 0) {
		std::vector<FancyPath> new_set;
		
		//std::cout << "\n------------------\nstarting while with working_set:\n";
		//for (auto p: working_set) {
		//	p.print(std::cout, suf);
		//}
		
		for (auto it=working_set.begin(); it!=working_set.end(); ++it) {
			FancyPath& path = *it;
			
			bool can_add = path.get_total_errors() < max_total;
			bool can_add_fake = can_add && (path.fakes < max_fakes);
			bool can_add_skip = can_add && (path.skips < max_skips);
			bool can_add_rep = can_add && (path.reps < max_reps);
			bool can_add_ignore = can_add && (path.ignores < max_ignores);
			
			// ___ Suffix advance ___
			for (SuffixPosition new_suf_pos: path.suf_pos.branch(suf)) {
				char new_char = suf.get(new_suf_pos);
				//std::cout << " --> new char is " << new_char << "\n";
				
				// ___ Suffix advance, DFA advance (never to dead states) ___
				for (DFAPosition new_dfa_pos: path.dfa_pos.branch(D)) {
					//std::cout << "   --> branching DFA to new one with state " << new_dfa_pos.state << " and prev_char " << new_dfa_pos.prev_char << "\n";
					// Perfect or Fake?
					FancyPath new_path(new_suf_pos, new_dfa_pos, path);
					if (new_dfa_pos.prev_char != new_char) {
						// Fake character!
						if (can_add_fake) {
							new_path.add_fake(new_char);
							new_set.push_back(new_path);
						}
					} else {
						new_path.add_perfect(new_dfa_pos.prev_char);
						new_set.push_back(new_path);
					}
				}
				
				// ___ Suffix advance, DFA stay ___
				FancyPath new_path(new_suf_pos, path.dfa_pos, path);
				if (can_add_rep && new_char == path.dfa_pos.prev_char) {
					// Repetition
					new_path.add_rep(new_char);
					new_set.push_back(new_path);
				} else if (can_add_ignore) {
					// Ignore Suffix input completely
					new_path.add_ignore(new_char);
					new_set.push_back(new_path);
				}
			}
			
			// ___ Suffix stay, DFA advance ___
			if (can_add_skip) for (DFAPosition new_dfa_pos: path.dfa_pos.branch(D)) {
				// Skip
				FancyPath new_path(path.suf_pos, new_dfa_pos, path);
				new_path.add_skip(new_dfa_pos.prev_char);
				new_set.push_back(new_path);
			}
		}
		
		working_set = new_set;

		// kill paths with too much errors or dead states --> Not needed anymore! yay!

		// insert final paths into result
		for (FancyPath& path: working_set) {
			if (D.is_final(path.dfa_pos.state)) {
				result.push_back(path);
			}
		}
	}
	
	return result;
}
