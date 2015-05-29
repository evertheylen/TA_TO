
#include "gtest/gtest.h"
#include "tinyxml.h"
#include "FSM.h"
#include "filecompare.h"
#include "../engine_r/RE-eNFA/RE-eNFA.h"
#include "../engine_r/eNFA-DFA/eNFA-DFA.h"
#include "string"

// Write tests for R_Engine here
template <typename R, typename S, typename T>
void write_dot(FSM<R,S,T>* F, std::string name) {
	std::ofstream output_file;
	output_file.open(name);
	F->to_dot(output_file);
	output_file.close();
};

TiXmlDocument read(string name) {
	TiXmlDocument doc;
	doc.LoadFile(name);
	return doc;
}

// Test Regex->eNFA
class Tests: public ::testing::Test {
protected:
	virtual void SetUp(){};
	virtual void TearDown(){};
};

TEST_F(Tests, Regex_eNFA_fileCompare){
	string arg = string("A*B*C*");
	string filename = string("testbestanden/RE-eNFA/file1.dot");
	eNFA <std::string, char, 'e'> N = RE_to_eNFA<std::string, char, 'e'>(arg);
	write_dot(&N, filename);
	EXPECT_TRUE(fileCompare("testbestanden/RE-eNFA/file1.dot", "testbestanden/RE-eNFA/Expectedfile1.dot"));
	
	arg = string("(01)*");
	filename = string("testbestanden/RE-eNFA/file2.dot");
	N = RE_to_eNFA<std::string, char, 'e'>(arg);
	write_dot(&N, filename);
	EXPECT_TRUE(fileCompare("testbestanden/RE-eNFA/file2.dot", "testbestanden/RE-eNFA/Expectedfile2.dot"));
	
	arg = string("e");
	filename = string("testbestanden/RE-eNFA/file3.dot");
	N = RE_to_eNFA<std::string, char, 'e'>(arg);
	write_dot(&N, filename);
	EXPECT_TRUE(fileCompare("testbestanden/RE-eNFA/file3.dot", "testbestanden/RE-eNFA/Expectedfile3.dot"));
	
	arg = string("(0123+345)*345(6+32)*");
	filename = string("testbestanden/RE-eNFA/file4.dot");
	N = RE_to_eNFA<std::string, char, 'e'>(arg);
	write_dot(&N, filename);
	EXPECT_TRUE(fileCompare("testbestanden/RE-eNFA/file4.dot", "testbestanden/RE-eNFA/Expectedfile4.dot"));

	arg = string("(0(63+1)*)*90A*");
	filename = string("testbestanden/RE-eNFA/file5.dot");
	N = RE_to_eNFA<std::string, char, 'e'>(arg);
	write_dot(&N, filename);
	EXPECT_TRUE(fileCompare("testbestanden/RE-eNFA/file5.dot", "testbestanden/RE-eNFA/Expectedfile5.dot"));
	
};

// Test eNFA->DFA
TEST_F(Tests, eNFA_DFA_fileCompare){
	std::string arg = std::string("testbestanden/eNFA-DFA/input1.xml");
	s_eNFA N;
	auto doc = read(arg);
	N.from_xml(doc);
	s_DFA D = MSSC(N);
	write_dot(&N, std::string("testbestanden/eNFA-DFA/org_input1.dot"));
	write_dot(&D, std::string("testbestanden/eNFA-DFA/output1.dot"));
	EXPECT_TRUE(fileCompare("testbestanden/eNFA-DFA/output1.dot", "testbestanden/eNFA-DFA/Expectedfile1.dot"));
	
	arg= "";
	arg = std::string("testbestanden/eNFA-DFA/input2.xml");
	doc = read(arg);
	s_eNFA F;
	F.from_xml(doc);
	s_DFA K = MSSC(N);
	write_dot(&F, std::string("testbestanden/eNFA-DFA/org_input2.dot"));
	write_dot(&K, std::string("testbestanden/eNFA-DFA/output2.dot"));
	EXPECT_TRUE(fileCompare("testbestanden/eNFA-DFA/output2.dot", "testbestanden/eNFA-DFA/Expectedfile2.dot"));
	
};

TEST_F(Tests, Productautomaat_fileCompare){

	
};
// Test Product

// Test DFA->CompactDFA

