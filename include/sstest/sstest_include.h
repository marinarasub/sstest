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

#ifndef _SSTEST_INCLUDE_H_
#define _SSTEST_INCLUDE_H_

#include "sstest_config.h"
#include "sstest_def.h"
#include "sstest_utility.h"
#include "sstest_traits.h"
#include "sstest_exception.h"
#include "sstest_info.h"
#include "sstest_float.h"
#include "sstest_string.h"
#include "sstest_timer.h"
#include "sstest_printer.h"
#include "sstest_console.h"
#include "sstest_compare.h"
#include "sstest_assertion.h"
#include "sstest_test.h"
#include "sstest_registry.h"
#include "sstest_registrar.h"
#include "sstest_summary.h"
#include "sstest_runner.h"
#include "sstest_run.h"

/**
 * \file sstest_include.h
 * \brief The main file to include for using SSTest. Includes all necessary files and include
 * most top level macros for using SSTest.
 */


/**
 * \def FAIL
 * \brief Fails the current test, stopping continue execution in the current test
 */
#define FAIL() \
        INTERNAL_SSTEST_FAILURE()

/**
 * \def SUCCEED
 * \brief Passes the current test, and does not continue execution in the current test
 */
#define SUCCEED() \
        INTERNAL_SSTEST_SUCCESS()

/**
 * \def SHOW_PASSED_ASSERTIONS
 * \brief Configure the test runner to show passed assertions (by default, this if off) 
 * 
 */
#define SHOW_PASSED_ASSERTIONS() \
        INTERNAL_SSTEST_SHOW_PASSED_ASSERTIONS()

/**
* \def MESSAGE
* \brief Create a streamable object to stream messages to. Your message can be printed out
* in the console or other outputs configured by the test runner.
* Use the stream operator<< to provide your message.
* A line break will be inserted at the end of your message
*/
#define MESSAGE() \
        INTERNAL_SSTEST_MESSAGE()

// All assertion results will be reported to the test runner.
// To avoid side effects of "argument collection", wrap your statement in parentheses
// - e.g. REQUIRE(1 == 2) -> REQUIRE((1 == 2))
//

//
// The REQUIRE() family of arguments check a condition and stop further execution of the current test if failed
// Exceptions are not handled and will propagate to the test runner
//

/**
 * \def REQUIRE
 * \brief Check if a general expression evaluates to or is implicitly converted to true, stopping execution of current test if fails 
 * 
 * Tries to collect and save as many arguments as possible to print later, if possible.
 * 
 * The expression's final result must be convertible to a bool, but intermediate types do not have to be convertible to bool
 */
#define REQUIRE(x) \
        INTERNAL_SSTEST_ASSERTION_INFER("REQUIRE", x, INTERNAL_SSTEST_EXIT)

/**
* \def REQUIRE_NOT
* \brief Check if negation of expression evaluates to or is implicitly converted to true, stopping execution of current test if fails
* 
*/
#define REQUIRE_NOT(x) \
        INTERNAL_SSTEST_ASSERTION_NOT("REQUIRE_NOT", x, INTERNAL_SSTEST_EXIT)

/**
 * \def REQUIRE_TRUE
 * \brief Check if an expression evaluates to true
 * 
 * As opposed to REQUIRE(), this is meant for explicit bool types and will not save each argument in a multi-argument comparison
 */
#define REQUIRE_TRUE(x) \
        INTERNAL_SSTEST_ASSERTION_BOOL_TRUE("REQUIRE_TRUE", x, INTERNAL_SSTEST_EXIT)

/**
 * \def REQUIRE_FALSE
 * \brief Check if an expression evaluatees to false
 * 
 * As opposed to REQUIRE(), this is meant for explicit bool types and will not save each argument in a multi-argument comparison
 */
#define REQUIRE_FALSE(x) \
        INTERNAL_SSTEST_ASSERTION_BOOL_FALSE("REQUIRE_FALSE", x, INTERNAL_SSTEST_EXIT)

/**
 * \def REQUIRE_NULL
 * \brief Check if a value is equal to nullptr
 * 
 * Designed to be used for pointer types but if you type is equal comparable to nullptr_t, it can also be used as argument here
 */
#define REQUIRE_NULL(x) \
        INTERNAL_SSTEST_ASSERTION_NULL("REQUIRE_NULL", x, INTERNAL_SSTEST_EXIT)

