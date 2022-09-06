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

#include "sstest/sstest_summary.h"

#include <cstddef>
#include <limits>
#include <vector>
#include <cassert>

#include "sstest/sstest_exception.h"
#include "sstest/sstest_assertion.h"
#include "sstest/sstest_test.h"

namespace sstest
{

    TestTotals TestTotals::operator+(const TestTotals& rhs) noexcept
    {
        TestTotals ret;
        ret.test_functions_total = this->test_functions_total + rhs.test_functions_total;
        ret.test_functions_ran = this->test_functions_ran + rhs.test_functions_ran;
        //ret.test_functions_skipped = this->test_functions_skipped + rhs.test_functions_skipped;
        ret.test_functions_passed = this->test_functions_passed + rhs.test_functions_passed;
        ret.test_suites_total = this->test_functions_total + rhs.test_functions_total;
        ret.test_suites_ran = this->test_functions_ran + rhs.test_functions_ran;
        //ret.test_cases_skipped = this->test_functions_skipped + rhs.test_functions_skipped;
        ret.test_suites_passed = this->test_functions_passed + rhs.test_functions_passed;
        ret.assertions_total = this->assertions_total + rhs.assertions_total;
        ret.assertions_ran = this->assertions_total + rhs.assertions_total;
        //ret.assertions_skipped = this->assertions_total + rhs.assertions_total;
        ret.assertions_passed = this->assertions_total + rhs.assertions_total;
        // TODO add more if needed
        return ret;
    }

    TestTotals::TestTotals() noexcept
        : test_functions_total(0), 
        test_functions_ran(0), 
        //test_functions_skipped(0), 
        test_functions_passed(0),
        test_suites_total(0), 
        test_suites_ran(0), 
        //test_cases_skipped(0), 
        test_suites_passed(0),
        assertions_total(0), 
        assertions_ran(0), 
        //assertions_skipped(0), 
        assertions_passed(0)
    {

    }

    bool operator==(const TestTotals& lhs, const TestTotals& rhs)
    {
        return 
            (lhs.test_functions_total   ==  rhs.test_functions_total)   &&
            (lhs.test_functions_ran     ==  rhs.test_functions_ran)     &&
            (lhs.test_functions_passed  ==  rhs.test_functions_passed)  &&
            (lhs.test_suites_total      ==  rhs.test_suites_total)      &&
            (lhs.test_suites_ran        ==  rhs.test_suites_ran)        &&
            (lhs.test_suites_passed     ==  rhs.test_suites_passed)     &&
            (lhs.assertions_total       ==  rhs.assertions_total)       &&
            (lhs.assertions_ran         ==  rhs.assertions_ran)         &&
            (lhs.assertions_passed      ==  rhs.assertions_passed);
    }

    bool TestTotals::allTestsPassed(bool pass_vacuous, bool pass_skipped) const noexcept
    {
        if (test_functions_ran == 0) return pass_vacuous;
        size_t total_counted = pass_skipped ? test_functions_ran : test_functions_total;
        return (total_counted == test_functions_passed);
    }

    bool TestTotals::allAssertionsPassed(bool pass_vacuous, bool pass_skipped) const noexcept
    {
        if (!assertions_ran) return pass_vacuous;
        size_t total_counted = pass_skipped ? assertions_ran : assertions_total;
        return (total_counted == assertions_passed);
    }

    // todo replace this with private/public member/interface so u cant make invalid
    bool TestTotals::validate() const
    {
        return 
            (test_functions_total   >=  test_functions_ran)     &&
            (test_functions_total   >=  test_functions_passed)  &&
            (test_functions_ran     >=  test_functions_passed)  && 
            (test_suites_total      >=  test_suites_ran)        &&
            (test_suites_total      >=  test_suites_passed)     &&
            (test_suites_ran        >=  test_suites_passed)     &&
            (assertions_total       >=  assertions_ran)         &&
            (assertions_total       >=  assertions_passed)      &&
            (assertions_ran         >=  assertions_passed);
    }

    void TestTotals::reset() noexcept
    {
        *this = TestTotals();
    }

    TestSummary::TestSummary() noexcept
    {

    }

    TestSummary::TestSummary(const std::vector<TestSuite*>& suites) noexcept
    {
        this->totals.test_suites_total = suites.size();
        for (const TestSuite* suite : suites)
        {
            this->totals.test_functions_total += suite->size();
        }
    }

    void TestSummary::reset() noexcept
    {
        totals.reset();
    }

    TestTotals TestSummary::getTotals() const noexcept
    {
        return totals;
    }

    TestSummary TestSummary::operator+(const TestSummary& rhs) noexcept
    {
        TestSummary ret;
        ret.totals = this->totals + rhs.totals;
        return ret;
    }

    TestSummary& TestSummary::addTestSuiteResult(const TestSuite& suite)
    {
        totals.test_suites_passed += suite.passed() ? 1 : 0;
        totals.test_suites_ran += 1;

        totals.test_functions_passed += suite.numTestsPassed();
        totals.test_functions_ran += suite.numTestsRan();//size();
        return *this;
    }
    
}
