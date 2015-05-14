#include "TFA.h"
#include <vector>
#include <iostream>
using namespace std;

bool block::operator==(block const& b){
    if(b.state1 == this->state1 and b.state2 == this->state2){
        return true;
    }
    else return false;
}

bool block::hasFinal(s_DFA D){
    if(D.isFinal(state1) == true xor D.isFinal(state2) == true) return true;
    else return false;
}

vector<block> makeTable(s_DFA D){
    vector<block> table_vec;
    for(int e=0; e <= D.num_states+1;e++){
        for(int i=0; i<= D.num_states+1;i++){
            block tempblock(e,i);
            if(tempblock.hasFinal(D) == true) tempblock.dist = true;
            for(int s = 0; s < table_vec.size(); s++){
                if(table_vec.at(s) == tempblock) continue;
                else if(i == e) continue;
                else table_vec.push_back(tempblock);
            }
        }
    }
    return table_vec;
}
bool areIdentical(bool a, bool b){
    if(a == false and b == false) return true;
    else if(a == true and b ==true) return true;
    else return false;
}

void makePairs(s_DFA D, vector<block> table_vec){
    for(vector<block>::iterator it = table_vec.begin(); it != table_vec.end(); ++it) {
        for(char a: D.sigma){
            int nextState1 = D.delta(it->state1,a);
            int nextState2 = D.delta(it->state2,a);/*
            if(nextstate1 == nextState2){
                it->dist = true;
                break;
            }*/
            if(areIdentical(D.isFinal(nextState1), D.isFinal(nextState2)) == false){
                it->dist = true;
                break;
            }
        }
    }
}

vector<block> removeDist(vector<block> table_vec){
    vector<block> new_vector;
    for(vector<block>::iterator it = table_vec.begin(); it != table_vec.end(); ++it){
        if(it->dist == false){
            new_vector.push_back(*it);
        }
    }
    return new_vector;
}

s_DFA makeNewDFA(s_DFA D,vector<block> table){
    s_DFA mini;
    for(int i=0; i < table.size(); i++){
        block tempBlock = table.at(i);
        string new_name = "{" + D.realState(tempBlock.state1) + ", " + D.realState(tempBlock.state2) + "}";
        mini.add_state(new_name, false);
        for(char a: D.sigma){
            int old1 = D.delta(tempBlock.state1, a);
            int old2 = D.delta(tempBlock.state2, a);
            string tempName = "{" + D.realState(old1) + ", " + D.realState(old2) + "}";
            mini.set_delta(D.ID(new_name),a,D.ID(tempName)); // waar gaat mijn nieuwe delta naartoe? naar de combinatie van old1 en old2 OPGELOST!
        }
    }
    return mini;  // Pieter, ik heb dit even toegevoegd, anders compileert het niet :) (Evert)
}

s_DFA TFA(s_DFA D){
	int n = D.num_states+1;
	// Step 0
	D.lazy_evaluation();
	// Step 1: create the table and mark all the pairs with a final and a nonfinal state
    vector<block> table = makeTable(D);
	// Step 2: mark all the rest and build the equivalent 'block'
    makePairs(D,table);
    table = removeDist(table);
	// Step 3: reconstruct DFA based on the new 'blocks'
    s_DFA optimusDFA = makeNewDFA(D, table);
	return optimusDFA;
}
