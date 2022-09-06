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

#include "sstest/sstest_string.h"

using namespace sstest;

CTEST_DEFINE_TEST(stringview_default_construct)
{
    StringView s;
    CTEST_ASSERT(s.empty());
    CTEST_ASSERT(s.length() == 0);
    CTEST_ASSERT(s.length() == s.size());
}

CTEST_DEFINE_TEST(stringview_cstr_construct)
{
    StringView s = "hello";
    CTEST_ASSERT(!s.empty());
    CTEST_ASSERT(s.length() == strlen("hello"));
    CTEST_ASSERT(s.size() == s.length());
    CTEST_ASSERT(s.substr(0) == s);
}

CTEST_DEFINE_TEST(stringview_cstrn_construct)
{
    StringView s;
    s = StringView("hello", 3);
    CTEST_ASSERT(!s.empty());
    CTEST_ASSERT(s.length() == 3);
    CTEST_ASSERT(s.size() == s.length());
    CTEST_ASSERT(s != "hello");
    CTEST_ASSERT(s == StringView("hello").substr(0, 3));

    s = StringView("hello", 6);
    CTEST_ASSERT(!s.empty());
    CTEST_ASSERT(s.length() == 6);
    CTEST_ASSERT(s.size() == s.length());
    CTEST_ASSERT(s[5] == '\0');
    CTEST_ASSERT(s != "hello");
    CTEST_ASSERT(compare("hello", s) < 0);
    CTEST_ASSERT(compare(s, "hello") > 0);
    CTEST_ASSERT(compare(s, "hello", strlen("hello")) == 0);
}

CTEST_DEFINE_TEST(test_cstrlen)
{
    CTEST_ASSERT(cstrlen("") == 0);
    CTEST_ASSERT(cstrlen("\0") == 0);
    CTEST_ASSERT(cstrlen("0") == 1);
    CTEST_ASSERT(cstrlen("abcd") == 4);
    CTEST_ASSERT(cstrlen("abcd\0efgh") == 4);
    CTEST_ASSERT(cstrlen("\r\n\t") == 3);
}

CTEST_DEFINE_TEST(test_cmemcmp)
{
    CTEST_ASSERT(cmemcmp("", "", 0) == 0);

    CTEST_ASSERT(cmemcmp("a", "a", 1) == 0);
    CTEST_ASSERT(cmemcmp("a", "b", 1) < 0);
    CTEST_ASSERT(cmemcmp("b", "a", 1) > 0);

    CTEST_ASSERT(cmemcmp("a", "ab", 1) == 0);
    CTEST_ASSERT(cmemcmp("a", "ab", 1) == 0);
    CTEST_ASSERT(cmemcmp("b", "ba", 1) == 0);

    CTEST_ASSERT(cmemcmp("ab", "ac", 1) == 0);
    CTEST_ASSERT(cmemcmp("ab", "ac", 2) < 0);
    CTEST_ASSERT(cmemcmp("ac", "ab", 2) > 0);
}

CTEST_DEFINE_TEST(test_stringview_cmp)
{
    StringView s1, s2;

    s1 = "";
    s2 = "";
    CTEST_ASSERT(s1 == s1);
    CTEST_ASSERT(s2 == s2);
    CTEST_ASSERT(s1 == s2);

    s1 = "a";
    s2 = "a";
    CTEST_ASSERT(s1 == s1);
    CTEST_ASSERT(s2 == s2);
    CTEST_ASSERT(s1 == s2);

    s1 = "a";
    s2 = "b";
    CTEST_ASSERT(s1 == s1);
    CTEST_ASSERT(s2 == s2);
    CTEST_ASSERT(s1 != s2);
    CTEST_ASSERT(s1 < s2);

    s1 = "a";
    s2 = "ab";
    CTEST_ASSERT(s1 == s1);
    CTEST_ASSERT(s2 == s2);
    CTEST_ASSERT(s1 != s2);
    CTEST_ASSERT(s2 != s1);
    CTEST_ASSERT(s1 < s2);
    CTEST_ASSERT(s2 > s1);

    s1 = "b";
    s2 = "ab";
    CTEST_ASSERT(s1 == s1);
    CTEST_ASSERT(s2 == s2);
    CTEST_ASSERT(s1 != s2);
    CTEST_ASSERT(s2 != s1);
    CTEST_ASSERT(s1 > s2);
    CTEST_ASSERT(s2 < s1);

    s1 = "aa";
    s2 = "ab";
    CTEST_ASSERT(s1 == s1);
    CTEST_ASSERT(s2 == s2);
    CTEST_ASSERT(s1 != s2);
    CTEST_ASSERT(s1 < s2);
    CTEST_ASSERT(s2 != s1);
    CTEST_ASSERT(s2 > s1);
}

int main()
{
    CTEST_RUN_TEST(stringview_default_construct);
    CTEST_RUN_TEST(stringview_cstr_construct);
    CTEST_RUN_TEST(stringview_cstrn_construct);

    CTEST_RUN_TEST(test_cstrlen);
    CTEST_RUN_TEST(test_cmemcmp);
    CTEST_RUN_TEST(test_stringview_cmp);

    return CTEST_SUCCESS;
}