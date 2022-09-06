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

#include "sstest/sstest_printer.h"

#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <cctype>
#include <cstdint>
#include "sstest/sstest_string.h"
#include "sstest/sstest_traits.h"


namespace sstest
{

    namespace print
    {

        // TODO configurable hexdump (width, show char etc)
        std::string hexdump(const byte* data, size_t nbyte)
        {   
            std::stringstream ss;
            ss << "(";
            for (size_t i = 0; i < nbyte; i++)
            {
                if (!(i % 8)) ss << "\n" << hexstr(&data[i]) << "  ";
                else if (!(i % 4)) ss << ' ';
                ss << hexstr(data[i]) << ' ';
            }
            ss << "\n)";
            return ss.str();
        }
    }

    void SSTestPrint(std::ostream& os, char c)
    {
        if (std::isprint(static_cast<unsigned char>(c)))
        {
            os << c;
        }
        else
        {
            switch (c)
            {
            case '\\':
                os << "\\\\";
                break;
            case '\0':
                os << "\\0";
                break;
            case '\n':
                os << "\\n";
                break;
            case '\r':
                os << "\\r";
                break;
            case '\t':
                os << "\\t";
                break;
            case '\v':
                os << "\\v";
                break;
            case '\f':
                os << "\\f";
                break;
            case '\a':
                os << "\\a";
                break;
            case '\b':
                os << "\\b";
                break;
            default:
                os << "\\x" << print::hexstr(c);
                break;
            }
        }
    }

    void SSTestPrint(std::ostream& os, const std::string& s)
    {
        os << '\"';
        for (const char& c : s)
        {
            SSTestPrint(os, c);
        } 
        os << '\"';
    }

    void SSTestPrint(std::ostream& os, const StringView& s)
    {
        os << '\"';
        for (const char& c : s)
        {
            SSTestPrint(os, c);
        } 
        os << '\"';
    }
    
    void SSTestPrint(std::ostream& os, const void* ptr)
    {
        os << "0x" << print::hexstr(ptr) << '\"';
    }
    
    void SSTestPrint(std::ostream& os, const char* cs)
    {
        SSTestPrint(os, StringView(cs));
    }
    
    void SSTestPrint(std::ostream& os, bool b)
    {
        os << (b ? "true" : "false");
    }
}