#pragma once

#include "gtest.h"

class SearchTest : public testing::Test
{
	public:
		SearchTest(void);
		~SearchTest(void);
	protected:
		virtual void SetUp();
		virtual void TearDown();

		void SearchTester(const wchar_t *, const wchar_t *, size_t start_pos, int);
        size_t* other_shifts;

};
