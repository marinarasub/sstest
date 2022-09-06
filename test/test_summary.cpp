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

#include "sstest/sstest_summary.h"
#include "sstest/sstest_assertion.h"
#include "sstest/sstest_test.h"
#include <vector>

/**
 * This class test TestSummary functionality
 */

using namespace sstest;

CTEST_DEFINE_TEST(test_summary_construct_blank)
{
    TestSummary summary;
    CTEST_ASSERT(summary.getTotals() == TestTotals());
    TestTotals totals;
    
    totals = summary.getTotals();
    CTEST_ASSERT(totals.allAssertionsPassed(true) == true);
    CTEST_ASSERT(totals.allAssertionsPassed(false) == false);
    CTEST_ASSERT(totals.allTestsPassed(true) == true);
    CTEST_ASSERT(totals.allTestsPassed(false) == false);

    CTEST_ASSERT(totals.validate());
}

CTEST_DEFINE_TEST(test_totals_reset)
{
    TestTotals totals;
    totals.test_suites_total = 1;
    totals.test_suites_ran = 2;
    totals.test_suites_passed = 3;
    totals.test_functions_total = 4;
    totals.test_functions_ran = 5;
    totals.test_functions_passed = 6;
    totals.assertions_total = 7;
    totals.assertions_ran = 8;
    totals.assertions_passed = 9;

    totals.reset();
    CTEST_ASSERT(totals == TestTotals());
}

CTEST_DEFINE_TEST(test_summary_assertion)
{
    TestSummary summary;
    TestTotals totals;

    totals = summary.getTotals();
    CTEST_ASSERT(totals.allAssertionsPassed(true) == true);
    CTEST_ASSERT(totals.allAssertionsPassed(false) == false);
    CTEST_ASSERT(totals.allTestsPassed(true) == true);
    CTEST_ASSERT(totals.allTestsPassed(false) == false);
    CTEST_ASSERT(totals.test_suites_total == 0);
    CTEST_ASSERT(totals.test_suites_ran == 0);
    CTEST_ASSERT(totals.test_suites_passed == 0);
    CTEST_ASSERT(totals.test_functions_total == 0);
    CTEST_ASSERT(totals.test_functions_ran == 0);
    CTEST_ASSERT(totals.test_functions_passed == 0);
    CTEST_ASSERT(totals.assertions_total == 0);
    CTEST_ASSERT(totals.assertions_ran == 0);
    CTEST_ASSERT(totals.assertions_passed == 0);
    CTEST_ASSERT(totals.validate());

    Assertion<bool> pass(TestInfo(""), LineInfo("", 0), "", true);
    summary.addAssertionResult(pass);
    totals = summary.getTotals();
    CTEST_ASSERT(totals.allAssertionsPassed(true) == true);
    CTEST_ASSERT(totals.allAssertionsPassed(false) == true);
    CTEST_ASSERT(totals.allTestsPassed(true) == true);
    CTEST_ASSERT(totals.allTestsPassed(false) == false);
    CTEST_ASSERT(totals.test_suites_total == 0);
    CTEST_ASSERT(totals.test_suites_ran == 0);
    CTEST_ASSERT(totals.test_suites_passed == 0);
    CTEST_ASSERT(totals.test_functions_total == 0);
    CTEST_ASSERT(totals.test_functions_ran == 0);
    CTEST_ASSERT(totals.test_functions_passed == 0);
    CTEST_ASSERT(totals.assertions_total == 1);
    CTEST_ASSERT(totals.assertions_ran == 1);
    CTEST_ASSERT(totals.assertions_passed == 1);
    CTEST_ASSERT(totals.validate());

    Assertion<bool> fail;
    summary.addAssertionResult(fail);
    totals = summary.getTotals();
    CTEST_ASSERT(totals.allAssertionsPassed(true) == false);
    CTEST_ASSERT(totals.allAssertionsPassed(false) == false);
    CTEST_ASSERT(totals.allTestsPassed(true) == true);
    CTEST_ASSERT(totals.allTestsPassed(false) == false);
    CTEST_ASSERT(totals.test_suites_total == 0);
    CTEST_ASSERT(totals.test_suites_ran == 0);
    CTEST_ASSERT(totals.test_suites_passed == 0);
    CTEST_ASSERT(totals.test_functions_total == 0);
    CTEST_ASSERT(totals.test_functions_ran == 0);
    CTEST_ASSERT(totals.test_functions_passed == 0);
    CTEST_ASSERT(totals.assertions_total == 2);
    CTEST_ASSERT(totals.assertions_ran == 2);
    CTEST_ASSERT(totals.assertions_passed == 1);
    CTEST_ASSERT(totals.validate());
}  

