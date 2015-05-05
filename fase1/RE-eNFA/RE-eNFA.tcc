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

	stack <int> brackets;			//Helpt bij het behouden endstaat, dus juiste beginpunt
	stack <int> begin;
	stack <int> end;
	string new_state1,new_state2, new_state3,alphabet, new_begin_state;
	int temp_state, beginstate;
	string::iterator it_temp;
	bool next = true;
	bool endstate = false;
	bool start = false;
	bool noPlus=true;
	int count = 0;
	int states = 1;
	//std::cout << "next" << std::endl;
	int current_start_state = N.ID("q0");	//Houdt de ID van de staat bij waar je moet beginnen
	int current_end_state = -100;				//Houdt de ID van de staat bij waar je eindigt
	for (string::iterator it = str.begin(); it!= str.end(); it++){
		//std::cout << *it << std::endl;
		if (*it == '('){
			string::iterator countplus=it+1;
			while(*countplus != ')'){
				if (*countplus == '+'){
					noPlus = false;
				}
				countplus++;
			}
			next = false;
			begin.push(current_start_state);
			continue;
		}
		if (*it == ')'){
			//std::cout << "Closing bracket"<< brackets.size() << std::endl;
			current_start_state = brackets.top();
			beginstate = begin.top();
			//std::cout << "Closing bracket2" << std::endl;
			next = true;
			count=0;
			if (brackets.size() !=0){
				brackets.pop();
			}
			if(begin.size() != 0){
				begin.pop();
			}
			if(end.size() != 0){
				end.pop();
			}
			//Kleine verandering, als ergens start fout geplaatst wordt zie hier
			start = true;
			//std::cout << "Closing bracket3" << std::endl;
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
				//cout << "Inserting in alphabet: " <<  *it << endl;
				N.sigma.insert(*it);
				alphabet.push_back(*it);
			}
			if (begin.empty() and start == false){
				//std::cout << "find it" << std::endl;
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
			if ((*it_temp == '+' or *it_temp == ')' or *it_temp == '*'  or *it_temp == '(') and noPlus==false){
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
					//std::cout << "count=0" << std::endl;
					temp = N.delta(current_start_state,'e');
					temp.insert(N.ID(new_state1));
					N.set_delta(current_start_state, 'e', temp);
				}
				if (count != 0){
					//std::cout << "count!=0" << std::endl;
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
				if (noPlus == true and *it_temp == ')'){
					if(brackets.size() != 0){
						brackets.pop();
					}
					brackets.push(N.ID(new_state3));
				}
			}
		}
	}

	/**
	*Zoekt de eindstaat(De staat waar geen transities uit vertrekken) en maakt deze staat een eindstaat dmv een dubbele cirkel
	*/
	bool transitions = true;
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
