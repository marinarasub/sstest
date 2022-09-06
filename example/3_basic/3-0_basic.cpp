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
#include "example_common.h"

// Make sure you include all the headers you need - some may be included from SSTest but may change in any future version.
#include <cmath>

/**
 * \file 3-0_basic.cpp
 * \brief First file in basic examples
 * This example consists of a multiple files which demonstrate basic functionality of SSTest, incuding 
 * assertions, seperate test cases result reporting.
 */


// The bulk of this file will cover basic tests and assertions and a variety of helper SSTest macros to do so.
// 
// ~Test Cases~
// A test case is just a series of steps to execute, and make sure everything behaves as expected. In SSTest we can define a test case using the 
// TEST() macro (will be introduced later), where each test is essentially a void function.
// 
// ~Assertion~
// An assertion checks a condition and if the condition macthes what is exepcted (depends on the helper used), the assertion PASSES and the 
// current test will continue to execute the remainder of the test body.
// If the expression in the assertion does NOT match what is expected, the assertion FAILS, which in turn fails the test.
// A test case will FAIL if any assertion within the body fails.
// There are two main types of assertions:
// - The REQUIRE() family - check an expression and if failed to match expected condition, fail the test and halt execution of the current test
// - The EXPECT() family - check an expression and if failed to match expected condition, fail the test but continue execution of the current test
//
// Note: some of these tests are not meant to pass, but are just to show functionality


// You can still have one anonymous test. This should be usable even with other tests.
TEST() {}

// Here we use the TEST() macro with a single parameter as the name of the test.
// - The name must be a valid C++ identifier
// - The name must be unique in the current executable
TEST(test_3_0_pass)
{
    // Normally passed assertions are not shown, but for the sake of demonstration enable
    SHOW_PASSED_ASSERTIONS();

    // Here we expect REQUIRE() to pass since 0 is indeed equal to 0
    // The output should show the value of both the left hand side and right hand side arguments
    REQUIRE(0 == 0);

    // Since al assertions have passed at the end of the test, the test passes.
}

TEST(test_3_0_multi_pass)
{
    // Normally passed assertions are not shown, but for the sake of demonstration enable
    SHOW_PASSED_ASSERTIONS();

    // Here we use REQUIRE() with a different comparator, <.
    REQUIRE(1 > 0);
    // An example with a single argument. Also, the type is bool so we should matching output appropriate for a bool
    REQUIRE(true);

    int x = 4;
    int* px = &x;
    // Example of a compound statement to demonstrate argument printing.
    // Notice how '-1' and '0' are printed but &x == px prints 'true'
    // - Depends on order of operations
    // - Sometimes it is better to use more specific macros for the expression, or to break into multiple assertions
    REQUIRE(-1 < (-1 + 1) && &x == px);

    // Example of a specialized assertion macro
    REQUIRE_EQUAL(9, 3 * 3);
}

TEST(test_3_0_noshow)
{
    // For this example, we keep passed assertions off (which is default)

    // REQUIRE_NULL is useful for checking pointer types
    REQUIRE_NULL(nullptr);
}

// Example of how REQUIRE() family stops execution of test
TEST(test_3_0_require_fail)
{
    // This assertion will fail, which will mark the current test as failed as well
    // Using REQUIRE(), the current test will stop execution if failed
    REQUIRE(false);

    // This statement will never execute
    REQUIRE(true && false);
}

TEST(test_3_0_multi_type)
{
    // Comparison macros can accept different types
    REQUIRE_EQUAL(3, 3.0);

}

// Example of the EXPECT() family of macros.
// Contrary to REQUIRE(), if the assertion fails using EXPECT(), the test continues executing (however if will still fail)
TEST(test_3_0_expect)
{
    // The following assertion will fail
    EXPECT(!true);

    // However, this next REQUIRE() statement will still execute since EXPECT() continues execution regardless of failure
    short flags = 0x01 | 0x02 | 0x08;
    REQUIRE(flags & 0x04);

    // Since the last REQUIRE assertion will fail, this statement will never execute
    EXPECT(0);
}

// Example using SUCCEED()
IGNORE_WARNING_UNREACHABLE_BEGIN
TEST(test_3_0_succeed)
{
    // To explicitly pass a test, use SUCCEED().
    // - the test will not continue execution
    SUCCEED();

    // This won't fail the test, even thoug since no code after SUCCEED() will run.
    EXPECT(0 != 0);
}
IGNORE_WARNING_UNREACHABLE_END

// Example with floating points
// Since floating point types may deviate from their intended values from calculation/rounding error, we can use special macros
// to help compare floating point types.
TEST(test_3_0_float)
{
    // This should pass since floats are allowed to vary by very little
    // Note you cannot mix float and double here since lenience is relative with type (i.e. floats get more lenience than doubles) and should be consistent between both arguments
    EXPECT_FLOAT_EQUAL(1.0f / 3.0f, 0.3333333f);
    // This probably won't pass
    EXPECT_EQUAL(1.0f / 3.0f, 0.3333333f);

    // Similar for doubles
    // This should fail since doubles have higher accuracy
    EXPECT_DOUBLE_EQUAL(1.0 / 3.0, 0.3333333);
    EXPECT_EQUAL(1.0 / 3.0, 0.3333333);

    // Be careful: since 0 is deduced as an integral type, this does not work. 
    // EXPECT_FLOAT_EQUAL(0, 0.0000001f)
    // instead:
    EXPECT_FLOAT_EQUAL(0.0f, 0.0000001f);
}

// Example using pointers
TEST(test_3_0_pointer)
{ 
    int x;
    int* px = nullptr;
    EXPECT_NOT_NULL(px);
    px = &x;
    EXPECT_NULL(px);
}

// Example with const varaibles
TEST(test_3_0_const)
{
    const size_t i = 0;
    EXPECT_LESS(i, i + 1);
}

// Example with result of function call as argument
TEST(test_3_0_function_result)
{
    SHOW_PASSED_ASSERTIONS();

    const double pi = 3.14159265;
    EXPECT_DOUBLE_EQUAL(0.0, sin(0));
    // If you two values won't be exactly equal, you can use REQUIRE_APPROX()/EXPECT_APPROX() with
    // - first parameter
    // - second parameter
    // - maximum difference
    EXPECT_APPROX(1.0 / 2.0, sin(pi / 6.0), 0.0001);
}