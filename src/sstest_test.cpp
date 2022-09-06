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

#include "sstest/sstest_test.h"

#include <cassert>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include <algorithm>

#include "sstest/sstest_exception.h"
#include "sstest/sstest_string.h"

namespace sstest
{

    ////////// TEST INTERFACE /////////////////

    TestInterface::TestInterface(TestInfo tinfo, LineInfo linfo, sstest_void_function test_callback) noexcept
        : test_info(tinfo), line_info(linfo), invoker(test_callback), result_(TestResult::INVALID)
    {

    }

    TestInterface::~TestInterface() {}

    StringView TestInterface::name() const noexcept
    {
        return test_info.name;
    }

    // StringView TestInterface::identifier() const noexcept
    // {
    //     return test_info.name;//std::string(test_info.name.data());
    // }

    void TestInterface::operator()()
    {
        run();
    }

    bool TestInterface::ran() const noexcept
    {
        return result_ != TestResult::INVALID;
    }

    bool TestInterface::passed() const noexcept
    {
        // TODO const message
        //if (!ran()) throw SSException("Call to passed() although test hasn't ran");
        return result_ == TestResult::PASS;
    }

    TestResult TestInterface::result() const noexcept
    {
        return result_;
    }

    void TestInterface::fail(bool fail)
    {
        result_ = (fail) ? TestResult::FAIL : result_;
    }

    TestInterface& TestInterface::runTestHelper(TestInterface& test)
    {
        try
        {
            test.invoker();
        }
        catch (const std::exception& e) {
            (void)e;
            test.result_ = TestResult::THROW;
        }
        catch (...)
        {
            test.result_ = TestResult::THROW;
        }
        return test;
    }



    /////////////// TEST FUNCTION ///////////////////////////////

    TestFunction::TestFunction(TestInfo tinfo, LineInfo linfo, sstest_void_function test_func)
        : TestInterface(tinfo, linfo, test_func), weight(1)
    {
        if (!invoker) throw InvalidArgument("Test function was null");
    }

    void TestFunction::run()
    {
        (void)weight;// TODO for -Wunusred-private-field
        result_ = TestResult::PASS;
        runTestHelper(*this); // TODO catch
        //finished = true;
    }

    TestFunction* TestFunction::clone() const
    {
        return new TestFunction(*this);
    }


    //////////////// TEST TEMPLATE ///////////////////////

    // void TestTemplate::run()
    // {
    //     (void)weight;// TODO for -Wunusred-private-field
    //     pass = true;
    //     runTestHelper(*this);
    //     finished = true;
    // }

    // TestTemplate* TestTemplate::clone() const
    // {
    //     return new TestTemplate(*this);
    // }


    ////////////////// TEST CASE /////////////////////

    TestSuite::TestSuite(TestInfo tinfo)
        : test_info(tinfo), pass(false), finished(false), num_ran(0)
        //: TestInterface(info)
    {

    }

    TestSuite::~TestSuite() 
    {
        clear();
    }

    void TestSuite::run(sstest_callback start_cb, sstest_callback finish_cb, sstest_comparator cmp)
    {
        // get sorted
        num_ran = 0;
        std::vector<TestInterface*> tests = getTests(cmp);
        pass = true;
        for (TestInterface* child : tests)
        {
            // should never be null
            assert(child != nullptr);
            // TODO if pass && run... short circuits on fial. leave as option.
            bool curr_pass = runSingleTestHelper(*child, start_cb, finish_cb).passed();
            pass = pass && curr_pass;
            num_ran++;
        }
        finished = true;
    }

    // TestSuite* TestSuite::clone() const
    // {
    //     return new TestSuite(*this);
    // }

    void TestSuite::clear() noexcept
    {
        for (auto &kv : test_map)
        {
            assert(kv.second != nullptr);
            delete kv.second;
            kv.second = nullptr;
        }
        test_map.clear();
        num_ran = 0;
        pass = false;
        finished = false;
    }

    bool TestSuite::empty() const noexcept
    {
        return test_map.empty();
    }

    size_t TestSuite::size() const noexcept
    {
        return test_map.size();
    }

    size_t TestSuite::numTestsPassed() const noexcept
    {
        size_t count = 0;
        for (auto &kv : test_map)
        {
            assert(kv.second != nullptr);
            count += kv.second->passed() ? 1 : 0;
        }
        return count;
    }

    StringView TestSuite::name() const noexcept
    {
        return test_info.name;
    }

    bool TestSuite::ran() const noexcept
    {
        return finished;
    }

    bool TestSuite::passed() const noexcept
    {
        return pass;
    }

    void TestSuite::addTest(const TestInterface& test)
    {
        auto it = test_map.find(test.name());
        if (it == test_map.end())
        {
            // copy new
            test_map[test.name()] = test.clone();//new TestInterface(test);
        }
        else
        {
            throw InvalidArgument(std::string(test.name()) + " already exists");
        }
    }

    TestInterface& TestSuite::getTest(const StringView& name) const
    {
        auto it = test_map.find(name);
        if (it != test_map.end())
        {
            assert(it->second != nullptr);
            return *it->second;
        }
        else
        {
            throw InvalidArgument(std::string(name) + " is not defined");
        }
    }

    std::vector<TestInterface*> TestSuite::getTests(sstest_comparator cmp) const
    {

        std::vector<TestInterface*> tests;
        tests.reserve(test_map.size());
        for (auto kv : test_map)
        {
            tests.push_back(kv.second);
        }
        if (cmp) 
        {
            std::sort(tests.begin(), tests.end(), cmp);
        }
        
        return tests;
    }

    TestInterface& TestSuite::runSingleTestHelper(TestInterface& test, sstest_callback start_cb, sstest_callback finish_cb)
    {
        if (start_cb) { start_cb(test); }
        test.run();
        if (finish_cb) { finish_cb(test); }
        return test;
    }

}
