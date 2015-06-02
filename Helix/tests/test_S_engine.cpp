
#include "gtest/gtest.h"
#include "tinyxml.h"
#include "FSM.h"
#include "filecompare.h"

// Test suffixtrees here

File f(../"miss2.txt");
std::ofstream file;
file.open("miss2.dot");
file << *f.suffixtree;
file.close();
EXPECT_TRUE(fileCompare("miss2.dot","miss2_referentie.dot"));