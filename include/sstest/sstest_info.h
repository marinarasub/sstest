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

#ifndef _SSTEST_INFO_H_
#define _SSTEST_INFO_H_

#include <string>
#include "sstest_string.h"
#include "sstest_config.h"

/**
 * \file sstest_info.h
 * \brief Class definitions for test case and assertion information
 * 
 */

namespace sstest
{

    struct LineInfo
    {
        constexpr LineInfo(const char* file, size_t line) noexcept
            : file_name(file), line_no(line) {}

        constexpr LineInfo(const StringView& file, size_t line) noexcept
            : file_name(file), line_no(line) {}
        
        std::string where() const noexcept;

        const StringView file_name;
        const size_t line_no; 
    };

    struct TestInfo
    {
        constexpr TestInfo(const char* name) noexcept
            : name(name) {}

        constexpr TestInfo(const StringView& name) noexcept
            : name(name) {}

        const StringView name;
    };

}

#endif // _SSTEST_INFO_H_