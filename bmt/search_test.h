#pragma once

#include <gtest/gtest.h>

class search_test : public testing::Test {
public:
    search_test(void);
    ~search_test(void);
protected:
    virtual void SetUp();
    virtual void TearDown();

    void search_run(const wchar_t*, const wchar_t*, size_t start_pos, int);
    size_t* other_shifts;

};
