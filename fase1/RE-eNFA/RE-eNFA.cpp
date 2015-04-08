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
	string new_state1,new_state2, new_state3,alphabet, new_begin_state;
	int temp_state, beginstate;
	string::iterator it_temp;
	bool next = true;
	bool endstate = false;
	bool start = false;
	int count = 0;
	int states = 1;
	int current_start_state = N.ID("q0");	//Houdt de ID van de staat bij waar je moet beginnen
	int current_end_state = -100;				//Houdt de ID van de staat bij waar je eindigt
	for (string::iterator it = str.begin(); it!= str.end(); it++){
		if (*it == '('){
			next = false;
			begin.push(current_start_state);
			continue;
		}
		if (*it == ')'){
			current_start_state = brackets.top();
			beginstate = begin.top();
			next = true;
			count=0;
			brackets.pop();
			begin.pop();
			end.pop();
			continue;
		}
		if (*it == '+'){
			//Hier moet de juiste staat terug gezet worden
			current_start_state = begin.top();
			current_end_state = brackets.top();
			end.push(current_end_state);
			next = false;
			count = 0;
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
			it_temp = it + 1;
			N.add_state(new_state3 ,false);
			temp = N.delta(current_end_state, 'e');
			temp.insert(N.ID(new_state3));
			N.set_delta(current_end_state, 'e',temp);
			
			//Eerste epsilon pijl
			new_begin_state = "q" + to_string(states);
			N.add_state(new_begin_state, false);
			states++;
			if (start == false){
				N.q0 = N.ID(new_begin_state);
				start = true;
			}
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
				alphabet.push_back(*it);
			}
			if (begin.empty() and start == false){
				N.q0 = N.ID(N.realState(current_start_state));
				start = true;
			}
			new_state1 = "q" + to_string(states);
			N.add_state(new_state1 ,false);
			states++;
			new_state2 = "q" + to_string(states);
			N.add_state(new_state2 ,false);
			states++;
			it_temp = it+1;
			if (*it_temp == '+' or *it_temp == ')' or *it_temp == '*'  or *it_temp == '('){
				if (current_end_state == -100 or next == true){
				new_state3 = "q" + to_string(states);
				N.add_state(new_state3 ,false);
				current_end_state = N.ID(new_state3);
				brackets.push(current_end_state);
				states++;
				}
				set <int> temp;
				if (count == 0){
					temp = N.delta(current_start_state,'e');
					temp.insert(N.ID(new_state1));
					N.set_delta(current_start_state, 'e', temp);
				}
				if (count != 0){
					temp = N.delta(temp_state,'e');
					temp.insert(N.ID(new_state1));
					N.set_delta(temp_state, 'e', temp);
				}
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
			else{
				set <int> temp;
				if (count == 0){
					temp = N.delta(current_start_state,'e');
					temp.insert(N.ID(new_state1));
					N.set_delta(current_start_state, 'e', temp);
				}
				if (count != 0){
					temp = N.delta(temp_state,'e');
					temp.insert(N.ID(new_state1));
					N.set_delta(temp_state, 'e', temp);
				}
				temp = N.delta(N.ID(new_state1),*it);
				temp.insert(N.ID(new_state2));
				N.set_delta(N.ID(new_state1), *it , temp);
				temp = N.delta(N.ID(new_state2),'e');
				new_state3 = "q" + to_string(states);
				N.add_state(new_state3 ,false);
				states++;
				temp.insert(N.ID(new_state3));
				N.set_delta(N.ID(new_state2), 'e' , temp);
				temp_state = N.ID(new_state3);
				count++;
			}
		}
	}
//	cout << alphabet << endl;
	bool transitions = true;
	for (int i = 0; i < N.num_states; i++){
		set <int> temp;
		temp = N.delta(i, 'e');
		if (temp.empty() == true){
			int count_empty=0;
			for ( string::iterator it = alphabet.begin(); it != alphabet.end(); it++){
				temp = N.delta(i, *it);
				if (temp.empty()){
					count_empty++;
					continue;
				}
				break;
			}
			
			if (count_empty == alphabet.size()){
				N.F.clear();
				N.F.insert(i);
				break;
			}
			continue;
		}
		continue;
	}
	cout << "Het aantal staten = " << states << endl;

	//Geeft overeenkomende eNFA terug
	return N;
}

int countOccurences(std::string str){
	int counter=0;
	for (string::iterator it = str.begin(); it!= str.end(); it++){
		if (*it == '+' or *it == '(' or *it == ')'){
			continue;
		}
		counter++;
	}
	return counter;
}
