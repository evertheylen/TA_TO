#include "RE-eNFA.h"
#include "../lib/FSM/FSM.h"

#include <string>
#include <stack>
#include <iostream>
using namespace std;

s_eNFA RE_to_eNFA(std::string& str) {
	s_eNFA N({"q0"},//states
			{'e'},		//alphabet
			0, {0});//from -> to
	stack <char> brackets;
	int states=1;
	for (string::iterator it = str.begin(); it!= str.end(); ++it){
		cout << *it << endl;
		if (*it == '('){
			brackets.push(*it);
			continue;
		}
		if (*it == ')'){
			brackets.pop();
			continue;
		}
		if (*it == '+'){
			continue;
		}
		if (*it == '*'){
			continue;
		}
		else{
			N.sigma.insert(*it);
			string new_state = "q" + states;
			states++;
			int temp_state;
			temp_state = N.add_state(new_state, false);
			N.set_delta(states-1,'e',N.delta(states-1,'e'));
		
		}
	}
	
	
	
	return N;
}
