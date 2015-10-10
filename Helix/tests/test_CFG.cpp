
#include <iostream>

#include "gtest/gtest.h"
#include "CFL/CFG.h"

TEST(CFG, CFG_init) {
	s_CFG C({'X', 'Y', 'Z'},
			{'0', '1', '2'},
			'0');
	
	C.set_production('X', {"0X1", "1X0"});
	EXPECT_DEATH(C.set_production('A', {}), ".*");
	
	s_PDA P(3, {'a', 'b', 'c'}, {'1', '0'}, 0, '0', {1,2});
	EXPECT_EQ(P.q0, 0);
}
