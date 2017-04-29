#include <stdio.h>

#include "search_test.h"
#include "eprintf.h"

TEST(normalize_size, ZeroBytes) {
    uint64_t size = 0;

    file_size_t result = normalize_size(size);

    EXPECT_EQ(result.unit, size_unit_bytes);
    EXPECT_EQ(result.value.size_in_bytes, size);
}

TEST(normalize_size, Bytes) {
    uint64_t size = 1023;

    file_size_t result = normalize_size(size);

    EXPECT_EQ(result.unit, size_unit_bytes);
    EXPECT_EQ(result.value.size_in_bytes, size);
}

TEST(normalize_size, KBytesBoundary) {
    uint64_t size = 1024;

    file_size_t result = normalize_size(size);

    EXPECT_EQ(result.unit, size_unit_kbytes);
    EXPECT_EQ(result.value.size, 1.0);
}

TEST(normalize_size, KBytes) {
    uint64_t size = BINARY_THOUSAND * 2;

    file_size_t result = normalize_size(size);

    EXPECT_EQ(result.unit, size_unit_kbytes);
    EXPECT_EQ(result.value.size, 2.0);
}

TEST(normalize_size, MBytes) {
    uint64_t size = BINARY_THOUSAND * BINARY_THOUSAND * 2;

    file_size_t result = normalize_size(size);

    EXPECT_EQ(result.unit, size_unit_mbytes);
    EXPECT_EQ(result.value.size, 2.0);
}

TEST(normalize_size, GBytes) {
    uint64_t size = BINARY_THOUSAND * BINARY_THOUSAND * BINARY_THOUSAND *
                              (uint64_t)4;

    file_size_t result = normalize_size(size);

    EXPECT_EQ(result.unit, size_unit_gbytes);
    EXPECT_EQ(result.value.size, 4.0);
}

TEST(normalize_size, TBytes) {
    uint64_t size = (uint64_t)BINARY_THOUSAND * BINARY_THOUSAND *
                              BINARY_THOUSAND * BINARY_THOUSAND * 2;

    file_size_t result = normalize_size(size);

    EXPECT_EQ(result.unit, size_unit_tbytes);
    EXPECT_EQ(result.value.size, 2.0);
}

TEST(normalize_size, PBytes) {
    uint64_t size = (uint64_t)BINARY_THOUSAND * BINARY_THOUSAND *
                              BINARY_THOUSAND * BINARY_THOUSAND * BINARY_THOUSAND * 2;

    file_size_t result = normalize_size(size);

    EXPECT_EQ(result.unit, size_unit_pbytes);
    EXPECT_EQ(result.value.size, 2.0);
}

TEST(normalize_size, EBytes) {
    uint64_t size = (uint64_t)BINARY_THOUSAND * BINARY_THOUSAND *
                              BINARY_THOUSAND * BINARY_THOUSAND * BINARY_THOUSAND *
                              BINARY_THOUSAND * 2;

    file_size_t result = normalize_size(size);

    EXPECT_EQ(size_unit_ebytes, result.unit);
    EXPECT_EQ(2.0, result.value.size);
}

int main(int argc, char **argv)
{
	testing::InitGoogleTest(&argc, argv);
	// Print tests time
	testing::GTEST_FLAG(print_time) = true;
	return RUN_ALL_TESTS();
}
