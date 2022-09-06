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

#include "sstest/sstest_runner.h"
// todo delete iostream
#include <cassert>
#include <ostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <ratio>// for ratios

#include "sstest/sstest_timer.h"
#include "sstest/sstest_traits.h"
#include "sstest/sstest_timer.h"
#include "sstest/sstest_printer.h"
#include "sstest/sstest_test.h"
#include "sstest/sstest_registry.h"
#include "sstest/sstest_assertion.h"
#include "sstest/sstest_summary.h"
#include "sstest/sstest_console.h"
#include "sstest/sstest_config.h"
#include "sstest/sstest_utility.h"

namespace sstest
{

    void TestRunner::Reporter::StreamObject::clear()
    {
        ss.clear();
        ss.str("");
    }

    std::string TestRunner::Reporter::StreamObject::str() const
    {
        return ss.str();
    }

    TestRunner::Reporter::Reporter(const Logger& logger, const TestRunner::Configuration& settings, size_t status_width)
        : settings(settings), status_width(status_width)
    {
        addLogger(logger);
    }

    size_t TestRunner::Reporter::addLogger(const Logger& logger)
    {
        size_t n = loggers.size();
        loggers.push_back(logger);
        return n;
    }

    Logger TestRunner::Reporter::getLogger(size_t id) const
    {
        return loggers[id];
    }

    void TestRunner::Reporter::forEachLogger(std::function<void(Logger&)> func) const
    {
        for (Logger logger : loggers)
        {
            func(logger);
        }
    }

    void TestRunner::Reporter::reportInitialized() const
    {
        forEachLogger([&](Logger& logger) -> void
        {
            logger << std::string(80, '~') << '\n';
            logger << "SSTest version " << version_string << '\n';
            logger.time();
            logger << std::string(80, '~') << std::endl; // TODO terminal wdith
            //logger << std::endl;
        });
    }

    void TestRunner::Reporter::reportGlobalBegin(const TestSummary& summary) const
    {
        std::string start_text;
        std::string header;
        const size_t ntest_functions = summary.getTotals().test_functions_total;
        const size_t ntest_suites = summary.getTotals().test_suites_total;
        if (ntest_functions == 0)
        {
            start_text = std::string(status_width, '-');
            header = std::string("No test suites to run");
        }
        else
        {
            start_text = "START";
            header = std::string("Running ") + std::to_string(ntest_functions) + " tests in " + std::to_string(ntest_suites) + " test suites"; // TODO
        }
        forEachLogger([&](Logger& logger) -> void
        {
            printStatus(logger, start_text, Logger::ANSITextColor::ANSI_GREEN, HorizontalAlignment::CENTER);
            logger.writeLine(header);
            logger.writeLine(); // extra line
        });
    }

    void TestRunner::Reporter::reportGlobalSummary(const TestSummary& summary, const std::vector<TestSuite*> suites) const
    {
        if (suites.empty()) return;

        constexpr StringView end_text = "FINISH";
        //StringView result_text;
        std::string header;
        const size_t ntest_functions = summary.getTotals().test_functions_total;
        const size_t ntest_suites = summary.getTotals().test_suites_total;
        header = std::string("Summary: ") + std::to_string(ntest_functions) + " tests in " + std::to_string(ntest_suites) + " test suites";

        // filter by result
        const std::vector<TestSuite*> suites_passed = filter(suites, [](const TestSuite* suite) -> bool { return suite->passed(); });
        const std::vector<TestSuite*> suites_failed = filter(suites, [](const TestSuite* suite) -> bool { return !suite->passed(); });

        forEachLogger([&](Logger& logger) -> void 
        {
            logger.writeLine();
            printStatus(logger, end_text, Logger::ANSITextColor::ANSI_GREEN, HorizontalAlignment::CENTER);
            logger.writeLine(header); // TODO
            logger.writeLine();

            if (!suites_passed.empty())
            {
                printStatus(logger, std::string(status_width, '-'), Logger::ANSITextColor::ANSI_GREEN, HorizontalAlignment::CENTER);
                logger.writeLine("Passed: ");
                listTestCaseResults(logger, suites_passed);
            }
            // TODO filter instead of this
            if (!suites_failed.empty())
            {
                // extra space if pased was printed
                if (!suites_passed.empty()) logger.writeLine();
                printStatus(logger, std::string(status_width, '-'), Logger::ANSITextColor::ANSI_RED, HorizontalAlignment::CENTER);
                logger.writeLine("Failed: ");
                listTestCaseResults(logger, suites_failed);
            }
        });
    }

