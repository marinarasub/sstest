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

#ifndef _SSTEST_TEST_H_
#define _SSTEST_TEST_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <memory>
#include "sstest_exception.h"
#include "sstest_info.h"
#include "sstest_string.h"

/**
 * \file sstest_test.h
 * \brief Contains classes for storing all information needed to track and run test functions
 * defined by the user.
 * 
 */

namespace testing
{

#ifndef SSTEST_SETUP_FUNCTION_NAME
/**
 * \brief Allow user to compile with custom name for test fixture setup function.
 * 
 */
#   define SSTEST_SETUP_FUNCTION_NAME SetUp
#endif

#ifndef SSTEST_TEARDOWN_FUNCTION_NAME
/**
 * \brief Allow user to compile with custom name for test fixture tear down function
 * 
 */
#   define SSTEST_TEARDOWN_FUNCTION_NAME TearDown
#endif

    /**
     * \brief Built-in text fixture class with virtual functions to overrload which will be used by sstest
     * 
     * \tparam Args Variadic types of user test function
     */
    template <typename... Args>
    struct Test
    {
    public:
        virtual ~Test() {}

        virtual void SSTEST_SETUP_FUNCTION_NAME() {}
        virtual void SSTEST_TEARDOWN_FUNCTION_NAME() {}
        virtual void operator()(Args...) = 0;
        
    protected:
        Test() {}
    };


}


namespace sstest
{
    class TestInterface;
    class TestSuite;

    typedef std::function<void()> sstest_void_function;
    typedef std::function<void(TestInterface&)> sstest_callback; //typedef void(*sstest_test_callback)();      
    typedef std::function<bool(const TestInterface*, const TestInterface*)> sstest_comparator;
    typedef std::function<bool(const TestSuite*, const TestSuite*)> sstest_case_comparator;

    enum class TestResult 
    {
        INVALID = -1,
        FAIL = 0,
        SUCCESS = 1,
        THROW = 3,
        PASS = SUCCESS,
    };
    
    /**
     * \brief Abstract base class for creating test objects that will be used by test runner to run user defined test functions
     * 
     */
    class TestInterface
    {

    public:

        /**
         * \brief Public helper function to aid in converting arbitrary callable to void function with no parameters, which can be used by sstest test classes
         * 
         * \tparam Callable 
         * \tparam Args 
         * \tparam std::enable_if<!std::is_base_of<::testing::Test<Args...>, Callable>::value>::type 
         * \param func 
         * \param args 
         * \return sstest_void_function 
         */
        template <typename Callable, typename... Args, typename = typename std::enable_if<!std::is_base_of<::testing::Test<Args...>, Callable>::value>::type>
        static sstest_void_function createTestInvoker(Callable func, Args&&... args)
        {
             // TODO Callable cannot be const& since overload with other createTestInvoker won't work. Instead create similar fashio to find first of in sstest_printer.
            return [=]() mutable -> void
            {
                func(args...);
            };
        }

        /**
         * \brief Public helper function to convert derived type of ::testing::Test<...> to a void function for use with sstest test classes
         * 
         * \tparam Callable 
         * \tparam Args 
         * \tparam std::enable_if<!std::is_base_of<::testing::Test<Args...>, Callable>::value>::type 
         * \param func 
         * \param args 
         * \return sstest_void_function 
         */
        template <typename TestType, typename... Args, typename = typename std::enable_if<std::is_base_of<::testing::Test<Args...>, TestType>::value>::type>
        static sstest_void_function createTestInvoker(const TestType& test_param, Args&&... args)
        {
            return sstest_void_function([=]() mutable -> void
            {
                TestType test_obj = test_param;

                test_obj.SSTEST_SETUP_FUNCTION_NAME();
                try
                {
                    test_obj(args...);
                }
                catch (...)
                {
                    test_obj.SSTEST_TEARDOWN_FUNCTION_NAME();
                    throw;
                }
                test_obj.SSTEST_TEARDOWN_FUNCTION_NAME();
            });
        }


    public:

        /**
         * \brief Construct a new Test Interface object from test information and a void function to call when running test
         * 
         * \param tinfo 
         * \param linfo 
         */
        TestInterface(TestInfo tinfo, LineInfo linfo, sstest_void_function) noexcept;

        virtual ~TestInterface();

        /**
         * \brief Return the string identifier of the test object
         * 
         * \return StringView 
         */
        StringView name() const noexcept;

        /**
         * \brief Runs the test function given on construction, tracking its result
         * 
         */
        virtual void run() = 0;

        /**
         * \sa run()
         */
        void operator()();

        /**
         * \brief Interface function for creating copies of test object that concerete derived classes must implement
         * 
         * \return TestInterface* 
         */
        virtual TestInterface* clone() const = 0;

        /**
         * \brief Check if the test held by the object has been run
         * 
         * \return true If the test has ran
         * \return false Else
         */
        bool ran() const noexcept;

        /**
         * \brief Check if the test passed when ran
         * 
         * \return true If the test passed
         * \return false Else, including if the test has not been run yet
         */
        bool passed() const noexcept;

