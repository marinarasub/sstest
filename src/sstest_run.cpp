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

#include "sstest/sstest_run.h"

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <utility>
#include <stdexcept>
#include <iostream>
#include "sstest/sstest_string.h"
#include "sstest/sstest_runner.h"


namespace testing
{
    
    int ExitCode(const ::sstest::TestTotals& totals)
    {
        return totals.allTestsPassed() ? SSTEST_SUCCESS : SSTEST_FAILURE;
    }

    void Configure(int argc, char** argv)
    {
        using namespace ::sstest;

        // TODO parse args to TestRunner::Configuration
        // For now, it does nothing.
        (void)argc;
        (void)argv;
    }

    int RunTests(int argc, char** argv)
    {
        using namespace ::sstest;
        
        Configure(argc, argv);

        return ExitCode(TestRunner::getInstance().runAllTests().getTotals());
    }

    int RunAllTests()
    {
        using namespace ::sstest;

        return ExitCode(TestRunner::getInstance().runAllTests().getTotals());
    }
}