/**
 * \def REQUIRE_NOT_NULL
 * \brief Check if a value is not equal to nullptr
 * 
 * Designed to be used for pointer types but if you type is not equal comparable to nullptr_t, it can also be used as argument here
 */
#define REQUIRE_NOT_NULL(x) \
        INTERNAL_SSTEST_ASSERTION_NOT_NULL("REQUIRE_NOT_NULL", x, INTERNAL_SSTEST_EXIT)

/**
 * \def REQUIRE_EQUAL
 * \brief Check if two values are equal; equivalent to using the == operator
 * 
 * Both arguments will be saved for printing
 * 
 * Assumes that the equal operator is symmetric (a == b -> b == a)
 */
#define REQUIRE_EQUAL(x, y) \
        INTERNAL_SSTEST_ASSERTION_EQ2("REQUIRE_EQUAL", x, y, INTERNAL_SSTEST_EXIT)

/**
 * \def REQUIRE_NOT_EQUAL
 * \brief Check if two values are not equal; equivalent to using the != operator
 * 
 * Both arguments will be saved for printing
 * 
 * Assumes that the not equal operator is symmetric (a != b -> b != a)
 */
#define REQUIRE_NOT_EQUAL(x, y) \
        INTERNAL_SSTEST_ASSERTION_NEQ2("REQUIRE_NOT_EQUAL", x, y, INTERNAL_SSTEST_EXIT)

/**
 * \def REQUIRE_LESS_EQUAL
 * \brief Check if the left hand side argument is less than or equal to the right hand side argument; equivalent to using the <= operator
 * 
 * Both arguments will be saved for printing
 */
#define REQUIRE_LESS_EQUAL(x, y) \
        INTERNAL_SSTEST_ASSERTION_LE("REQUIRE_LESS_EQUAL", x, y, INTERNAL_SSTEST_EXIT)

/**
 * \def REQUIRE_GREATER_EQUAL
 * \brief Check if the left hand side argument is greater or equal to the right hand side argument; equivalent to using the >= operator
 * 
 * Both arguments will be saved for printing
 */
#define REQUIRE_GREATER_EQUAL(x, y) \
        INTERNAL_SSTEST_ASSERTION_GE("REQUIRE_GREATER_EQUAL", x, y, INTERNAL_SSTEST_EXIT)

/**
 * \def REQUIRE_LESS
 * \brief Check if the left hand side argument is strictly less than the right hand side argument; equivalent to using the < operator
 * 
 * Both arguments will be saved for printing
 */
#define REQUIRE_LESS(x, y) \
        INTERNAL_SSTEST_ASSERTION_LESS("REQUIRE_LESS", x, y, INTERNAL_SSTEST_EXIT)

/**
 * \def REQUIRE_GREATER
 * \brief Check if the left hand side argument is strictly greater than the right hand side argument; equivalent to using the > operator
 * 
 * Both arguments will be saved for printing
 */
#define REQUIRE_GREATER(x, y) \
        INTERNAL_SSTEST_ASSERTION_GREATER("REQUIRE_GREATER", x, y, INTERNAL_SSTEST_EXIT)

/**
 * \def REQUIRE_ALL_EQUAL
 * \brief Check if all values are equal. All values are compared to the first argument, in O(n) comparisons. Equivalent to using == operator for each other argument as right hand side argument
 * 
 * Assumes that the equal operator is transitive and symmetric between each pair of types
 * 
 * All arguments are saved for printing
 * 
 * \sa REQUIRE_ALL_EACH_EACH
 */
#define REQUIRE_ALL_EQUAL(x, y, ...) \
        INTERNAL_SSTEST_ASSERTION_EQALL("REQUIRE_ALL_EQUAL", INTERNAL_SSTEST_EXIT, x, y, __VA_ARGS__)

/**
 * \def REQUIRE_ALL_EQUAL_EACH
 * \brief Check if all values are equal. All values are compared to each other, so requires O(n^2) comparisons. Equivalent to using == operator for each combination of two arguments
 * 
 * Useful if transitive property (a == b, b == c -> a == c) is not strictly true (e.g. floating point types)
 * 
 * Assumes that the equal operator is symmetric between each pair of types
 * 
 * All arguments are saved for printing
 * 
 * \sa REQUIRE_ALL_EQUAL
 */
