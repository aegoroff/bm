#include "SearchTest.h"
#include "bmsearch.h"

const wchar_t* c_pStr = L"Тестовая строка";

SearchTest::SearchTest(void) : pOtherShifts(NULL), nPatternLength(0), result(-10)
{
}

SearchTest::~SearchTest(void)
{
}

void SearchTest::SetUp()
{
}

void SearchTest::TearDown()
{
	if (pOtherShifts != NULL && nPatternLength > 0) {
	
		delete [] pOtherShifts;
	}
	clean();
}

void SearchTest::SearchTester(const wchar_t* text, const wchar_t* pattern, int startPos, int expected)
{
	nPatternLength = wcslen(pattern);
	
	pOtherShifts = new int[nPatternLength];

	build(pattern, nPatternLength, pOtherShifts);
	result = search(text, wcslen(text), startPos, nPatternLength, pOtherShifts);
	EXPECT_EQ(expected, result);
}

TEST_F(SearchTest, SuccessRus) {
	SearchTester(c_pStr, L"ñòðîêà", 0, 9);
}

TEST_F(SearchTest, FailRus) {
	SearchTester(c_pStr, L"õðåí", 0, -1);
}

TEST_F(SearchTest, PatternIsTheSameAsText) {
	SearchTester(c_pStr, c_pStr, 0, 0);
}

TEST_F(SearchTest, EmptyPattern) {
	SearchTester(c_pStr, L"", 0, 0);
}

TEST_F(SearchTest, TwoMatches) {
	int nPatternLength = 0;
	int* pOther = NULL;
	int r = -1;

	const wchar_t* pattern = L"ñò";
	
	nPatternLength = wcslen(pattern);
	
	pOther = new int[nPatternLength];

	build(pattern, nPatternLength, pOther);
	r = search(c_pStr, wcslen(c_pStr), 0, nPatternLength, pOther);
	EXPECT_EQ(2, r);
	r = search(c_pStr, wcslen(c_pStr), 3, nPatternLength, pOther);
	EXPECT_EQ(9, r);

	if (pOther != NULL) {
	
		delete [] pOther;
	}
}