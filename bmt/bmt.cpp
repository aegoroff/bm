#include <stdio.h>
//#include <tchar.h>
//#include <windows.h>

#include "SearchTest.h"

int main(int argc, char **argv) {
	testing::InitGoogleTest(&argc, argv);
	// Print tests time
	testing::GTEST_FLAG(print_time) = true;
	return RUN_ALL_TESTS();
}