#define REQUIRE_ALL_EQUAL_EACH(x, y, ...) \
        INTERNAL_SSTEST_ASSERTION_EQALLEACH("REQUIRE_ALL_EQUAL_EACH", INTERNAL_SSTEST_EXIT, x, y, __VA_ARGS__)

/**
 * \def REQUIRE_FLOAT_EQUAL
 * \brief Check if two floating points are equal, within a very small lenience
 * 
 * Useful if a small amount of rounding and/or computation error is expected
 * 
 * Save both arguments for printing
 */
#define REQUIRE_FLOAT_EQUAL(x, y) \
        INTERNAL_SSTEST_ASSERTION_EQFLOAT("REQUIRE_FLOAT_EQUAL", x, y, INTERNAL_SSTEST_EXIT)

/**
 * \def REQUIRE_DOUBLE_EQUAL
 * \brief Equivalent to REQUIRE_FLOAT_EQUAL()
 * 
 * \sa REQUIRE_FLOAT_EQUAL()
 */
#define REQUIRE_DOUBLE_EQUAL(x, y) \
        INTERNAL_SSTEST_ASSERTION_EQFLOAT("REQUIRE_DOUBLE_EQUAL", x, y, INTERNAL_SSTEST_EXIT)

/**
 * \def REQUIRE_APPROX
 * \brief Check if two values are within a given delta
 * 
 * Requires the subtraction operator (-) be defined, and the result of the subtraction to be compared against the given delta
 * 
 * Save both comparison arguments and delta for printing
 */
#define REQUIRE_APPROX(x, y, del) \
        INTERNAL_SSTEST_ASSERTION_APPROX("REQUIRE_APPROX", x, y, del, INTERNAL_SSTEST_EXIT)

/**
 * \def REQUIRE_ALL
 * \brief Check if all arguments evaluate, or are implicitly converted to true
 * 
 * Save all arguments for printing
 */
#define REQUIRE_ALL(x, ...) \
        INTERNAL_SSTEST_ASSERTION_ALL("REQUIRE_ALL", INTERNAL_SSTEST_EXIT, x, __VA_ARGS__)

/**
 * \def REQUIRE_ANY
 * \brief Check if for all arguments, there exists one that evaluates, or is implicitly converted to true.
 * 
 * Save all arguments for printing
 */
#define REQUIRE_ANY(x, ...) \
        INTERNAL_SSTEST_ASSERTION_ANY("REQUIRE_ANY", INTERNAL_SSTEST_EXIT, x, __VA_ARGS__)

/**
 * \def REQUIRE_ASCENDING
 * \brief Check if the arguments are in ascending order, in order of the left most argument to the right most; equivalent to a using the <= operator between each adjacent pair of arguments
 * 
 * Assumes the <= operator is transitive (a <= b, b <= c -> a <= c)
 * 
 * Save all arguments for printing
 */
#define REQUIRE_ASCENDING(x, ...) \
        INTERNAL_SSTEST_ASSERTION_ASC("REQUIRE_ASCENDING", INTERNAL_SSTEST_EXIT, x, __VA_ARGS__)

/**
 * \def REQUIRE_DESCENDING
 * \brief Check if the arguments are in descending order, in order of the left most argument to the right most; equivalent to a using the >= operator between each adjacent pair of arguments
 * 
 * Assumes the >= operator is transitive (a >= b, b >= c -> a >= c)
 * 
 * Save all arguments for printing
 */
#define REQUIRE_DESCENDING(x, ...) \
        INTERNAL_SSTEST_ASSERTION_DESC("REQUIRE_DESCENDING", INTERNAL_SSTEST_EXIT, x, __VA_ARGS__)

/**
 * \def REQUIRE_PREDICATE
 * \brief Require that a custom condition is true, by supplying a custom predicate function (which returns a value convertible to bool) and its arguments
 * 
 * Save both predicate and any additional arguments for printing
 */
#define REQUIRE_PREDICATE(predicate, ...) \
        INTERNAL_SSTEST_ASSERTION_PRED("REQUIRE_PREDICATE", predicate, INTERNAL_SSTEST_EXIT, __VA_ARGS__)
        
//
// The EXPECT() family of arguments check a condition and continue test execution regardless of failure
// However, exceptions are not handled and will propagate until handled by the test runner
//

