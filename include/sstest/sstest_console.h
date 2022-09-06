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

#ifndef _SSTEST_CONSOLE_H_
#define _SSTEST_CONSOLE_H_

#include <string>
#include "sstest_string.h"
#include "sstest_config.h"


/**
 * \file sstest_console.h
 * \brief Contains logger and console output helper classes and definitions
 * 
 */

namespace sstest
{

    enum class HorizontalAlignment
    {
        LEFT,
        RIGHT,
        CENTER,
        CENTRE = CENTER
    };

    /**
     * \brief Logger class writes text to an output stream
     * Additionally, provides functionallity for formatting when printing
     * 
     */
    class Logger
    {

    public:
        static const size_t DEFAULT_TAB_WIDTH = 4;

    public:

        enum ANSITextColor
        {
            ANSI_DEFAULT_COLOR = 0,
            ANSI_RED = 1,
            ANSI_GREEN = 2,
            ANSI_YELLOW = 3,
            ANSI_BLUE = 4,
            ANSI_NO_COLOR_CHOICE
        };

        // flags can be | (bitwise or'd)
        // reset auto inserted after printline or print
        enum ANSITextFormat
        {
            ANSI_RESET = 0,
            ANSI_NO_FORMAT = 0x1,
            ANSI_BOLD = 0x2,
            ANSI_ITALICS = 0x4,
            ANSI_UNDERLINE = 0x8,
            ANSI_BLINK = 0x10,
            ANSI_TRIPLE_EMPHASIS = ANSI_BOLD | ANSI_ITALICS | ANSI_UNDERLINE,
        };

        /**
         * \brief Create default logger, which uses std::cout
         * 
         */
        Logger();

        explicit Logger(std::ostream& out, bool ansi_enable = false, size_t tab_spaces = DEFAULT_TAB_WIDTH);

        virtual ~Logger();

        /**
         * \brief Flush the internal stream buffer
         * 
         */
        void flush();

        // 4 space/ indentation level (tab hard to control width)
        void tab(size_t ntab = 1) const;
        void time() const;

        Logger& operator<<(std::ostream&(*)(std::ostream &));

        template <typename T, typename = decltype( std::declval<std::ostream&>() << (std::declval<const T&>()) )>
        Logger& operator<<(const T& val)
        {
            out << val;
            return *this;
        }
        Logger& operator<<(char);
        Logger& operator<<(const char*);
        Logger& operator<<(const std::string&);
        Logger& operator<<(ANSITextColor);
        Logger& operator<<(ANSITextFormat);

        void write(
            const std::string&, 
            ANSITextColor = ANSI_NO_COLOR_CHOICE,
            ANSITextFormat = ANSI_NO_FORMAT
        );

        void writeLine(
            const std::string& = "",
            ANSITextColor = ANSI_NO_COLOR_CHOICE,
            ANSITextFormat = ANSI_NO_FORMAT
        );

    protected:
        static void enable_ansi(bool enable = true);

    private:
        std::string ANSITextColorString(ANSITextColor) const noexcept;
        std::string ANSITextFormatString(ANSITextFormat) const noexcept;
        std::string ANSIReset() const noexcept;

        std::ostream& out;

        bool ansi_enable;
        //size_t console_width; // !!! TODO in chars, default 80. will append \n after width and draw lines this wide
        size_t tab_width;
    };

}

#endif // _SSTEST_CONSOLE_H_