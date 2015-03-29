#define CATCH_CONFIG_MAIN
// This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"

#include "FSM.h"
#include <fstream>
#include "tinyxml/tinyxml.h"

using namespace std;


TEST_CASE("DFA", "DFA's are cool") {

	s_DFA D({"q0", "q1", "unused1", "q3", "unused3"},  // states (numbered)
		{'a', 'b', 'c'},  // alphabet
		0, {1});  // from -> {to}
	
	// the delta function must be manually set.
	D.set_delta(D.ID("q0"), 'b', D.ID("q1"));
	D.set_delta(D.ID("q1"), 'a', D.ID("q3"));
	
	SECTION("init") {
		REQUIRE(D.realState(1) == "q1");
	}
	
	SECTION("lazy") {
		REQUIRE(D.lazy_evaluation());
		REQUIRE(D.num_states == 3);
	}
	
	SECTION("print") {
		std::ofstream output_file;
		output_file.open("DFA.dot");
		D.to_dot(output_file);
		output_file.close();
		std::cout << "Check the file DFA.dot for yourself.\n";
	}
	
	SECTION("input") {
		TiXmlDocument doc;
		doc.LoadFile("DFA.xml");
		s_DFA Di;
		Di.from_xml(doc);
		
		
		std::ofstream output_file;
		output_file.open("DFA.xml.dot");
		Di.to_dot(output_file);
		output_file.close();
		std::cout << "Check the file DFA.xml.dot for yourself.\n";
	}
}

TEST_CASE("eNFA", "epsilon madness") {
	s_eNFA N({"q0", "q1", "q2", "q3", "q4", "unused1", "unused2", "q7"},
			{'a', 'b', 'c'},
			0, {4});
	
	// (was once a) strongly connected graph. If this works, everything works.
	N.set_delta(0, 'e', {1});
	N.set_delta(1, 'e', {2});
	N.set_delta(2, 'e', {3, 1});
	N.set_delta(3, 'e', {4, 1});
	N.set_delta(4, 'e', {4, 0, 7});
	
	N.set_delta(5, 'a', {5, 6});
	N.set_delta(6, 'e', {5});

	SECTION("ECLOSE") {
		REQUIRE(N.ECLOSE(0).size() == 6);
	}
	
	SECTION("lazy") {
		REQUIRE(N.num_states == 8);
		REQUIRE(N.lazy_evaluation());
		REQUIRE(N.num_states == 6);
		REQUIRE(! N.lazy_evaluation());
	}
	
	
	SECTION("print") {
		std::ofstream output_file;
		output_file.open("eNFA.dot");
		N.to_dot(output_file);
		output_file.close();
		std::cout << "Check the file eNFA.dot for yourself.\n";
	}
	
	SECTION("[input]") {
		TiXmlDocument doc;
		doc.LoadFile("eNFA.xml");
		s_eNFA Ni;
		Ni.from_xml(doc);
		
		
		std::ofstream output_file;
		output_file.open("eNFA.xml.dot");
		Ni.to_dot(output_file);
		output_file.close();
		std::cout << "Check the file eNFA.xml.dot for yourself.\n";
	}
}