/**
 * \def EXPECT
 * \brief Check if a general expression evaluates to or is implicitly converted to true; continue test execution regardless of result 
 * 
 * Tries to collect and save as many arguments as possible to print later, if possible.
 * 
 * The expression's final result must be convertible to a bool, but intermediate types do not have to be convertible to bool
 */
#define EXPECT(x) \
        INTERNAL_SSTEST_ASSERTION_INFER("EXPECT", x, INTERNAL_SSTEST_CONTINUE)

/**
 * \def EXPECT_NOT
 * \brief Check if negation of expression evaluates to or is implicitly converted to true; continue test execution regardless of result 
 * 
 */
#define EXPECT_NOT(x) \
        INTERNAL_SSTEST_ASSERTION_NOT("EXPECT_NOT", x, INTERNAL_SSTEST_CONTINUE)

/**
 * \def EXPECT_TRUE
 * \brief Check if an expression evaluates to true
 * 
 * As opposed to REQUIRE(), this is meant for explicit bool types and will not save each argument in a multi-argument comparison
 */
#define EXPECT_TRUE(x) \
        INTERNAL_SSTEST_ASSERTION_BOOL_TRUE("EXPECT_TRUE", x, INTERNAL_SSTEST_CONTINUE)

/**
 * \def EXPECT_FALSE
 * \brief Check if an expression evaluatees to false
 * 
 * As opposed to REQUIRE(), this is meant for explicit bool types and will not save each argument in a multi-argument comparison
 */
#define EXPECT_FALSE(x) \
        INTERNAL_SSTEST_ASSERTION_BOOL_FALSE("EXPECT_FALSE", x, INTERNAL_SSTEST_CONTINUE)

/**
 * \def EXPECT_NULL
 * \brief Check if a value is equal to nullptr
 * 
 * Designed to be used for pointer types but if you type is equal comparable to nullptr_t, it can also be used as argument here
 */
#define EXPECT_NULL(x) \
        INTERNAL_SSTEST_ASSERTION_NULL("EXPECT_NULL", x, INTERNAL_SSTEST_CONTINUE)

/**
 * \def EXPECT_NOT_NULL
 * \brief Check if a value is not equal to nullptr
 * 
 * Designed to be used for pointer types but if you type is not equal comparable to nullptr_t, it can also be used as argument here
 */
#define EXPECT_NOT_NULL(x) \
        INTERNAL_SSTEST_ASSERTION_NOT_NULL("EXPECT_NOT_NULL", x, INTERNAL_SSTEST_CONTINUE)

/**
 * \def EXPECT_EQUAL
 * \brief Check if two values are equal; equivalent to using the == operator
 * 
 * Both arguments will be saved for printing
 * 
 * Assumes that the equal operator is symmetric (a == b -> b == a)
 */
#define EXPECT_EQUAL(x, y) \
        INTERNAL_SSTEST_ASSERTION_EQ2("EXPECT_EQUAL", x, y, INTERNAL_SSTEST_CONTINUE)

/**
 * \def EXPECT_NOT_EQUAL
 * \brief Check if two values are not equal; equivalent to using the != operator
 * 
 * Both arguments will be saved for printing
 * 
 * Assumes that the not equal operator is symmetric (a != b -> b != a)
 */
#define EXPECT_NOT_EQUAL(x, y) \
        INTERNAL_SSTEST_ASSERTION_NEQ2("EXPECT_NOT_EQUAL", x, y, INTERNAL_SSTEST_CONTINUE)

/**
 * \def EXPECT_LESS_EQUAL
 * \brief Check if the left hand side argument is less than or equal to the right hand side argument; equivalent to using the <= operator
 * 
 * Both arguments will be saved for printing
 */
#define EXPECT_LESS_EQUAL(x, y) \
        INTERNAL_SSTEST_ASSERTION_LE("EXPECT_LESS_EQUAL", x, y, INTERNAL_SSTEST_CONTINUE)

/**
 * \def EXPECT_GREATER_EQUAL
 * \brief Check if the left hand side argument is greater or equal to the right hand side argument; equivalent to using the >= operator
 * 
 * Both arguments will be saved for printing
 */
#define EXPECT_GREATER_EQUAL(x, y) \
        INTERNAL_SSTEST_ASSERTION_GE("EXPECT_GREATER_EQUAL", x, y, INTERNAL_SSTEST_CONTINUE)

