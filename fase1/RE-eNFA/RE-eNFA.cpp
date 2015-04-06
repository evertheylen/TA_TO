#include "RE-eNFA.h"
#include "FSM.h"

#include <string>
#include <stack>
#include <iostream>
#include <fstream>
#include <set>
using namespace std;

s_eNFA RE_to_eNFA(std::string& str) {
	s_eNFA N({"q0"},				//staten
			{'e'},					//alfabet
			0, {0});				//from -> to
	stack <int> brackets;			//Helpt bij het behouden startstaat, dus juiste beginpunt
	int states=1;
	int current_start_state = N.ID("q0"); //Houdt de ID van de staat bij waar je moet beginnen
	int current_end_state;
	cout << "Converting the regex string" << endl;
	for (string::iterator it = str.begin(); it!= str.end(); it++){
		cout << *it << endl;
		if (*it == '('){
			brackets.push(current_start_state);
			continue;
		}
		if (*it == ')'){
			current_start_state = brackets.top();
			brackets.pop();
			continue;
		}
		if (*it == '+'){
			//Hier moet de juiste staat terug gezet worden
			
			continue;
		}
		if (*it == '*'){
			//Random e pijlen toevoegen
			continue;
		}
		else{
			//Gaat na of het element in het alfabet zit, zo niet voeg toe
			if (N.isInSigma(*it) == false){
				cout << "Inserting in alphabet: " <<  *it << endl;
				N.sigma.insert(*it);
			}
			string new_state1 = "q" + to_string(states);
			N.add_state(new_state1 ,false);
			states++;
			string new_state2 = "q" + to_string(states);
			N.add_state(new_state2 ,false);
			states++;
			string new_state3 = "q" + to_string(states);
			N.add_state(new_state3 ,false);
			states++;
			set<int> temp = N.delta(current_start_state,'e');
			temp.insert(N.ID(new_state1));
			N.set_delta(current_start_state, 'e', temp);
			temp = N.delta(N.ID(new_state1),*it);
			temp.insert(N.ID(new_state2));
			N.set_delta(N.ID(new_state1), *it , temp);
			temp = N.delta(N.ID(new_state2),'e');
			temp.insert(current_end_state);
			N.set_delta(N.ID(new_state2), 'e' , temp);
		}
	}
	cout << "Het aantal staten = " << states << endl;
	
	//Tijdelijk printen naar dot
	std::ofstream test;
	test.open("RE_eNFA.dot");
	N.to_dot(test);
	test.close();
	
	//Geeft overeenkomende eNFA terug
	return N;
}
