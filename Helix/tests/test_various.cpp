
#include "gtest/gtest.h"
#include "../etc/fasta.h"
#include "string"
#include "vector"

class testVarious: public ::testing::Test {
protected:
	virtual void SetUp(){};
	virtual void TearDown(){};
};
// Test various stuff if needed (fasta string replace?)

TEST_F(testVarious, tests){
	std::string str = "ABCHGUT;RYKMSWBDHVNX";
	std::vector<std::string> result;
	result.push_back("A(C+G+T+U)C(A+C+T+U)GUT");
	result.push_back("(A+G)(C+T+U)(G+T+U)(A+C)(C+G)(A+T+U)(C+G+T+U)(A+G+T+U)(A+C+T+U)(A+C+G)(A+C+G+T+U)X");
	EXPECT_EQ(result, fastaReplace(str));
};
