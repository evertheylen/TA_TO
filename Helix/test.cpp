
#include <string>
#include <fstream>
#include "gtest/gtest.h"

#include "tests/filecompare.h"

TEST(Meta, GTestTest) {
	EXPECT_TRUE(true);
	EXPECT_EQ(45, 45);
	EXPECT_FALSE(false);
}

TEST(Meta, FileTests) {
	EXPECT_TRUE(fileCompare("test.cpp", "test.cpp"));
}

int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	// Get rid of the irritating threading warning when testing DEATH
	::testing::FLAGS_gtest_death_test_style = "threadsafe";
	return RUN_ALL_TESTS();
}
