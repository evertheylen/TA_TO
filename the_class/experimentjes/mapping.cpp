#include <iostream>
#include <map>
#include <unordered_map>
#include <set>
#include <string>

using namespace std;

int main() {
	set<int> s1 {1,8,6,6};
	set<int> s2 {1,8,8,7};
	map<set<int>, string> m_sets;
	
	m_sets[s1] = "test";
	m_sets[s2] = "lol";
	
	cout << m_sets[s1] << endl;
	
	
	set<int> s3 {1,2,6,7};
	set<int> s4 {1,8,8,7};
	map<set<int>, string> um_sets;
	
	um_sets[s1] = "test";
	um_sets[s2] = "lol";
	
	cout << um_sets[s1] << endl;
}