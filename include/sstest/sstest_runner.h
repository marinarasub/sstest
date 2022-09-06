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

#ifndef _SSTEST_RUNNER_H_
#define _SSTEST_RUNNER_H_

#include <vector>
#include <functional>
#include <sstream>
#include "sstest_traits.h"
#include "sstest_printer.h"
#include "sstest_test.h"
#include "sstest_registry.h"
#include "sstest_assertion.h"
#include "sstest_summary.h"
#include "sstest_console.h"
#include "sstest_compare.h"


// TODO malloc/realloc/free hook to detect leaks
// TODO performance testing

/**
 * \file sstest_runner.h
 * \brief Class definition of a singleton test runner that tracks everything that happens using SSTest
 * 
 */

namespace sstest
{

    class Stopwatch;
    class Registry;
    struct TestSummary;

    /**
     * \brief Singleton test runner for sstest. Manages and handles test registration, configuration and running.
     * All requests to sstest should be made through instance of TestRunner
     * 
     */
    class TestRunner
    {
    
    public:
                // struct for storing test runner settings
        struct Configuration
        {
            constexpr Configuration() 
                : show_assertion_pass(false), 
                show_assertion_fail(false),
                expand_args_assertion_pass(false),
                expand_args_assertion_fail(false),
                max_assertions(0),
                max_tests(0)
            {}

            constexpr Configuration(bool show_assertion_pass, bool show_assertion_fail, bool expand_args_assertion_pass, bool expand_args_assertion_fail) 
                : show_assertion_pass(show_assertion_pass), 
                show_assertion_fail(show_assertion_fail),
                expand_args_assertion_pass(expand_args_assertion_pass),
                expand_args_assertion_fail(expand_args_assertion_fail),
                max_assertions(0),
                max_tests(0)
            {}

            static const Configuration default_settings;

            Configuration(const Configuration&) = default;
            Configuration& operator=(const Configuration&) = default;

            inline Configuration operator||(const Configuration& other)
            {
                this->show_assertion_pass = this->show_assertion_pass || other.show_assertion_pass;
                this->show_assertion_fail = this->show_assertion_fail || other.show_assertion_fail;
                this->expand_args_assertion_pass = this->expand_args_assertion_pass || other.expand_args_assertion_pass;
                this->expand_args_assertion_fail = this->expand_args_assertion_fail || other.expand_args_assertion_fail;
                return *this;
            }

            void reset() noexcept;

            bool show_assertion_pass;
            bool show_assertion_fail;
            bool expand_args_assertion_pass;
            bool expand_args_assertion_fail;
            size_t max_assertions;
            size_t max_tests;
            //size_t timeout;
            //detail level 0,1,2,3 etc
            //bool display_percentages
        };


        class Reporter
        {
        public:

            static constexpr const size_t MAX_STATUS_WIDTH = 8;

            struct StreamObject
            {
                //Message& operator<<(const std::string& msg);

                inline StreamObject& operator<<(const char* msg)
                {
                    ss << msg;
                    return *this;
                }

                inline StreamObject& operator<<(const std::string& msg)
                {
                    ss << msg;
                    return *this;
                }

                // call streamed function TODO probably needs better name. problem is, this only gets used on faiil macro, but it actually can be used elsewhere
                inline StreamObject& callback(std::function<void()> cb)
                {
                    cb();
                    return *this;
                }

                template <typename T>
                StreamObject& operator<<(const T& msg)
                {
                    ss << formatTypeAsString(msg);
                    return *this;
                }

                std::string str() const;
                void clear();
                inline bool operator!() const
                {
                    return ss.rdbuf()->in_avail() == 0;
                }
            private:
                std::stringstream ss;
            };


        public:

            explicit Reporter(const Logger&, const Configuration&, size_t status_width = MAX_STATUS_WIDTH);
            // return logger id
            size_t addLogger(const Logger&);

            Logger getLogger(size_t id) const;

