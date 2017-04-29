#include "SearchTest.h"
#include "bmsearch.h"

const wchar_t *kTestString = L"Тестовая строка";

SearchTest::SearchTest(void) : other_shifts(NULL) {
}

SearchTest::~SearchTest(void) {
}

void SearchTest::SetUp() {
}

void SearchTest::TearDown() {
    if (other_shifts != NULL) {
        delete[] other_shifts;
    }
    clean();
}

void SearchTest::SearchTester(const wchar_t *text, const wchar_t *pattern, size_t start_pos, int expected) {
    // Arrange
    auto pattern_length = wcslen(pattern);
    other_shifts = new size_t[pattern_length];

    build(pattern, pattern_length, other_shifts);

    // Act
    auto result = search(text, wcslen(text), start_pos, pattern_length, other_shifts);

    // Assert
    EXPECT_EQ(expected, result);
}

TEST_F(SearchTest, SuccessRus) {
    SearchTester(kTestString, L"строка", 0, 9);
}

TEST_F(SearchTest, StringStartsWithPattern) {
    SearchTester(kTestString, L"Тестовая", 0, 0);
}

TEST_F(SearchTest, FailRus) {
    SearchTester(kTestString, L"хрен", 0, -1);
}

TEST_F(SearchTest, PatternIsTheSameAsText) {
    SearchTester(kTestString, kTestString, 0, 0);
}

TEST_F(SearchTest, EmptyPattern) {
    SearchTester(kTestString, L"", 0, 0);
}

TEST_F(SearchTest, TwoMatches) {
    // Arrange
    const wchar_t *pattern = L"ст";
    size_t pattern_length = wcslen(pattern);
    other_shifts = new size_t[pattern_length];

    build(pattern, pattern_length, other_shifts);

    // Act
    auto r1 = search(kTestString, wcslen(kTestString), 0, pattern_length, other_shifts);
    auto r2 = search(kTestString, wcslen(kTestString), 3, pattern_length, other_shifts);

    // Assert
    EXPECT_EQ(2, r1);
    EXPECT_EQ(9, r2);
}
