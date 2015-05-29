
#include "gtest/gtest.h"
#include "../engine_r/RE-eNFA/RE-eNFA.h"
#include "string"
#include "filecompare.h"
// Write tests for R_Engine here

// Test Regex->eNFA
class Regex_eNFA: public ::testing::Test {
protected:
	virtual void SetUp(){};
	virtual void TearDown(){};
};

TEST_F(Regex_eNFA, Regex_eNFA_fileCompare){
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

// Test Product

// Test DFA->CompactDFA

