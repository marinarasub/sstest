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

#include "sstest/sstest_console.h"

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include "sstest/sstest_exception.h"
#include "sstest/sstest_string.h"


#if defined(_WIN32) || defined(_WIN64)

#   ifdef _MSC_VER
        __pragma( warning(disable : 5105) ) // C5105: macro expansion producing 'defined' has undefined behavior: enabled with /Zc:preprocessor, in windows.h
#   endif // _MSC_VER

#   define WIN32_LEAN_AND_MEAN
#   define NOMINMAX
#   include <windows.h>

#   ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
#       define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
#   endif // ENABLE_VIRTUAL_TERMINAL_PROCESSING

#   define SSTEST_ENABLE_ANSI() \
        do { \
            HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE); \
            DWORD dw; \
            GetConsoleMode(hout, &dw); \
            SetConsoleMode(hout, dw | ENABLE_VIRTUAL_TERMINAL_PROCESSING); \
        } while(0)

#else // defined(_WIN32) || defined(_WIN64)

// else these do nothing
#   define SSTEST_ENABLE_ANSI()

#endif // defined(_WIN32) || defined(_WIN64)

namespace sstest
{
    //https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797

    //static constexpr const StringView ANSI_STR_CLEAR_ALL = "\x1B[0J";
    //static constexpr const StringView ANSI_STR_CLEAR_LINE = "\x1B[2K";
    static constexpr const StringView ANSI_STR_YELLOW = "\x1B[33m";
    static constexpr const StringView ANSI_STR_GREEN = "\x1B[32m";
    static constexpr const StringView ANSI_STR_RED = "\x1B[31m";
    static constexpr const StringView ANSI_STR_BLUE = "\x1B[34m";
    static constexpr const StringView ANSI_STR_BOLD = "\x1B[1m";
    static constexpr const StringView ANSI_STR_ITALICS = "\x1B[3m";
    static constexpr const StringView ANSI_STR_UNDERLINE = "\x1B[4m";
    static constexpr const StringView ANSI_STR_BLINK = "\x1B[5m";
    static constexpr const StringView ANSI_STR_DEFAULT_COLOR = "\x1B[39m";
    static constexpr const StringView ANSI_STR_RESET = "\x1B[0m";
    //static constexpr const StringView ANSI_STR_CURSOR_RESET = "\x1B[0G";

    Logger::Logger()
        : Logger(std::cout) {}

    Logger::Logger(std::ostream& out, bool ansi_enable, size_t tab_spaces)
        : out(out), ansi_enable(ansi_enable), tab_width(tab_spaces)
    {
        if (ansi_enable) { enable_ansi(); }
    }

    Logger::~Logger()
    {
        flush();
    }

    void Logger::flush()
    {
        out.flush();
    }

    void Logger::tab(size_t ntab) const
    {
        out << std::string(ntab * tab_width, ' ');
    }

    void Logger::time() const
    {
        std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
#   ifdef _MSC_VER
#       pragma warning(push)
#       pragma warning(disable : 4996) // CRT safety warning for ctime: for now we use ctime to get time
#   endif
        out << std::ctime(&time);
#   ifdef _MSC_VER
#       pragma warning(pop)
#   endif
    }

    Logger& Logger::operator<<(std::ostream&(*iomanip)(std::ostream &)) 
    {
        iomanip(out);
        return *this;
    }

    Logger& Logger::operator<<(char c)
    {
        out << c;
        return *this;
    }

    Logger& Logger::operator<<(const char* msg)
    {
        out << msg;
        return *this;
    }

    Logger& Logger::operator<<(const std::string& msg)
    {
        out << msg;
        return *this;
    }

    Logger& Logger::operator<<(ANSITextColor clr)
    {
        out << ANSITextColorString(clr);
        return *this;
    }

    Logger& Logger::operator<<(ANSITextFormat fmt)
    {
        out << ANSITextFormatString(fmt);
        return *this;
    }

    void Logger::write(const std::string& msg, ANSITextColor clr, ANSITextFormat fmt)
    {
        *this << clr << fmt << msg << ANSIReset();
    }

    // 4 space/ indentation level (tab hard to control width)
    void Logger::writeLine(const std::string& msg, ANSITextColor clr, ANSITextFormat fmt)
    {
        write(msg + '\n', clr, fmt);
    }


    void Logger::enable_ansi(bool enable)
    {
        static bool initialized = false;
        if (!initialized && enable)
        {
            SSTEST_ENABLE_ANSI();
        }
    }
    
    std::string Logger::ANSITextColorString(ANSITextColor clr) const noexcept
    {
        if (!ansi_enable) return "";

        switch (clr)
        {
        case Logger::ANSITextColor::ANSI_RED:
            return ANSI_STR_RED;
        case Logger::ANSITextColor::ANSI_YELLOW:
            return ANSI_STR_YELLOW;
        case Logger::ANSITextColor::ANSI_GREEN:
            return ANSI_STR_GREEN;
        case Logger::ANSITextColor::ANSI_BLUE:
            return ANSI_STR_BLUE;
        case Logger::ANSITextColor::ANSI_DEFAULT_COLOR:
            return ANSI_STR_DEFAULT_COLOR;
        default: // default do nothing
            return "";
        }
    }

    std::string Logger::ANSITextFormatString(ANSITextFormat flags) const noexcept
    {
        if (!ansi_enable || (flags & Logger::ANSITextFormat::ANSI_NO_FORMAT)) return "";
        if (flags == ANSI_RESET) return ANSIReset();

        // build format string
        std::string fmt = "";
        if (flags & Logger::ANSITextFormat::ANSI_BOLD) fmt += std::string(ANSI_STR_BOLD);
        if (flags & Logger::ANSITextFormat::ANSI_ITALICS) fmt+= std::string(ANSI_STR_ITALICS);
        if (flags & Logger::ANSITextFormat::ANSI_UNDERLINE) fmt += std::string(ANSI_STR_UNDERLINE);
        if (flags & Logger::ANSITextFormat::ANSI_BLINK) fmt += std::string(ANSI_STR_BLINK);
        return fmt;
    }

    std::string Logger::ANSIReset() const noexcept
    {
        if (!ansi_enable) return "";

        return ANSI_STR_RESET;
    }

}