/**
 * \def EXPECT_LESS
 * \brief Check if the left hand side argument is strictly less than the right hand side argument; equivalent to using the < operator
 * 
 * Both arguments will be saved for printing
 */
#define EXPECT_LESS(x, y) \
        INTERNAL_SSTEST_ASSERTION_LESS("EXPECT_LESS", x, y, INTERNAL_SSTEST_CONTINUE)

/**
 * \def EXPECT_GREATER
 * \brief Check if the left hand side argument is strictly greater than the right hand side argument; equivalent to using the > operator
 * 
 * Both arguments will be saved for printing
 */
#define EXPECT_GREATER(x, y) \
        INTERNAL_SSTEST_ASSERTION_GREATER("EXPECT_GREATER", x, y, INTERNAL_SSTEST_CONTINUE)

/**
 * \def EXPECT_ALL_EQUAL
 * \brief Check if all values are equal. All values are compared to the first argument, in O(n) comparisons. Equivalent to using == operator for each other argument as right hand side argument
 * 
 * Assumes that the equal operator is transitive and symmetric between each pair of types
 * 
 * All arguments are saved for printing
 * 
 * \sa EXPECT_ALL_EACH_EACH
 */
#define EXPECT_ALL_EQUAL(x, y, ...) \
        INTERNAL_SSTEST_ASSERTION_EQALL("EXPECT_ALL_EQUAL", INTERNAL_SSTEST_CONTINUE, x, y, __VA_ARGS__)

/**
 * \def EXPECT_ALL_EQUAL_EACH
 * \brief Check if all values are equal. All values are compared to each other, so requires O(n^2) comparisons. Equivalent to using == operator for each combination of two arguments
 * 
 * Useful if transitive property (a == b, b == c -> a == c) is not strictly true (e.g. floating point types)
 * 
 * Assumes that the equal operator is symmetric between each pair of types
 * 
 * All arguments are saved for printing
 */
#define EXPECT_ALL_EQUAL_EACH(x, y, ...) \
        INTERNAL_SSTEST_ASSERTION_EQALLEACH("EXPECT_ALL_EQUAL_EACH", INTERNAL_SSTEST_CONTINUE, x, y, __VA_ARGS__)

/**
 * \def EXPECT_FLOAT_EQUAL
 * \brief Check if two floating points are equal, within a very small lenience
 * 
 * Useful if a small amount of rounding and/or computation error is expected
 * 
 * Save both arguments for printing
 */
#define EXPECT_FLOAT_EQUAL(x, y) \
        INTERNAL_SSTEST_ASSERTION_EQFLOAT("EXPECT_FLOAT_EQUAL", x, y, INTERNAL_SSTEST_CONTINUE)

/**
 * \def EXPECT_DOUBLE_EQUAL
 * \brief Equivalent to EXPECT_FLOAT_EQUAL()
 * 
 * \sa EXPECT_FLOAT_EQUAL()
 */
#define EXPECT_DOUBLE_EQUAL(x, y) \
        INTERNAL_SSTEST_ASSERTION_EQFLOAT("EXPECT_DOUBLE_EQUAL", x, y, INTERNAL_SSTEST_CONTINUE)

/**
 * \def EXPECT_APPROX
 * \brief Check if two values are within a given delta
 * 
 * Requires the subtraction operator (-) be defined, and the result of the subtraction to be compared against the given delta
 * 
 * Save both comparison arguments and delta for printing
 */
#define EXPECT_APPROX(x, y, del) \
        INTERNAL_SSTEST_ASSERTION_APPROX("EXPECT_APPROX", x, y, del, INTERNAL_SSTEST_CONTINUE)

/**
 * \def EXPECT_ALL
 * \brief Check if all arguments evaluate, or are implicitly converted to true
 * 
 * Save all arguments for printing
 */
#define EXPECT_ALL(x, ...) \
        INTERNAL_SSTEST_ASSERTION_ALL("EXPECT_ALL", INTERNAL_SSTEST_CONTINUE, x, __VA_ARGS__)

/**
 * \def EXPECT_ANY
 * \brief Check if for all arguments, there exists one that evaluates, or is implicitly converted to true.
 * 
 * Save all arguments for printing
 */
#define EXPECT_ANY(x, ...) \
        INTERNAL_SSTEST_ASSERTION_ANY("EXPECT_ANY", INTERNAL_SSTEST_CONTINUE, x, __VA_ARGS__)

