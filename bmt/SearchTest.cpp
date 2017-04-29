#include "SearchTest.h"
#include "bmsearch.h"

const wchar_t *kTestString = L"Тестовая строка";

SearchTest::SearchTest(void) : other_shifts(NULL), pattern_length(0), result(-10) {
}

SearchTest::~SearchTest(void) {
}

void SearchTest::SetUp() {
}

void SearchTest::TearDown() {
    if (other_shifts != NULL && pattern_length > 0) {
        delete[] other_shifts;
    }
    clean();
}

void SearchTest::SearchTester(const wchar_t *text, const wchar_t *pattern, size_t start_pos, int expected) {
    pattern_length = wcslen(pattern);

    other_shifts = new size_t[pattern_length];

    build(pattern, pattern_length, other_shifts);
    result = search(text, wcslen(text), start_pos, pattern_length, other_shifts);
    EXPECT_EQ(expected, result);
}

TEST_F(SearchTest, SuccessRus) {
    SearchTester(kTestString, L"строка", 0, 9);
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
    size_t pattern_length1 = 0;
    size_t *other = NULL;
    long long r = -1;

    const wchar_t *pattern = L"ст";

    pattern_length1 = wcslen(pattern);

    other = new size_t[pattern_length1];

    build(pattern, pattern_length1, other);
    r = search(kTestString, wcslen(kTestString), 0, pattern_length1, other);
    EXPECT_EQ(2, r);
    r = search(kTestString, wcslen(kTestString), 3, pattern_length1, other);
    EXPECT_EQ(9, r);

    delete[] other;
}
