#include <stdio.h>

#include "SearchTest.h"
#include "eprintf.h"

TEST(NormalizeSize, ZeroBytes) {
    uint64_t size = 0;

    file_size_t result = NormalizeSize(size);

    EXPECT_EQ(result.unit, SizeUnitBytes);
    EXPECT_EQ(result.value.sizeInBytes, size);
}

TEST(NormalizeSize, Bytes) {
    uint64_t size = 1023;

    file_size_t result = NormalizeSize(size);

    EXPECT_EQ(result.unit, SizeUnitBytes);
    EXPECT_EQ(result.value.sizeInBytes, size);
}

TEST(NormalizeSize, KBytesBoundary) {
    uint64_t size = 1024;

    file_size_t result = NormalizeSize(size);

    EXPECT_EQ(result.unit, SizeUnitKBytes);
    EXPECT_EQ(result.value.size, 1.0);
}

TEST(NormalizeSize, KBytes) {
    uint64_t size = BINARY_THOUSAND * 2;

    file_size_t result = NormalizeSize(size);

    EXPECT_EQ(result.unit, SizeUnitKBytes);
    EXPECT_EQ(result.value.size, 2.0);
}

TEST(NormalizeSize, MBytes) {
    uint64_t size = BINARY_THOUSAND * BINARY_THOUSAND * 2;

    file_size_t result = NormalizeSize(size);

    EXPECT_EQ(result.unit, SizeUnitMBytes);
    EXPECT_EQ(result.value.size, 2.0);
}

TEST(NormalizeSize, GBytes) {
    uint64_t size = BINARY_THOUSAND * BINARY_THOUSAND * BINARY_THOUSAND *
                              (uint64_t)4;

    file_size_t result = NormalizeSize(size);

    EXPECT_EQ(result.unit, SizeUnitGBytes);
    EXPECT_EQ(result.value.size, 4.0);
}

TEST(NormalizeSize, TBytes) {
    uint64_t size = (uint64_t)BINARY_THOUSAND * BINARY_THOUSAND *
                              BINARY_THOUSAND * BINARY_THOUSAND * 2;

    file_size_t result = NormalizeSize(size);

    EXPECT_EQ(result.unit, SizeUnitTBytes);
    EXPECT_EQ(result.value.size, 2.0);
}

TEST(NormalizeSize, PBytes) {
    uint64_t size = (uint64_t)BINARY_THOUSAND * BINARY_THOUSAND *
                              BINARY_THOUSAND * BINARY_THOUSAND * BINARY_THOUSAND * 2;

    file_size_t result = NormalizeSize(size);

    EXPECT_EQ(result.unit, SizeUnitPBytes);
    EXPECT_EQ(result.value.size, 2.0);
}

TEST(NormalizeSize, EBytes) {
    uint64_t size = (uint64_t)BINARY_THOUSAND * BINARY_THOUSAND *
                              BINARY_THOUSAND * BINARY_THOUSAND * BINARY_THOUSAND *
                              BINARY_THOUSAND * 2;

    file_size_t result = NormalizeSize(size);

    EXPECT_EQ(SizeUnitEBytes, result.unit);
    EXPECT_EQ(2.0, result.value.size);
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	// Print tests time
	testing::GTEST_FLAG(print_time) = true;
	return RUN_ALL_TESTS();
}
