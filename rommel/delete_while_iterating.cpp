
#include <vector>
#include <string>
#include <iostream>
#include <set>

template<typename T>
void print(std::vector<T> s) {
	std::cout << "{";
	for (auto e: s) {
		std::cout << e << ", ";
	}
	std::cout << "}\n";
}

int main() {
	std::vector<int> l {50,4,1,20,4,50,3,10,4,4,5,1,2,4,200,1,1,20};
	
	std::cout << "start:\n";
	print(l);
	std::cout << "\n";
	
	for (int j=0; j<l.size(); j++) {
		std::cout << "checking " << j << " = " << l[j] << "\n";
		if (l[j] >= 10) {
			std::cout << "erasing...\n";
			l.erase(l.begin()+j);
			print(l);
			j--;
		}
	}
	
	std::cout << "\nend:\n";
	print(l);
	std::cout << "\n";
}