            void forEachLogger(std::function<void(Logger&)> func) const;

            inline void message(const std::string& str)
            {
                forEachLogger([&](Logger& logger) -> void
                {
                    logger << str << std::endl;
                });
            }

            inline void operator+=(const StreamObject& msg) const
            {
                if (!msg) return;
                forEachLogger([&](Logger& logger) -> void
                {
                    logger << "user message:\n"  << msg.str() << std::endl;
                });
            }

        public:
        
            void reportInitialized() const;
            void reportGlobalBegin(const TestSummary&) const;
            void reportGlobalSummary(const TestSummary&, const std::vector<TestSuite*> = std::vector<TestSuite*>()) const;
            void reportGlobalResult(const TestSummary&, const std::string& info = "") const;
            void reportTestBegin(const TestInterface&) const;
            void reportTestResult(const TestInterface&, const std::string& info = "") const;
            //void reportTestTemplateBegin(const TestTemplate&) const;
            //void reportTestTemplateResult(const TestTemplate&) const;
            void reportTestCaseBegin(const TestSuite&) const;
            void reportTestCaseResult(const TestSuite&, const std::string& info = "") const;
            void reportException(const std::exception& e) const;
            //void reportUnknownException(const StringView& msg = "") const;

            inline void reportAssertion(const Assertion<bool>& assertion) const
            {
                forEachLogger([&](Logger& logger) -> void
                {
                    bool print_result = false;
                    switch (assertion.result())
                    {
                    case AssertionResult::SUCCESS:
                        print_result = settings.show_assertion_pass;
                        break;
                    case AssertionResult::FAIL:
                        print_result = settings.show_assertion_fail;
                        break;
                    case AssertionResult::INVALID:
                        print_result = true;
                        break;
                    default:
                        break;
                    }
                    if (print_result) printAssertionResult(logger, assertion);
                });
            }

            template <typename... Args>
            inline void reportAssertion(const Assertion<Args...>& assertion) const
            {
                forEachLogger([&](Logger& logger) -> void
                {
                    bool print_result = false;
                    bool print_args = false;
                    switch (assertion.result())
                    {
                    case AssertionResult::SUCCESS:
                        print_result = settings.show_assertion_pass;
                        print_args = settings.expand_args_assertion_pass;
                        break;
                    case AssertionResult::FAIL:
                        print_result = settings.show_assertion_fail;
                        print_args = settings.expand_args_assertion_fail;
                        break;
                    case AssertionResult::INVALID:
                        print_result = true;
                        print_args = false;
                        break;
                    default:
                        break;
                    }
                    if (print_result) printAssertionResult(logger, assertion);
                    if (print_args) 
                    { 
                        printComparison(logger, assertion.compare_result());
                    }
                });
            }

            void reportExplicitFailure() const;

        private:

            template <typename... Args>
            void printComparison(Logger& logger, const comparison::CompareHelper<Args...>& compare) const
            {
                logger.writeLine("evaluated to:");
                logger.writeLine(formatTypeAsString(compare.result));
                logger.writeLine("with arguments:");
                tuplePrinter<decltype(compare.args), std::tuple_size<decltype(compare.args)>::value>::print(logger, compare.args);
            }

            void listTestCaseResults(Logger&, const std::vector<TestSuite*>) const;

            template <typename... Args>
            void printAssertionResult(Logger& logger, const Assertion<Args...>& assertion) const
            {
                logger.write(assertion.where() + ": "); // file:line:
                switch (assertion.result())
                {
                case AssertionResult::SUCCESS:
                    logger.writeLine("assertion passed:", Logger::ANSITextColor::ANSI_GREEN); // pass/fail:
                    break;
                case AssertionResult::FAIL:
                    logger.writeLine("assertion failed:", Logger::ANSITextColor::ANSI_RED); // pass/fail:
                    break;
                default:
                    logger.writeLine("invalid assertion result:", Logger::ANSITextColor::ANSI_RED);
                    break;
                }
                logger.tab();
                logger.writeLine(assertion.text());
            }

