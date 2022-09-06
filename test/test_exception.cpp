/***************************************************************************//**
* SSTest: A C++ Testing Library
* 
* This software is distributed free of charge, under the MIT License.
* 
* 
* Copyright (c) 2022 David Lu
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal 
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
* SOFTWARE.
* 
*******************************************************************************/

#include "ctest_macros.h" 

#include <limits>
#include <vector>
#include <string>
#include "sstest/sstest_exception.h"

using namespace sstest;


static const std::string what = "what";

CTEST_DEFINE_TEST(test_ssexception_construct)
{
    Exception e(what);
}

CTEST_DEFINE_TEST(test_ssinvalidarg_construct)
{
    InvalidArgument e(what);
}

CTEST_DEFINE_TEST(test_range_inrange)
{
    range<int> r{ -1, 1 };
    CTEST_ASSERT(r.lower == -1);
    CTEST_ASSERT(r.upper == 1);
    CTEST_ASSERT(r.in_range(0));
    CTEST_ASSERT(r.in_range(-1));
    CTEST_ASSERT(r.in_range(1));
    CTEST_ASSERT(!r.in_range(-2));
    CTEST_ASSERT(!r.in_range(2));
}

CTEST_DEFINE_TEST(test_iterable_range_empty)
{
    iterable_range<int> r{ 0, -1 };
    CTEST_ASSERT(r.lower == 0);
    CTEST_ASSERT(r.upper == -1);

    std::vector<int> v;
    for (int x : r)
    {
        v.push_back(x);
    }
    CTEST_ASSERT(v.empty());
}

CTEST_DEFINE_TEST(test_iterable_range_short)
{
    iterable_range<int> r{ 0, 1 };
    CTEST_ASSERT(r.lower == 0);
    CTEST_ASSERT(r.upper == 1);

    std::vector<int> v;
    for (int x : r)
    {
        v.push_back(x);
    }
    CTEST_ASSERT(v.size() == 1);
    CTEST_ASSERT(v[0] == r.lower);
}

CTEST_DEFINE_TEST(test_iterable_range_long)
{
    iterable_range<int> r{ std::numeric_limits<int>::min(), std::numeric_limits<int>::max() };
    CTEST_ASSERT(r.lower == std::numeric_limits<int>::min());
    CTEST_ASSERT(r.upper == std::numeric_limits<int>::max());

    int i = std::numeric_limits<int>::min();
    for (int x : r)
    {
        CTEST_ASSERT(x == i);
        i++;
    }
    CTEST_ASSERT(i == std::numeric_limits<int>::max());
}

int main()
{
    CTEST_RUN_TEST(test_ssexception_construct);
    CTEST_RUN_TEST(test_ssinvalidarg_construct);
    CTEST_RUN_TEST(test_range_inrange);

    CTEST_RUN_TEST(test_iterable_range_empty);
    CTEST_RUN_TEST(test_iterable_range_short);
    CTEST_RUN_TEST(test_iterable_range_long);

    return CTEST_SUCCESS;
}