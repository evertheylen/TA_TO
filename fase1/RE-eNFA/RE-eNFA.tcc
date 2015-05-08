#include "FSM.h"

#include <string>
#include <stack>
#include <iostream>
#include <fstream>
#include <set>
using namespace std;

template<	typename StateT,
			typename SymbolT,
			SymbolT epsilon>
eNFA<StateT, SymbolT, epsilon> RE_to_eNFA(string& str) {
	eNFA <StateT, SymbolT, epsilon> N({"q0"},				/** Staten (met initieel de startstaat)*/
			{'e'},					/**alfabet (met standaard de epsilon)*/
			0, {0});				/**Transities van een staat naar een verzameling van staten*/

	string alphabet;								/**Vergemakkelijkt het vinden van de eindstaat*/
	int currentstate = N.ID("q0");	/** Houdt de huidige staat bij*/
	int states = 1;									/**Counter van de staten */
	stack<int> start,end; 					/**Startstaat,eindstaat bijhouden*/
	for (string::iterator it = str.begin(); it!= str.end(); it++){
		std::cout << *it << std::endl;
		if (*it == '('){
			// start.push(start.top());
			// end.push(currentstate);
			continue;
		}
		if (*it == ')'){
			// end.push(currentstate);
			continue;
		}
		if (*it == '+'){
			cout << "Popping the start state from the start stack\n";
			end.push(currentstate);
			currentstate = start.top();
			start.pop();
			continue;
		}
		if (*it == '*'){
			if(*(it-1) != ')'){
				set<int> tempTransitions;
				//Laatste epsilon pijl
				string new_end = 'q' + to_string(states);
				N.add_state(new_end,false);
				states++;
				tempTransitions = N.delta(currentstate,'e');
				tempTransitions.insert(N.ID(new_end));
				N.set_delta(currentstate, 'e', tempTransitions);
				tempTransitions.clear();

				//Wederkerende epsilon pijl
				tempTransitions = N.delta(currentstate,'e');
				tempTransitions.insert(start.top());
				N.set_delta(currentstate, 'e', tempTransitions);
				currentstate = N.ID(new_end);
				tempTransitions.clear();

				//Eerste epsilon pijl
				string afterbeginstate = 'q' + to_string(states);
				N.add_state(afterbeginstate, false);
				states++;
				tempTransitions = N.delta(start.top(),'e');
				N.set_delta(N.ID(afterbeginstate),'e',tempTransitions);
				tempTransitions.clear();
				tempTransitions.insert(N.ID(afterbeginstate));
				tempTransitions.insert(currentstate);
				N.set_delta(start.top(),'e',tempTransitions);
				tempTransitions.clear();
			}
			//Hier komt het andere geval namelijk * na de haakjes
			
			continue;
		}
		if(*it == '.'){
			continue;
		}
		else{
			//Gaat na of het element in het alfabet zit, zo niet voeg toe
			if (N.isInSigma(*it) == false){
				N.sigma.insert(*it);
				alphabet.push_back(*it);
			}
			//Label:Eerste staat
			string new_state1 = "q" + to_string(states);
			N.add_state(new_state1, false);
			states++;
			//Label:Tweede staat
			string new_state2 = "q" + to_string(states);
			N.add_state(new_state2, false);
			states++;

			//Toevoegen standaard geval: Namelijk eerst epsilon dan *it en dan epsilon
			set<int> tempTransitions;
			start.push(currentstate);
			tempTransitions = N.delta(currentstate, 'e');
			tempTransitions.insert(N.ID(new_state1));
			N.set_delta(currentstate, 'e', tempTransitions);
			currentstate = N.ID(new_state1);
			tempTransitions.clear();

			tempTransitions = N.delta(currentstate, *it);
			tempTransitions.insert(N.ID(new_state2));
			N.set_delta(currentstate, *it , tempTransitions);
			currentstate = N.ID(new_state2);
			tempTransitions.clear();

			tempTransitions = N.delta(currentstate, 'e');
			if (end.size() == 0){
				//Label:Derde staat
				string new_state3 = "q" + to_string(states);
				N.add_state(new_state3, false);
				states++;
				tempTransitions.insert(N.ID(new_state3));
				N.set_delta(currentstate, 'e' , tempTransitions);
				currentstate = N.ID(new_state3);
				tempTransitions.clear();
			}
			else{
				if(*(it+1) == '.'){
					string new_state3 = "q" + to_string(states);
					N.add_state(new_state3, false);
					states++;
					tempTransitions.insert(N.ID(new_state3));
					N.set_delta(currentstate, 'e' , tempTransitions);
					currentstate = N.ID(new_state3);
					tempTransitions.clear();
				}
				else{
					tempTransitions.insert(end.top());
					N.set_delta(currentstate,'e',tempTransitions);
					currentstate = end.top();
					end.pop();
					tempTransitions.clear();
				}
			}
		}
	}

	/**
	*Zoekt de eindstaat(De staat waar geen transities uit vertrekken) en maakt deze staat een eindstaat dmv een dubbele cirkel
	*/
	for (int i = 0; i < N.num_states; i++){
		//std::cout << "Finding end state" << std::endl;
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
				//Element heeft geen uitgaande transities ==> Eindstaat
				N.F.clear();
				N.F.insert(i);
				break;
			}
			continue;
		}
		continue;
	}

	/**
	*  Geeft overeenkomende eNFA(N) terug
	*/
	return N;
}
