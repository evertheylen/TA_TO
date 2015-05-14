 
#include <iostream>
#include <boost/bimap.hpp>

using namespace std;

int main() {
	typedef boost::bimap<int, float> m_t;
	m_t m;
	m.left.insert(m_t::left_value_type(5, 3.14));
	m.right.insert(m_t::right_value_type(5.7, 7));
	
// 	m.left[5] = 3.14;
// 	m.right[5.7] = 7;
	
	cout << m.right.at(3.14) << endl << m.left.at(7);
}
	