    void TestRunner::Reporter::reportGlobalResult(const TestSummary& summary, const std::string& info) const
    {
       // std::string footer = // TODO some kind of to string
        Logger::ANSITextColor result_clr = summary.getTotals().allTestsPassed() ? 
            Logger::ANSITextColor::ANSI_GREEN : 
            Logger::ANSITextColor::ANSI_RED; // do yellwo for partial
        std::string footer;
        TestTotals totals = summary.getTotals();
        size_t ntests = totals.test_functions_total;
        if (ntests == 0)
        {
            return;   
        }
        else
        {
            // TODO also rpint tet cases passed, skipped
            footer = std::to_string(totals.test_suites_passed) + "/" + std::to_string(totals.test_suites_ran) +
                " test suites passed, " + std::to_string(totals.test_suites_total - totals.test_suites_ran) + " skipped" + " (" +
                std::to_string(totals.assertions_passed) + "/" + std::to_string(totals.assertions_ran) + " assertions passed)";
        }
        forEachLogger([&](Logger& logger) -> void
        {
            logger << "\n";
            printStatus(logger, std::string(status_width, '-'), result_clr, HorizontalAlignment::CENTER);
            logger.writeLine(footer, result_clr, Logger::ANSITextFormat::ANSI_BOLD);
            // total time TODO
            printStatus(logger, std::string(status_width, '-'), Logger::ANSITextColor::ANSI_GREEN, HorizontalAlignment::CENTER);
            logger.writeLine(info);
            logger << std::endl;
        });
    }

    void TestRunner::Reporter::reportTestBegin(const TestInterface& test) const
    {
        forEachLogger([&](Logger& logger) -> void
        {
            printStatus(logger, "RUN", Logger::ANSITextColor::ANSI_GREEN, HorizontalAlignment::LEFT);
            test.name() ? logger.writeLine(test.name()) : logger.writeLine("<anonymous>");
        });
    }

    void TestRunner::Reporter::reportTestResult(const TestInterface& test, const std::string& info) const
    {
        forEachLogger([&](Logger& logger) -> void
        {
            switch (test.result())
            {
            case TestResult::PASS:
                printStatus(logger, "OK", Logger::ANSITextColor::ANSI_GREEN, HorizontalAlignment::RIGHT);
                break;
            case TestResult::THROW:
                logger.writeLine("test body threw exception TODO"); // TODO
                printStatus(logger, "FAIL", Logger::ANSITextColor::ANSI_RED, HorizontalAlignment::RIGHT);
                break;
            case TestResult::FAIL:
                printStatus(logger, "FAIL", Logger::ANSITextColor::ANSI_RED, HorizontalAlignment::RIGHT);
                break;
            case TestResult::INVALID:
            default:
                throw Exception("internal: Invalid test result given to reportTestResult()");
                break;
            }
            logger.write(test.name()); // TODO write time taken.
            logger.writeLine(info);

        });
    }

    void TestRunner::Reporter::reportTestCaseBegin(const TestSuite& tc) const
    {
        forEachLogger([&](Logger& logger) -> void
        {
            printStatus(logger, "TEST", Logger::ANSITextColor::ANSI_GREEN, HorizontalAlignment::LEFT);
            tc.name().empty() ? logger.write("<global>") : logger.write(tc.name());
            logger.writeLine();
            printStatus(logger, std::string(status_width, '-'), Logger::ANSITextColor::ANSI_GREEN, HorizontalAlignment::CENTER);
            logger.writeLine();
        });
    }

    void TestRunner::Reporter::reportTestCaseResult(const TestSuite& tc, const std::string& info) const
    {
        forEachLogger([&](Logger& logger) -> void
        {
            Logger::ANSITextColor clr;
            StringView stat_text;
            if (tc.passed())
            {
                clr = Logger::ANSITextColor::ANSI_GREEN;
                stat_text = "PASSED";
            }
            else 
            {
                clr = Logger::ANSITextColor::ANSI_RED;
                stat_text = "FAILED";
            }
            printStatus(logger, std::string(status_width, '-'), clr, HorizontalAlignment::CENTER);
            logger.writeLine();
            printStatus(logger, stat_text, clr, HorizontalAlignment::RIGHT);
            tc.name().empty() ? logger.write("<global>") : logger.write(tc.name()); // TODO write time taken.
            logger << " ";
            logger.writeLine(info);
            logger.writeLine();
        });

    }

    void TestRunner::Reporter::listTestCaseResults(Logger& logger, const std::vector<TestSuite*> suites) const
    {
        StringView result_text;
        Logger::ANSITextColor result_clr;
        for (const TestSuite* suite : suites)
        {
            if (suite->passed()) { result_text = "PASSED"; result_clr = Logger::ANSITextColor::ANSI_GREEN; }
            else { result_text = "FAILED"; result_clr = Logger::ANSITextColor::ANSI_RED; }
            printStatus(logger, result_text, result_clr, HorizontalAlignment::CENTER);
            logger.write(suite->name() ? suite->name() : "<global>");
            logger.writeLine(" (" + std::to_string(suite->numTestsPassed()) + "/" + std::to_string(suite->numTestsRan()) + ")", result_clr);
        }
    }

    void TestRunner::Reporter::reportException(const std::exception& e) const
    {
        forEachLogger([&](Logger& logger) -> void
        {
            logger << "exception: " << e.what() << '\n';   
        });
    }

    void TestRunner::Reporter::reportExplicitFailure() const
    {
        forEachLogger([&](Logger& logger) -> void
        {
            logger << ("explicit failure") << '\n'; 
        });
    }

