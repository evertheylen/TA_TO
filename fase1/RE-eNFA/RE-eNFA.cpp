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
	stack <int> brackets;			//Helpt bij het behouden endstaat, dus juiste beginpunt
	stack <int> begin;
	stack <int> end;
	string new_state1,new_state2, new_state3;
	string::iterator it_temp;
	//cout << countOccurences(str) << "Het aantal tekens van het alfabet" << endl;
	bool next = true;
	int states=1;
	int current_start_state = N.ID("q0");	//Houdt de ID van de staat bij waar je moet beginnen
	int current_end_state = -100;				//Houdt de ID van de staat bij waar je eindigt
	int beginstate;
	for (string::iterator it = str.begin(); it!= str.end(); it++){
		cout << *it << endl;
		if (*it == '('){
			next = false;
			begin.push(current_start_state);
			continue;
		}
		if (*it == ')'){
			current_start_state = brackets.top();
			beginstate = begin.top();
			next = true;
			brackets.pop();
			begin.pop();
			continue;
		}
		if (*it == '+'){
			//Hier moet de juiste staat terug gezet worden
			current_start_state = begin.top();
			current_end_state = brackets.top();
			end.push(current_end_state);
			next = false;
			continue;
		}
		if (*it == '*'){
			//Teruggaande epsilon pijlen
			set<int> temp = N.delta(current_end_state, 'e');
			temp.insert(beginstate);
			N.set_delta(current_end_state, 'e',temp);
			
			
			//Laatste epsilon pijl
			new_state3 = "q" + to_string(states);
			states++;
			N.add_state(new_state3 ,false);
			temp = N.delta(current_end_state, 'e');
			temp.insert(N.ID(new_state3));
			N.set_delta(current_end_state, 'e',temp);
			
			//Eerste epsilon pijl
			string new_begin_state = "q" + to_string(states);
			N.add_state(new_begin_state, false);
			states++;
			
			temp = N.delta(N.ID(new_begin_state), 'e');
			temp.insert(beginstate);
			temp.insert(current_end_state);
			N.set_delta(N.ID(new_begin_state),'e', temp);
			current_start_state = N.ID(new_state3);
			continue;
		}
		else{
			//Gaat na of het element in het alfabet zit, zo niet voeg toe
			if (N.isInSigma(*it) == false){
				cout << "Inserting in alphabet: " <<  *it << endl;
				N.sigma.insert(*it);
			}
			new_state1 = "q" + to_string(states);
			N.add_state(new_state1 ,false);
			states++;
			new_state2 = "q" + to_string(states);
			N.add_state(new_state2 ,false);
			states++;
			if (current_end_state == -100 or next == true){
				new_state3 = "q" + to_string(states);
				N.add_state(new_state3 ,false);
				current_end_state = N.ID(new_state3);
				brackets.push(current_end_state);
				states++;
			}
//			it_temp = it + 1;
//			while(*it_temp!='+' or *it_temp!=')' or *it_temp!='(' or *it_temp!='*'){
//				string temp1,temp2,temp3;
//				set <int> temp_delta;
//				temp1 = "q" + to_string(states);
//				N.add_state(temp1 ,false);
//				states++;
//				temp2 = "q" + to_string(states);
//				N.add_state(temp2 ,false);
//				states++;
//				temp3 = "q" + to_string(states);
//				N.add_state(temp3 ,false);
//				states++;
//				temp_delta = N.delta(N.ID(temp1),'e');
//				temp_delta.insert(N.ID(temp1));
//				N.set_delta(N.ID(temp1), 'e', temp_delta);
//				temp_delta = N.delta(N.ID(temp2),'e');
//				temp_delta.insert(N.ID(temp2));
//				N.set_delta(N.ID(temp2), 'e', temp_delta);
//				temp_delta = N.delta(N.ID(temp3),'e');
//				temp_delta.insert(N.ID(temp3));
//				N.set_delta(N.ID(temp3), 'e', temp_delta);
//				it_temp++;
//			}
//			it = it_temp;
			set<int> temp = N.delta(current_start_state,'e');
			temp.insert(N.ID(new_state1));
			N.set_delta(current_start_state, 'e', temp);
			temp = N.delta(N.ID(new_state1),*it);
			temp.insert(N.ID(new_state2));
			N.set_delta(N.ID(new_state1), *it , temp);
			temp = N.delta(N.ID(new_state2),'e');
			temp.insert(current_end_state);
			N.set_delta(N.ID(new_state2), 'e' , temp);
			if (next == true){
				current_start_state = current_end_state;
				current_end_state = -100;
				continue;
			}
		}
	}
	cout << "Het aantal staten = " << states << endl;

	//Geeft overeenkomende eNFA terug
	return N;
}

int countOccurences(std::string str){
	int counter=0;
	for (string::iterator it = str.begin(); it!= str.end(); it++){
		if (*it == '+' or *it == '(' or *it == ')' or  *it == '*'){
			continue;
		}
		counter++;
	}
	return counter;
}
