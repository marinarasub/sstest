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

#include <cassert>
#include <algorithm>

#include "sstest/sstest_registry.h"

namespace sstest
{

    TestRegistry::TestRegistry()
    {
        test_map[""] = new TestSuite(TestInfo(""));
        assert(test_map.find("") != test_map.end());
    }

    TestRegistry::~TestRegistry()
    {
        clear();
    }

    void TestRegistry::clear() noexcept
    {
        for (auto &t : test_map)
        {
            assert(t.second != nullptr);
            delete t.second;
            t.second = nullptr;
        }
        test_map.clear();
    }

    TestSuite* TestRegistry::getDefaultTestCase() noexcept
    {
        assert(test_map.find("") != test_map.end());
        TestSuite* default_group = test_map[""];
        return default_group;
    }

    TestSuite* TestRegistry::getTestCase(const StringView& name)
    {
        if (name.empty()) throw InvalidArgument("Test Group \"\" is reserved for default");

        TestSuite* test_case = nullptr;

        auto it = test_map.find(name);
        if (it == test_map.end())
        {
            // create a new test case if not exists
            test_case = new TestSuite(TestInfo(name));
            test_map[name] = test_case;
        }
        else
        {
            assert(it->second != nullptr);
            test_case = it->second;
        }
        assert(test_map.find(name) != test_map.end());
        assert(test_case != nullptr);
        return test_case;
    }

    std::vector<TestSuite*> TestRegistry::getTestCases(bool include_empty, sstest_case_comparator cmp) noexcept
    {
        std::vector<TestSuite*> tests;
        tests.reserve(test_map.size());
        for (auto kv : test_map)
        {
            assert(kv.second != nullptr);
            if (include_empty || !kv.second->empty()) tests.push_back(kv.second);
        }
        if (cmp) 
        { 
            std::sort(tests.begin(), tests.end(), cmp);
        }
        return tests;
    }


}