CTEST_DEFINE_TEST(test_summary_empty_suite)
{
    TestSuite suite(TestInfo(""));
    TestSummary summary(std::vector<TestSuite*>{ &suite });
    TestTotals totals;

    totals = summary.getTotals();
    CTEST_ASSERT(totals.allAssertionsPassed(true) == true);
    CTEST_ASSERT(totals.allAssertionsPassed(false) == false);
    CTEST_ASSERT(totals.allTestsPassed(true) == true);
    CTEST_ASSERT(totals.allTestsPassed(false) == false);
    CTEST_ASSERT(totals.test_suites_total == 1);
    CTEST_ASSERT(totals.test_suites_ran == 0);
    CTEST_ASSERT(totals.test_suites_passed == 0);
    CTEST_ASSERT(totals.test_functions_total == 0);
    CTEST_ASSERT(totals.test_functions_ran == 0);
    CTEST_ASSERT(totals.test_functions_passed == 0);
    CTEST_ASSERT(totals.assertions_total == 0);
    CTEST_ASSERT(totals.assertions_ran == 0);
    CTEST_ASSERT(totals.assertions_passed == 0);
    CTEST_ASSERT(totals.validate());

    suite.run();
    summary.addTestSuiteResult(suite);
    totals = summary.getTotals();
    CTEST_ASSERT(totals.allAssertionsPassed(true) == true);
    CTEST_ASSERT(totals.allAssertionsPassed(false) == false);
    CTEST_ASSERT(totals.allTestsPassed(true) == true);
    CTEST_ASSERT(totals.allTestsPassed(false) == false);
    CTEST_ASSERT(totals.test_suites_total == 1);
    CTEST_ASSERT(totals.test_suites_ran == 1);
    CTEST_ASSERT(totals.test_suites_passed == 1);
    CTEST_ASSERT(totals.test_functions_total == 0);
    CTEST_ASSERT(totals.test_functions_ran == 0);
    CTEST_ASSERT(totals.test_functions_passed == 0);
    CTEST_ASSERT(totals.assertions_total == 0);
    CTEST_ASSERT(totals.assertions_ran == 0);
    CTEST_ASSERT(totals.assertions_passed == 0);
    CTEST_ASSERT(totals.validate());
}

CTEST_DEFINE_TEST(test_summary_single_test)
{
    TestFunction test(TestInfo(""), LineInfo("", 0), []() {});
    TestSuite suite(TestInfo(""));
    suite.addTest(test);
    TestSummary summary(std::vector<TestSuite*>{ &suite });
    TestTotals totals;

    totals = summary.getTotals();
    CTEST_ASSERT(totals.allAssertionsPassed(true) == true);
    CTEST_ASSERT(totals.allAssertionsPassed(false) == false);
    CTEST_ASSERT(totals.allTestsPassed(true) == true);
    CTEST_ASSERT(totals.allTestsPassed(false) == false);
    CTEST_ASSERT(totals.test_suites_total == 1);
    CTEST_ASSERT(totals.test_suites_ran == 0);
    CTEST_ASSERT(totals.test_suites_passed == 0);
    CTEST_ASSERT(totals.test_functions_total == 1);
    CTEST_ASSERT(totals.test_functions_ran == 0);
    CTEST_ASSERT(totals.test_functions_passed == 0);
    CTEST_ASSERT(totals.assertions_total == 0);
    CTEST_ASSERT(totals.assertions_ran == 0);
    CTEST_ASSERT(totals.assertions_passed == 0);
    CTEST_ASSERT(totals.validate());

    suite.run();
    summary.addTestSuiteResult(suite);
    totals = summary.getTotals();
    CTEST_ASSERT(totals.allAssertionsPassed(true) == true);
    CTEST_ASSERT(totals.allAssertionsPassed(false) == false);
    CTEST_ASSERT(totals.allTestsPassed(true) == true);
    CTEST_ASSERT(totals.allTestsPassed(false) == true);
    CTEST_ASSERT(totals.test_suites_total == 1);
    CTEST_ASSERT(totals.test_suites_ran == 1);
    CTEST_ASSERT(totals.test_suites_passed == 1);
    CTEST_ASSERT(totals.test_functions_total == 1);
    CTEST_ASSERT(totals.test_functions_ran == 1);
    CTEST_ASSERT(totals.test_functions_passed == 1);
    CTEST_ASSERT(totals.assertions_total == 0);
    CTEST_ASSERT(totals.assertions_ran == 0);
    CTEST_ASSERT(totals.assertions_passed == 0);
    CTEST_ASSERT(totals.validate());
}

