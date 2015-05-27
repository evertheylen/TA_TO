
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

#include "search.h"


int main() {
	s_DFA D( {"q0", "q1", "q2", "dead"}, {'T', 'A'}, 0, {2});
	D.set_delta(0, 'T', 1);
	D.set_delta(0, 'A', 3);

	D.set_delta(1, 'T', 2);
	D.set_delta(1, 'A', 1);

	D.set_delta(2, 'T', 3);
	D.set_delta(2, 'A', 3);

	D.set_delta(3, 'T', 3);
	D.set_delta(3, 'A', 3);  // Dead stays dead

	CompactDFA CD(D);

	std::string s = "TAAATTAATATT$";
	Suffix3 suf;
	suf.s = s;
	suf.build();
	
	Query q(CD,2,2,2,2,2);
	std::vector<FancyPath> paths = q.search(suf);
	
	std::cout << "\n----[ Results ]-----\n";
	
	for (auto p: paths) {
		p.print(std::cout, suf);
	}
	
	std::cout << "\nroot of suffix3: " << suf.root << "\n";
	
	s_DFA_Runner r(D);
	std::cout << "TAAT? " << r.process("TAAT") << "\n";
	std::cout << "TAAAAAAT? " << r.process("TAAAAAAT") << "\n";
	std::cout << "TT? " << r.process("TT") << "\n";
	
	std::cout << sizeof(FancyPath) << " <<-\n";
	
	generate_dot(suf, "Tadaa", 0);
}