    void TestRunner::Reporter::printStatus(
        Logger& logger,
        const std::string& status_text, 
        Logger::ANSITextColor clr,
        HorizontalAlignment align) const
    {
        logger.write(statusBadge(status_text, status_width + 4, align) + " ", clr);
    }

    std::string TestRunner::Reporter::statusBadge(std::string str, size_t width, HorizontalAlignment align)
    {
        // check min width: 4 + 1
        size_t slen = str.length();
        if (slen + 4 > width) width = slen + 4;//throw InvalidArgument("status bar too small to fit");
        // calcualte extra spaces
        size_t extra_spaces = width - (slen + 4);
        std::string rv;
        switch (align)
        {
        case HorizontalAlignment::LEFT:
            rv = "[ " + str + std::string(extra_spaces, ' ') + " ]";
            break;
        case HorizontalAlignment::RIGHT:
            rv = "[ " +  std::string(extra_spaces, ' ') + str + " ]";
            break;
        case HorizontalAlignment::CENTER: // fallthrough
        default:
            rv = "[ " + std::string(extra_spaces/2, ' ') + str + std::string(extra_spaces/2 + (extra_spaces%2), ' ') + " ]";
            break;
        //default:
           // throw SSException("invalid text align");
        }
        return rv;
    }

    const TestRunner::Configuration TestRunner::Configuration::default_settings = Configuration(false, true, false, true);

    void TestRunner::Configuration::reset() noexcept
    {
        *this = TestRunner::Configuration::default_settings;
    }

    TestRunner::TestRunner()
        : registry_(new TestRegistry), 
        curr_test(nullptr), 
        settings(TestRunner::Configuration::default_settings),
        reporter_(new TestRunner::Reporter(Logger(std::cout, true), settings)) 
    { 
        // TODO !!! this is just for testing.
        static auto fos = std::ofstream("test.log", std::ofstream::out);
        reporter_->addLogger(Logger(fos, false));

        reporter_->reportInitialized();
    }

    TestRunner::~TestRunner()
    {
        assert(registry_);
        assert(curr_test == nullptr);
        delete registry_;
        assert(reporter_);
        delete reporter_;
    }

    TestRunner::Configuration& TestRunner::configure(const TestRunner::Configuration* new_settings) noexcept
    {
        if (new_settings != nullptr) settings = *new_settings;
        return settings;
    }

    TestRegistry& TestRunner::registry()
    {
        return *registry_;
    }

    TestRunner::Reporter& TestRunner::reporter()
    {
        assert(reporter_);
        return *reporter_;
    }

    TestSummary TestRunner::runAllTests()
    {
        std::vector<TestSuite*> test_list = registry_->getTestCases(false, [](const TestSuite* lhs, const TestSuite* rhs) -> bool {
            return lhs->name() < rhs->name();
        });
        return runTestCasesHelper(test_list);
    }
    
    TestSummary TestRunner::runTests(std::vector<StringView> names)
    {
        std::vector<TestSuite*> test_list;
        for (const StringView& name : names)
        {
            TestSuite* suite = registry_->getTestCase(name);
            if (!suite->empty()) test_list.push_back(suite);
        } 
        return runTestCasesHelper(test_list);
    }

    void TestRunner::explicitFailure()
    {
        reporter_->reportExplicitFailure();
        curr_test->fail();
    }

    // TODO limit max n tests to max int. (very reasonable)

    TestSummary TestRunner::runTestCasesHelper(const std::vector<TestSuite*> suites)
    {
        test_summary = TestSummary(suites);
        reporter_->reportGlobalBegin(test_summary);

        Configuration config = this->settings; // save config, which can be modified per test
        Stopwatch timer;
        timer.start();
        for (TestSuite* suite : suites)
        {
            assert(suite != nullptr);
            reporter_->reportTestCaseBegin(*suite);
            timer.lap();
            
            suite->run(
                [&](TestInterface& test) -> void 
                { 
                    curr_test = &test;  
                    reporter_->reportTestBegin(test); 
                    this->settings = config; // reset to original pre test
                },
                [&](TestInterface& test) -> void 
                { 
                    curr_test = nullptr; 
                    reporter_->reportTestResult(test); /*test_summary.addTestResult(test);*/ 
                }
            );
            std::chrono::milliseconds::rep ms = timer.lap<std::chrono::milliseconds>().count();
            curr_test = nullptr;
            reporter_->reportTestCaseResult(*suite, std::string("(") + std::to_string(ms) + " ms)");
            test_summary.addTestSuiteResult(*suite);
        }

        std::chrono::milliseconds::rep total_ms = timer.stop<std::chrono::milliseconds>().count();

        reporter_->reportGlobalSummary(test_summary, suites); // if (SUMMARIZE_TESTS) for each printf [FAILED/PASSED] name
        reporter_->reportGlobalResult(test_summary, "total time: " + std::to_string(total_ms) + " ms");
        
        test_summary.getTotals().validate();
        return test_summary;
    }
    
}
