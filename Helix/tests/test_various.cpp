
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

//A or G	puRine
//Y	C, T or U	pYrimidines
//K	G, T or U	bases which are Ketones
//M	A or C	bases with aMino groups
//S	C or G	Strong interaction
//W	A, T or U	Weak interaction
//B	not A (i.e. C, G, T or U)	B comes after A
//D	not C (i.e. A, G, T or U)	D comes after C
//H	not G (i.e., A, C, T or U)	H comes after G
//V	neither T nor U (i.e. A, C or G)	V comes after U
//N	A C G T U	Nucleic acid
//X
