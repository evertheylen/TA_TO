

#include "search.h"
#include <sstream>
#include "../etc/fasta.h"

#include "../engine_r/eNFA-DFA/eNFA-DFA.h"
#include "../engine_r/Product/product.h"
#include "../engine_r/RE-eNFA/RE-eNFA.h"
#include "../engine_r/TFA/TFA.h"
#include "../engine_r/extra.h"

#define uint unsigned int

// ----[ FChar ]-------------

void html_open_status(Status s, std::ostream& out) {
	switch (s) {
		case Status::Fake:
			out << "<font color='#FF9022'>";  // orange
			break;
		case Status::Skip:
			out << "<font color='#0050FF'>";  // blue
			break;
		case Status::Repetition:
			out << "<font color='#33FF33'>";  // green
			break;
		case Status::Ignore:
			out << "<font color='#FF3333'>";  // red
			break;
	} // Perfects aren't colored
}

void html_close_status(Status s, std::ostream& out) {
	if (s != Status::Perfect) {
		out << "</font>";
	}
}

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

SuffixPosition::SuffixPosition(uint _node, int _pos_in_node):
	node(_node), pos_in_node(_pos_in_node) {}

std::vector<SuffixPosition> SuffixPosition::branch(Suffix3& suf) {
	if (int(pos_in_node) < (int(suf.data[node].end) - int(suf.data[node].start)-1)) {
		std::cout << "(branching suffix to next position in node " << node << ")\n";
		std::cout << "    (pos_in_node=" << pos_in_node << ", start=" << suf.data[node].start << ", end=" << suf.data[node].end << "\n";
		return {SuffixPosition(node, pos_in_node+1)};
	}

	std::vector<SuffixPosition> result;

	for (uint child_i=suf.data[node].leftmost_child; child_i != 0; child_i = suf.data[child_i].right_brother) {
		std::cout << "(branching suffix to new node: " << child_i << " = "
				<< suf.s.substr(suf.data[child_i].start, suf.data[child_i].end-suf.data[child_i].start) << ")\n";
		SuffixPosition extra(child_i, 0);
		result.push_back(extra);
	}
	
	return result;
}

// void SuffixPosition::print(std::ostream& out,  Suffix3& tree) {
// 	for (Node3* parent: parents) {
// 		out << tree.s.substr(parent->start, parent->end - parent->start) << ".";
// 	}
// 	out << tree.s.substr(node->start, pos_in_node+1);
// }



// -----[ DFAPosition ]---------
// For more advanced positions in the DFA, obviously

DFAPosition::DFAPosition(int s, char c):
		state(s), prev_char(c) {}
	
	// Never branches to dead states!
