#include "SearchTest.h"
#include "bmsearch.h"

const wchar_t* kTestString = L"Тестовая строка";

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

TEST_F(SearchTest, SuccessRus)
{
	SearchTester(kTestString, L"строка", 0, 9);
}

TEST_F(SearchTest, FailRus)
{
	SearchTester(kTestString, L"хрен", 0, -1);
}

TEST_F(SearchTest, PatternIsTheSameAsText)
{
	SearchTester(kTestString, kTestString, 0, 0);
}

TEST_F(SearchTest, EmptyPattern)
{
	SearchTester(kTestString, L"", 0, 0);
}

TEST_F(SearchTest, TwoMatches)
{
	int nPatternLength = 0;
	int* pOther = NULL;
	int r = -1;

	const wchar_t* pattern = L"ст";

	nPatternLength = wcslen(pattern);

	pOther = new int[nPatternLength];

	build(pattern, nPatternLength, pOther);
	r = search(kTestString, wcslen(kTestString), 0, nPatternLength, pOther);
	EXPECT_EQ(2, r);
	r = search(kTestString, wcslen(kTestString), 3, nPatternLength, pOther);
	EXPECT_EQ(9, r);

	delete [] pOther;
}