CTEST_DEFINE_TEST(test_summary_multi_test)
{
    TestSuite suiteA(TestInfo("A"));
    TestSuite suiteB(TestInfo("B"));
    TestFunction testA1(TestInfo("1"), LineInfo("", 0), []() {});
    TestFunction testA2(TestInfo("2"), LineInfo("", 0), [&]() { suiteA.getTest("A").fail(); });
    suiteA.addTest(testA1);
    suiteA.addTest(testA2);
    TestFunction testB1(TestInfo("1"), LineInfo("", 0), []() {});
    suiteB.addTest(testB1);
    TestSummary summary(std::vector<TestSuite*>{ &suiteA, &suiteB });
    TestTotals totals;

    totals = summary.getTotals();
    CTEST_ASSERT(totals.allTestsPassed(true) == true);
    CTEST_ASSERT(totals.allTestsPassed(false) == false);
    CTEST_ASSERT(totals.test_suites_total == 2);
    CTEST_ASSERT(totals.test_suites_ran == 0);
    CTEST_ASSERT(totals.test_suites_passed == 0);
    CTEST_ASSERT(totals.test_functions_total == 3);
    CTEST_ASSERT(totals.test_functions_ran == 0);
    CTEST_ASSERT(totals.test_functions_passed == 0);
    CTEST_ASSERT(totals.validate());

    suiteB.run();
    summary.addTestSuiteResult(suiteB);
    totals = summary.getTotals();
    CTEST_ASSERT(totals.allTestsPassed(true, false) == false);
    CTEST_ASSERT(totals.allTestsPassed(false, false) == false);
    CTEST_ASSERT(totals.allTestsPassed(true, true) == true);
    CTEST_ASSERT(totals.allTestsPassed(false, true) == true);
    CTEST_ASSERT(totals.test_suites_total == 2);
    CTEST_ASSERT(totals.test_suites_ran == 1);
    CTEST_ASSERT(totals.test_suites_passed == 1);
    CTEST_ASSERT(totals.test_functions_total == 3);
    CTEST_ASSERT(totals.test_functions_ran == 1);
    CTEST_ASSERT(totals.test_functions_passed == 1);
    CTEST_ASSERT(totals.validate());

    suiteA.run();
    summary.addTestSuiteResult(suiteA);
    totals = summary.getTotals();
    CTEST_ASSERT(totals.allTestsPassed(true, false) == false);
    CTEST_ASSERT(totals.allTestsPassed(false, false) == false);
    CTEST_ASSERT(totals.allTestsPassed(true, true) == false);
    CTEST_ASSERT(totals.allTestsPassed(false, true) == false);
    CTEST_ASSERT(totals.test_suites_total == 2);
    CTEST_ASSERT(totals.test_suites_ran == 2);
    CTEST_ASSERT(totals.test_suites_passed == 1);
    CTEST_ASSERT(totals.test_functions_total == 3);
    CTEST_ASSERT(totals.test_functions_ran == 3);
    CTEST_ASSERT(totals.test_functions_passed == 2);
    CTEST_ASSERT(totals.validate());
}

int main()
{
    CTEST_RUN_TEST(test_summary_construct_blank);
    CTEST_RUN_TEST(test_totals_reset);
    CTEST_RUN_TEST(test_summary_assertion);
    CTEST_RUN_TEST(test_summary_empty_suite);
    CTEST_RUN_TEST(test_summary_single_test);
    CTEST_RUN_TEST(test_summary_multi_test);

    return CTEST_SUCCESS;
}