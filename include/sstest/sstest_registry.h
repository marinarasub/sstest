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

#ifndef _SSTEST_REGISTRY_H_
#define _SSTEST_REGISTRY_H_

#include <unordered_map>
#include <vector>
#include "sstest_test.h"
#include "sstest_string.h"
#include "sstest_config.h"

/**
 * \file sstest_info.h
 * \brief Class definition for test registry, which keeps track of all test cases automatically discovered by SSTest
 * 
 */

namespace sstest
{

    /**
     * \brief Stores and organizes a collection of test objects created by a user program
     * 
     */
    class TestRegistry
    {
    public:
        
        TestRegistry();

        ~TestRegistry();

        void clear() noexcept;

        TestSuite* getDefaultTestCase() noexcept;

        TestSuite* getTestCase(const StringView& name);

        std::vector<TestSuite*> getTestCases(bool include_empty = false, sstest_case_comparator cmp = nullptr) noexcept;

    private:
        // map of name of test suite to test functions in each suite
        std::unordered_map<const StringView, TestSuite*> test_map;
    };


}

#endif // _SSTEST_REGISTRY_H_