            template <class Tuple, size_t N>
            struct tuplePrinter
            {
                static void print(Logger& logger, const Tuple& tup)
                {
                    tuplePrinter<Tuple, N - 1>::print(logger, tup);
                    logger.writeLine(formatTypeAsString(std::get<N - 1>(tup)));
                }
            };

            template <class Tuple>
            struct tuplePrinter<Tuple, 0>
            {
                static void print(Logger&, const Tuple&)
                {
                }
            };
            
            void printTypes(const Logger&) const {}

            template <typename T, typename ... Other>
            void printTypes(Logger& logger, const T& t, const Other&...rest) const
            {
                logger.writeLine(formatTypeAsString(t));
                printTypes(logger, rest...);
            }

            void printStatus(
                Logger& logger,
                const std::string& badge_text, 
                Logger::ANSITextColor = Logger::ANSITextColor::ANSI_NO_COLOR_CHOICE,
                HorizontalAlignment = HorizontalAlignment::CENTER
            ) const;

            static std::string statusBadge(std::string, size_t width, HorizontalAlignment = HorizontalAlignment::CENTER); // fmt as [ ... ] with given wdith >= strlen + 4 for bracket and spaces

            std::vector<Logger> loggers;

            const Configuration& settings;

            size_t status_width;
        };

    public:

        TestRunner(const TestRunner&) = delete;
        TestRunner(TestRunner&&) = delete;
        TestRunner& operator=(const TestRunner&) = delete;
        TestRunner& operator=(TestRunner&&) = delete;

        /**
         * Use meyer's singleton 
         * 
         * \return Singleton instance of test runner
         */
        static TestRunner& getInstance()
        {
            // lazy initialize
            static TestRunner runner;
            return runner;
        }

        /**
         * \brief Change the runtime configuration of the test runner
         * Configuration must be set before running tests. If the configuration is changed while running tests,
         * it will only apply to the current test.
         * 
         * \return Configuration& to member
         */
        Configuration& configure(const Configuration* = nullptr) noexcept;

        /**
         * \brief Access the test registry which contains a record of all available tests to run
         * 
         * \return TestRegistry& 
         */
        TestRegistry& registry();

        /**
         * \brief Access the reporter, which is the main way to send controlled output though the test runner
         * 
         * \return Reporter& 
         */
        Reporter& reporter();

        /**
         * \brief Run all the tests registered in the test registry owned by the test runner
         * 
         * \return TestSummary, a tally/summary of test results
         */
        TestSummary runAllTests();

        /**
         * \brief Run tests specified in given container of test names
         * 
         * \param test_names Names of tests to run
         * \return TestSummary 
         */
        TestSummary runTests(std::vector<StringView> test_names);

        /**
         * \brief Report an assertion to the test runner. Without calling this, the test runner would have 
         * no knowledge of an assertion result
         * 
         * \tparam Args 
         * \param assertion 
         * \return const Assertion<Args...>& 
         */
        template <typename... Args>
        const Assertion<Args...>& reportAssertion(const Assertion<Args...>& assertion)
        {
            reporter_->reportAssertion(assertion);
            test_summary.addAssertionResult(assertion);
            if (assertion.failed()) curr_test->fail();
            return assertion;
        }

        /**
         * \brief Mark the current test as failed, with no additonal reason
         * 
         */
        void explicitFailure();
        
    private:
        // private constructor for singleton
        TestRunner();
        ~TestRunner();

        TestSummary runTestCasesHelper(const std::vector<TestSuite*> tests);

        TestRegistry* registry_;
        TestInterface* curr_test;

        TestSummary test_summary;
        Configuration settings;
        Reporter* reporter_; // TODO make unique ptr

    };

}

#endif // _SSTEST_RUNNER_H_