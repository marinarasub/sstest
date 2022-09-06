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

#include <sstest/sstest_include.h>

/**
 * \file A-0_tutorial.cpp
 * File used in SSTest tutorial. A very basic example for beginners to unit testing.
 */

/* User functions */

// add two integers and return result
int add(int a, int b)
{
    return a + b;
}

// return true if string is empty (first character is nul char)
bool is_string_empty(const char* str)
{
    return *str == '\0';
}

/* Test functions */

TEST(test_add)
{
    // check 0 + 0  == 0
    REQUIRE(0 == add(0, 0));
    // check 1 + 0  == 1, both left and right hand side
    REQUIRE(1 == add(0, 1));
    REQUIRE(1 == add(1, 0));
    // check 1 + 1 == 2
    REQUIRE(2 == add(1, 1));
    // check 2 + 1 == 3, both left and right hand side
    REQUIRE(3 == add(2, 1));
    REQUIRE(3 == add(1, 2));
    // check 1 + -1 == 0
    REQUIRE(2 == add(1, -1));
}

TEST(test_string_empty)
{
    // Now it's your turn! write some tests here...
}