/**
 * \def EXPECT_ASCENDING
 * \brief Check if the arguments are in ascending order, in order of the left most argument to the right most; equivalent to a using the <= operator between each adjacent pair of arguments
 * 
 * Assumes the <= operator is transitive (a <= b, b <= c -> a <= c)
 * 
 * Save all arguments for printing
 */
#define EXPECT_ASCENDING(x, ...) \
        INTERNAL_SSTEST_ASSERTION_ASC("EXPECT_ASCENDING", INTERNAL_SSTEST_CONTINUE, x, __VA_ARGS__)

/**
 * \def EXPECT_DESCENDING
 * \brief Check if the arguments are in descending order, in order of the left most argument to the right most; equivalent to a using the >= operator between each adjacent pair of arguments
 * 
 * Assumes the >= operator is transitive (a >= b, b >= c -> a >= c)
 * 
 * Save all arguments for printing
 */
#define EXPECT_DESCENDING(x, ...) \
        INTERNAL_SSTEST_ASSERTION_DESC("EXPECT_DESCENDING", INTERNAL_SSTEST_CONTINUE, x, __VA_ARGS__)

/**
 * \def EXPECT_PREDICATE
 * \brief Expect that a custom condition is true, by supplying a custom predicate function (which returns a value convertible to bool) and its arguments
 * 
 * Save both predicate and any additional arguments for printing
 */
#define EXPECT_PREDICATE(predicate, ...) \
        INTERNAL_SSTEST_ASSERTION_PRED("EXPECT_PREDICATE", predicate, INTERNAL_SSTEST_CONTINUE, __VA_ARGS__)


// TODO EXPECT_THROWS etc
// TODO CHECK only reports value if false, but doesn't count towards a fail

/**
 * \def TEST
 * \brief Define a test case with an optional parent suite and name.
 * 
 * Definition of test body should immediately follow, as a C/C++ function with no return type (void). 
 * The test function defined will be automatically added to the test runner, and can be invoked later.
 * 
 * TEST() with no parameter will can only defined once per file and will be reported as an anonymous test.
 * 
 * TEST(<test name>) will define a test case with an identifier. Must be a valid C/C++ function name, and must be unique within a translation unit.
 * 
 * TEST(<test suite>, <test name>) will define a test suite, or possibly a test fixture if the first parameter is a complete type. The second parameter specifies the name within the test case, which must be unique within the suite/fixture.
 */
#define TEST(...) \
        INTERNAL_SSTEST_DEFINE_TEST(__VA_ARGS__)

/**
 * \def TEST_PARAMETERIZED_TEMPLATE
 * \brief Define a parameterized test case which takes an arbitrary number of user arguments
 * 
 * Definition of the test body should immediately follow, as a C/C++ function with no return type (void). 
 * Unlike TEST, the definition will not automatically be added to the test runner as the additional user arguments need to be instantiated with
 * TEST_PARAMETERIZED by referring to the current parameterized test definition.
 * 
 * The first parameter should be the test suite or fixture the template is used within.
 * 
 * The second parameter should be a name for the template, which must be unique within its containing sutie/fixture
 * 
 * The remaining parameters are regular function arguments, which can be used in the following test body.
 * 
 * Example: TEST_PARAMETERIZED_TEMPLATE(MyFixture, TwoIntsOneString, int x, int y, std::string str) 
 * 
 * \sa TEST_PARAMETERIZED
 */
#define TEST_PARAMETERIZED_TEMPLATE(...) \
        INTERNAL_SSTEST_TEST_PARAMETERIZED_TEMPLATE(__VA_ARGS__)

/**
 * \def TEST_PARAMETERIZED
 * \brief Instatiate an instance of a previously defined parameterized test template given its name with given values
 * 
 * No definition or body is required.
 * 
 * The first two parameters (suite/fixture, template name) must be identical to those used with TEST_PARAMETERIZED_TEMPLATE.
 * 
 * The third parameter gives should be a unique name for this configuration of the parameterized tests in use.
 * 
 * The remaning arguments are the values to call the template function with, which should match the types from the template. 
 */
#define TEST_PARAMETERIZED(...) \
        INTERNAL_SSTEST_TEST_PARAMETERIZED(__VA_ARGS__)



#endif // _SSTEST_INCLUDE_H_