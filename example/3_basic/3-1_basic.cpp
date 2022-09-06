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

#include <cmath>
#include <vector>

/**
 * \file 3-1_basic.cpp
 * \brief Second file in basic examples
 * This example consists of a multiple files which demonstrate basic functionality of SSTest, incuding 
 * assertions, seperate test cases result reporting.
 * This is the second file in this example and should demonstrate that SSTest can be used with multiple files per project
 * This file will contain some more complete use cases and examples
 */


// This file continues from 3-0_basic.cpp, and introduces test suites later (below).


// This will NOT work since an anonymous test is already defined in 3-0_basic.cpp
// TEST() {}

// A simple function that returns 1 more than the last call, starting at 0.
static int counter()
{
    static int count = 0;
    return count++;
}

// An example of testing a function with a changing return value.
TEST(test_3_1_counter)
{
    int i = 0;
    EXPECT_EQUAL(i++, counter());
    EXPECT_EQUAL(i++, counter());
    EXPECT_EQUAL(i++, counter());
    EXPECT_EQUAL(i, 3);
}

TEST(test_3_1_carray)
{
    int arr1[] = { 1, 3, 5, 3, -9, 0, 2 };
    int arr2[1] = { 0 };
    REQUIRE_EQUAL(arr1, arr2);
}

// Example showing container value printing
TEST(test_3_1_container)
{
    std::vector<int> v = { 1, 3, 5, 3, -9, 0, 2 };
    REQUIRE_EQUAL(v, std::vector<int>{});
}

// In this next section, there will be examples for test suites.
// Test suites are a way to group similar tests.
//
// To use a test suite, simple give 2 arguments to the TEST() macro instead of 1.
// - The first parameter is the test suite
// - The second paramter is the name of the test, unique within the suite
// Like the basic single argument TEST(), both parameters must be valid C++ identifiers
TEST(suite_3_1_suite, first)
{
    // Use MESSAGE() to send an output to the test runner to print out, similar to cout.
    MESSAGE() << "Running first!";
}

// A test suite can have more than 1 test case
TEST(suite_3_1_suite, second)
{
    MESSAGE() << "Running second!";
}

TEST(suite_3_1_suite, fail)
{
    REQUIRE(0);
}