        /**
         * \brief Return the test result as a TestResult enum type for a more detailed description of the test result than passed()
         * 
         * \return TestResult 
         */
        TestResult result() const noexcept;

        /**
         * \brief Fail the test, regardless of current test status
         * 
         */
        void fail(bool = true);
       
    protected:
        /**
         * \brief Helper function to run a test object and capture its result
         * 
         * \return TestInterface& 
         */
        static TestInterface& runTestHelper(TestInterface&);

        TestInfo test_info;
        LineInfo line_info;
        sstest_void_function invoker; // TODO move this to concrete impl.?
        TestResult result_;
    };

    /**
     * \brief Concrete implementaiton of basic test function for user defined tests
     * 
     */
    class TestFunction : public TestInterface
    {

    public:
        
        TestFunction(TestInfo tinfo, LineInfo linfo, sstest_void_function test_func); // TOD reverse args

        /**
         * \sa TestInterface::run()
         */
        virtual void run() override;

        /**
         * \sa TestInterface::clone()
         */
        virtual TestFunction* clone() const override;
        
    private:

        int weight; // !!! TODO currently unused

    };

    /**
     * \brief Manages a suite, or collection of tests of a similar fashion
     * 
     */
    class TestSuite
    {
    public:
        /**
         * \brief Construct a new empty test suite with test suite information
         * Every test object is designed to be part of a test suite, to be logically and functionally grouped with similar tests
         * 
         * \param info 
         */
        TestSuite(TestInfo info);

        virtual ~TestSuite();

        /**
         * \brief Run all tests in the suite
         * 
         * \param start_cb A function to call before running each individual test case, which takes a const TestInteface& as parameter
         * \param finish_cb  A function to call after finishing each individual test case, which takes a const TestInterface& as parameter
         * \param cmp A comparator function which takes compares const TestInterface&. Should return signed integral type < 0 if a test should run first between two test objects
         */
        virtual void run(sstest_callback start_cb = nullptr, sstest_callback finish_cb = nullptr, sstest_comparator cmp = nullptr);

        /**
         * \brief Clear, or empty, the test suite
         * 
         */
        void clear() noexcept;

        /**
         * \brief Check if the test suite is empty, contains 0 test cases
         * 
         * \return true 
         * \return false 
         */
        bool empty() const noexcept;

        /**
         * \brief Return the number of test objects stored in the test 
         * 
         * \return size_t 
         */
        size_t size() const noexcept;

        /**
         * \sa size()
         */
        inline size_t numTests() const noexcept { return size(); }

        /**
         * \brief Return the number of tests that passed in the test suite
         * Can still be called if not all tests were ran in tests suite
         * 
         * \return size_t 
         */
        size_t numTestsPassed() const noexcept;

        /**
         * \brief Return the number of tests in the suite that has finished running
         * Tests that have attempted to run, but failed, or threw a caught exception etc. are included in this count
         * 
         * \return size_t 
         */
        inline size_t numTestsRan() const noexcept { return num_ran; }

        /**
         * \brief Return the string identifier of the test suite
         * 
         * \return StringView 
         */
        StringView name() const noexcept;

        /**
         * \brief Check if the test suite has finished running
         * That is, all tests have ran or have attempted to run
         * \sa TestInterface::ran()
         * 
         * \return true 
         * \return false 
         */
        bool ran() const noexcept;

        /**
         * \brief Check if all tests in the suite have passed
         * \sa TestInterface::passed()
         * 
         * \return true 
         * \return false 
         */
        bool passed() const noexcept;

        /**
         * \brief Add a test object to the test suite
         * \note Will clone the test object given, does not own or use the original test object given as a parameter
         * \throw InvalidArgument if a test by the same identifier (name) already exists in the suite
         * 
         * \param test 
         */
        void addTest(const TestInterface& test);

        // void removeTest(); !!! TODO
        
        /**
         * \brief Get a test object by its string identifier
         * The identifier is guarenteed to map to a unique test within the test suite
         * \sa addTest()
         * 
         * \param name 
         * \return TestInterface& 
         */
        TestInterface& getTest(const StringView& name) const;

        /**
         * \brief Return a container of pointers to each test in the test case
         *
         * \param cmp A comparator function which takes compares const TestInterface&. Should return signed integral type < 0 if a test should run first between two test objects
         * \return std::vector<TestInterface*> 
         */
        std::vector<TestInterface*> getTests(sstest_comparator cmp = nullptr) const;

    protected:
        /**
         * \brief Helper function for running a single test object within the test suite
         * Will perform all tasks needed to be done per test object
         * 
         * \param start_cb 
         * \param finish_cb 
         * \return TestInterface& 
         */
        static TestInterface& runSingleTestHelper(TestInterface&, sstest_callback start_cb = nullptr, sstest_callback finish_cb = nullptr);

    private:

        std::unordered_map<const StringView, TestInterface*> test_map;
        TestInfo test_info;
        bool pass;
        bool finished;
        size_t num_ran;

    };

}

#endif // _SSTEST_TEST_H_
