
#include "gtest/gtest.h"
#include "tinyxml.h"
#include "FSM.h"
#include "filecompare.h"
#include "../engine_r/RE-eNFA/RE-eNFA.h"
#include "../engine_r/eNFA-DFA/eNFA-DFA.h"
#include "../engine_r/Product/product.h"
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

TEST(Tests, Regex_eNFA_fileCompare){
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
	
	arg = string("");
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
TEST(Tests, eNFA_DFA_fileCompare){
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

// Test Product
TEST(Tests, Product_fileCompare){
	std::string arg1 = std::string("testbestanden/Product/input1_DFA1.xml");
	std::string arg2 = std::string("testbestanden/Product/input1_DFA2.xml");
	
	s_DFA D1;
	auto doc = read(arg1);
	D1.from_xml(doc);
	
	s_DFA D2;
	doc = read(arg2);
	D2.from_xml(doc);
	
	DFA <std::string, char> P = product(D1, D2, true);  // true --> intersection
	write_dot(&P, std::string("testbestanden/Product/output1.dot"));
	EXPECT_TRUE(fileCompare("testbestanden/Product/output1.dot", "testbestanden/Product/Expectedfile1.dot"));
	
	arg1 = std::string("testbestanden/Product/input1_DFA1.xml");
	arg2 = std::string("testbestanden/Product/input1_DFA2.xml");
	
	s_DFA D3;
	doc = read(arg1);
	D3.from_xml(doc);
	
	s_DFA D4;
	doc = read(arg2);
	D4.from_xml(doc);
	
	DFA <std::string, char> P2 = product(D3, D4, true);  // true --> intersection
	write_dot(&P2, std::string("testbestanden/Product/output2.dot"));
	EXPECT_TRUE(fileCompare("testbestanden/Product/output2.dot", "testbestanden/Product/Expectedfile2.dot"));
	
	
	arg1 = std::string("testbestanden/Product/input3_DFA1.xml");
	arg2 = std::string("testbestanden/Product/input3_DFA2.xml");
	
	s_DFA D5;
	doc = read(arg1);
	D5.from_xml(doc);
	
	s_DFA D6;
	doc = read(arg2);
	D6.from_xml(doc);
	
	DFA <std::string, char> P3 = product(D5, D6, true);  // true --> intersection
	write_dot(&P3, std::string("testbestanden/Product/output3.dot"));
	EXPECT_TRUE(fileCompare("testbestanden/Product/output3.dot", "testbestanden/Product/Expectedfile3.dot"));
};
// Test DFA->CompactDFA
TEST(Tests, TFA_fileCompare){
    std::string arg = std::string("testbestanden/TFA/file2.xml");
    s_DFA D;
    auto doc = read(arg);
    D.from_xml(doc);
    write_dot(&D, std::string("testbestanden/TFA/output1.dot"));
    EXPECT_TRUE(fileCompare("testbestanden/TFA/output1.dot", "testbestanden/TFA/Expectedfile1.dot"));
    
    arg= "";
    arg = std::string("testbestanden/TFA/file4.xml");
    doc = read(arg);
    s_DFA K;
    K.from_xml(doc);
    write_dot(&K, std::string("testbestanden/TFA/output4.dot"));
    EXPECT_TRUE(fileCompare("testbestanden/TFA/output4.dot", "testbestanden/TFA/Expectedfile4.dot"));
};


