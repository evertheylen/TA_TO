
#include <iostream>

#include "suffix.h"

struct Test {
	int A;
	int B;
	
	Test(int i): A(i), B(i) {}
	
	friend std::ostream& operator<<(std::ostream& out, Test& t) {
		out << t.A;
		return out;
	}
};


int main() {
	FancyVector<Test> v(0);
	v.print(std::cout, 25);
	//*((int*) v.cons_data) = 45612;
	v.print(std::cout, 25);
	std::cout << "\n";
	for (int i=0; i<20; i++) {
		std::cout << v.push_back(1000+i) << " cons_cap:" << v.cons_cap() << "\n";
		v.print(std::cout, 25);
	}
	
	v = FancyVector<Test>(10);
	v.print(std::cout, 25);
	std::cout << "\n";
	for (int i=0; i<20; i++) {
		std::cout << v.push_back(1000+i) << " cons_cap:" << v.cons_cap() << "\n";
		v.print(std::cout, 25);
	}
	
	return 0;
}
