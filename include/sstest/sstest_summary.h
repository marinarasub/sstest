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

#ifndef _SSTEST_SUMMARY_H_
#define _SSTEST_SUMMARY_H_

#include <cstddef>
#include <vector>
#include "sstest_config.h"

/**
 * \file sstest_summary.h
 * \brief Classes for containing test results 
 * 
 */

namespace sstest
{

    template <typename...> class Assertion;
    class TestInterface;
    class TestSuite;

    /**
     * \brief Data pack struct for tallies and test metrics
     * 
     */
    struct TestTotals
    {
        TestTotals operator+(const TestTotals&) noexcept;
        friend bool operator==(const TestTotals&, const TestTotals&);
        
        /**
         * \brief Zero intialize test counts
         * 
         */
        TestTotals() noexcept;

        /**
         * \brief Check if number of tests expected to run have passed
         * \note If not all tests expected to run have been reported, may still indicate not all passed 
         * 
         * \param pass_vacuous If true, pass in the case where total tests is 0
         * \param pass_skipped If true, only check number of passed tests matches number of tests that ran (rather than total tests)
         * \return true If all tests pass, considering the given parameters
         * \return false Else
         */
        bool allTestsPassed(bool pass_vacuous = true, bool pass_skipped = false) const noexcept;

        /**
         * \brief Check if number of assertions expected to run have passed
         * \note If not all assertions expected to run have been reported, may still indicate not all passed
         * 
         * \param pass_vacuous If true, pass in the case where total assertions is 0
         * \param pass_skipped If true, only check number of passed assertions matches number of tests that ran (rather than total assertions)
         * \return true If all assertions pass, considering given parameters
         * \return false Else
         */
        bool allAssertionsPassed(bool pass_vacuous = true, bool pass_skipped = false) const noexcept;
        bool validate() const; // throw except. if invalidated (make sure results make sense)
        void reset() noexcept;

        size_t test_functions_total;
        size_t test_functions_ran;
        size_t test_functions_passed;

        size_t test_suites_total;
        size_t test_suites_ran;
        size_t test_suites_passed;

        size_t assertions_total;
        size_t assertions_ran;
        size_t assertions_passed;
    };

    /**
     * \brief Wrapper around TestTotals with utility functions for integration with other sstest types
     * 
     */
    struct TestSummary
    {
    public:
        /**
         * \brief Create a blank test summary, where all totals are initialized to 0
         * 
         */
        TestSummary() noexcept;

        /**
         * \brief Initialize a test summary with the test suites that are going to be run
         * Unlike addTestSuiteResult(), does not count results, only pre-run information such as how many tests are going to run
         * \sa addTestSuiteResult()
         * 
         */
        TestSummary(const std::vector<TestSuite*>&) noexcept;

        /**
         * \brief Reset the test summary to a blank test summary
         * 
         */
        void reset() noexcept;

       /**
        * \brief Automatically update test totals with each test object in a test suite
        * \note If the test suite given if empty but ran, it will still be counted towards the summary
        * \sa TestSuite::ran()
        * 
        * \return Reference to *this
        */
        TestSummary& addTestSuiteResult(const TestSuite&);

        /**
         * \brief Automatically update test totals with an assertion result
         * 
         * \tparam Args 
         * \param assertion 
         * \return Reference to *this
         */
        template <typename... Args>
        inline TestSummary& addAssertionResult(const Assertion<Args...>& assertion)
        {
            if (assertion.passed()) totals.assertions_passed++;
            totals.assertions_total++;
            totals.assertions_ran++;
            return *this;
        }

        /**
         * \brief Return a copy of the test totals
         * 
         * \return TestTotals 
         */
        TestTotals getTotals() const noexcept;
        
        TestSummary operator+(const TestSummary&) noexcept;


    private:

        TestTotals totals;
    };
}

#endif // _SSTEST_SUMMARY_H_