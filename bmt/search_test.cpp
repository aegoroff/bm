#include "search_test.h"
#include "bmsearch.h"

const wchar_t* TEST_STRING = L"Тестовая строка";

search_test::search_test(void) : other_shifts(NULL) {
}

search_test::~search_test(void) {
}

void search_test::SetUp() {
}

void search_test::TearDown() {
    if(other_shifts != NULL) {
        delete[] other_shifts;
    }
    clean();
}

void search_test::search_run(const wchar_t* text, const wchar_t* pattern, size_t start_pos, int expected) {
    // Arrange
    auto pattern_length = wcslen(pattern);
    other_shifts = new size_t[pattern_length];

    build(pattern, pattern_length, other_shifts);

    // Act
    auto result = search(text, wcslen(text), start_pos, pattern_length, other_shifts);

    // Assert
    EXPECT_EQ(expected, result);
}

TEST_F(search_test, search_rustringnotfromstart_positiveresult) {
    search_run(TEST_STRING, L"строка", 0, 9);
}

TEST_F(search_test, search_stringstartsfrompattern_zeroresult) {
    search_run(TEST_STRING, L"Тестовая", 0, 0);
}

TEST_F(search_test, search_nomatch_negativeresult) {
    search_run(TEST_STRING, L"хрен", 0, -1);
}

TEST_F(search_test, search_patternisthetextitself_zeroresult) {
    search_run(TEST_STRING, TEST_STRING, 0, 0);
}

TEST_F(search_test, search_emptypattern_zeroresult) {
    search_run(TEST_STRING, L"", 0, 0);
}

TEST_F(search_test, search_twomatchesinstring_twosuccessresults) {
    // Arrange
    const wchar_t* pattern = L"ст";
    size_t pattern_length = wcslen(pattern);
    other_shifts = new size_t[pattern_length];
    auto text_length = wcslen(TEST_STRING);

    build(pattern, pattern_length, other_shifts);

    // Act
    auto r1 = search(TEST_STRING, text_length, 0, pattern_length, other_shifts);
    auto r2 = search(TEST_STRING, text_length, 3, pattern_length, other_shifts);

    // Assert
    EXPECT_EQ(2, r1);
    EXPECT_EQ(9, r2);
}