std::vector<DFAPosition> DFAPosition::branch(CompactDFA& D) {
	std::vector<DFAPosition> result;
	for (int i_a=0; i_a<D.sigma.size(); i_a++) {
		if (!D.is_dead(D.d_data.at(state).at(i_a))) {
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
	//suf_pos.print(out, tree);
	out << ", ";
	out << *this;
}



// -----[ Match ]-------------

Match::Match(FancyPath& p, Suffix3& suf):
		FancyPath(p) {
	// Save locations
	std::cout << "match on node " << p.suf_pos.node << "\n";
	suf.get_leaves(p.suf_pos.node, locations);
// 	for (int i=0; i<locations.size(); i++) {
// 		locations[i] += p.suf_pos.pos_in_node;
// 	}
}

std::string Match::format(File& file) {
	std::stringstream firstline;  // FASTA
	std::stringstream secondline; // pattern
	
	int fasta_i = locations.at(0);  // if there's no match, something serious is wrong
	Status current = Status::Perfect;
	
	for (FChar& fc: text) {
		// print fasta character, unless it's a skip
		if (fc.s != Status::Skip) {
			firstline << file.suffixtree->s[fasta_i];
			fasta_i++;
		} else {
			firstline << '.'; // space gives problems with truncating
		}
		
		if (fc.s != current) {
			html_close_status(current, secondline);
			current = fc.s;
			html_open_status(current, secondline);
		}
		secondline << fc.c;
	}
	html_close_status(current, secondline);
	
	//merge the two lines
	firstline << "\n<br/>";
	firstline << secondline.str();
	return std::string(firstline.str());
	
// 	std::cout << "node address: " << suf_pos.node << "\n";
// 	std::cout << "first: " << firstline.str() << "\n";
// 	std::cout << "secnd: " << secondline.str() << "\n";
// 	std::cout << "locations: ";
// 	for (int& l: locations) {
// 		std::cout << l << ", ";
// 	}
// 	std::cout << "\n";
}




// -----[ Result]---------------

Result::Result(std::vector<FancyPath>& paths, File* f, Query* q):
			file(f), query(q) {
	for (FancyPath& fp: paths) {
// 		std::cout << "Adding path:\n";
// 		fp.print(std::cout, *(file.suffixtree));
		matches.push_back(Match(fp, *(file->suffixtree)));
	}
}

Result::Result() {}

std::string Result::summary() {
	int total_matched_loc = 0;
	for (Match& m: matches) {
		total_matched_loc += m.locations.size();
	}
	if (total_matched_loc == 0) {
		return "no matches";
	} else if (total_matched_loc == 1) {
		return "1 match";
	} else {
		return std::to_string(total_matched_loc) + " matches";
	}
}





// -----[ Query ]----------------

Query::Query(std::string& fancypattern, int f, int s, int r, int i, int m):
		max_fakes(f), max_skips(s), max_reps(r), max_ignores(i), max_total(m), input(fancypattern) {
	// All of the theory and algorithms have led up to this:
	// Converting the fancypattern to a single DFA.
	std::vector<std::string> pats = fastaReplace(fancypattern);
// 	std::cout << "Query about to create DFA\n";
	if (pats.size() == 1) {
		auto DFA = to_DFA<std::string, char, 'e'>(pats[0]);
		
		
// 		std::cout << "####### DFA ########\n";
// 		for (int s=0; s<DFA.num_states; s++) {
// 			for (char c: DFA.sigma) {
// 				std::cout << "delta[" << s << "][" << c << "] = " << DFA.d_data.at(s).at(c) << "\n";
// 			}
// 		}
// 		std::cout << "###################\n";
		
		D = CompactDFA(DFA);
	} else {
		// at least two DFA's
		s_DFA currentD = product<std::string, char>(to_DFA<std::string, char, 'e'>(pats[0]),
													to_DFA<std::string, char, 'e'>(pats[1]), true);
		s_DFA newD;
		for (int i=2; i<pats.size(); i++) {
			newD = to_DFA<std::string, char, 'e'>(pats[i]);
			currentD = product<std::string, char>(currentD, newD, true);
		}
		D = CompactDFA(currentD);
	}
	
	// DFA Table
// 	std::cout << "###################\n";
// 	for (int s=0; s<D.num_states; s++) {
// 		for (int ia=0; ia<D.sigma.size(); ia++) {
// 			std::cout << "delta[" << s << "][" << D.sigma[ia] << "] = " << D.d_data.at(s).at(ia) << "\n";
// 		}
// 	}
// 	std::cout << "###################\n";
}

void Query::search(File* f) {
	if (results_per_file.find(f->ID) == results_per_file.end()) {
		std::cout << "adding results\n";
		auto raw_results = real_search(*f->suffixtree);
		results_per_file.insert(std::pair<int, Result>(f->ID, Result(raw_results, f, this)));
	}
}


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
std::vector<FancyPath> Query::real_search(Suffix3& suf) {
	std::vector<FancyPath> result;
	
	std::vector<FancyPath> working_set = {FancyPath(SuffixPosition(0, 0), DFAPosition(D.q0, '\0'))};
	
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
							new_path.add_fake(new_dfa_pos.prev_char);
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
