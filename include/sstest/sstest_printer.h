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

#ifndef _SSTEST_PRINTER_H_
#define _SSTEST_PRINTER_H_

#include <iostream>
#include <sstream>
#include <string>
#include "sstest_traits.h"
#include "sstest_config.h"

/**
 * \file sstest_printer.h
 * \brief Functions for pretty printing different types
 * 
 */

namespace sstest
{
    class StringView;

    template <typename T> void SSTestPrint(std::ostream&, const T&);
    void SSTestPrint(std::ostream&, char);
    void SSTestPrint(std::ostream&, const std::string&);
    void SSTestPrint(std::ostream&, const StringView&);
    void SSTestPrint(std::ostream&, const char*);
    void SSTestPrint(std::ostream&, const void*);
    void SSTestPrint(std::ostream&, bool);

    namespace print
    {
    
    #ifndef SSTEST_TO_STRING_FUNCTION_NAME
    /**
     * \def SSTEST_TO_STRING_FUNCITON_NAME
     * \brief Allow user to compile library with a custom name for their to string function
     * By default, this is to_string()
     */
    #   define SSTEST_TO_STRING_FUNCTION_NAME to_string
    #endif

        typedef unsigned char byte;
        // since uintptr_t is optional i C++11, use max uint instead
        typedef uintmax_t uintptr_type;
        static_assert(sizeof(uintptr_type) >= sizeof(void*), "uintmax_t is stoo small to represent represent void*");

        template <typename I, typename = typename std::enable_if<std::is_integral<I>::value>::type> 
        inline std::string hexstr_lower(I val, size_t width = sizeof(I) * 2) 
        {
            static const char digits[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', };
            std::string str(width, '0');
            for (size_t i = 0, j = (width - 1) * 4 ; i < width; ++i, j -= 4)
            {
                str[i] = digits[(val >> j) & 0x0f];
            }
            return str;
        }

        inline std::string hexstr_lower(const void* ptr, size_t width = sizeof(void*) * 2) 
        {
            uintptr_type val = reinterpret_cast<uintptr_type>(ptr);
            static const char digits[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f', };
            std::string str(width, '0');
            for (size_t i = 0, j = (width - 1) * 4 ; i < width; ++i, j -= 4)
            {
                str[i] = digits[(val >> j) & 0x0f];
            }
            return str;
        }

        template <typename T> 
        inline std::string hexstr(const T& val) // TODO leading 0, lower/upper, 0x 
        {
            return hexstr_lower(val);
        }

        /**
         * \brief Return a string which is a hexidecimal representaiton of a byte array
         * 
         * \param data Pointer to byte array, cannot be null
         * \param nbyte Size fo byte array to show
         * \return std::string of byte array representation
         */
        std::string hexdump(const byte* data, size_t nbyte);

        namespace streamprint_without_lexical_lookup
        {
            // Taken from google test
            // The presence of an operator<< here will terminate lexical scope lookup straight away
            struct LookupBlocker {};
            void operator<<(LookupBlocker, LookupBlocker);
            
            struct streamPrinter
            {
                template <typename T, typename = decltype( std::declval<std::ostream&>() << (std::declval<const T&>()) )>
                static void print(std::ostream& os, const T& val)
                {
                    os << val;
                }
            };
        }
        
        struct bytePrinter
        {
            template <typename T>
            static void print(std::ostream& os, const T& val)
            {
                size_t size = sizeof(T) / sizeof(byte);
                const byte* bytes = reinterpret_cast<const byte*>(&val); 
                os << hexdump(bytes, size);
            }
        };
        
        struct toStringPrinter
        {
            template <typename T, typename = decltype(SSTEST_TO_STRING_FUNCTION_NAME(std::declval<T&>()))>
            static void print(std::ostream& os, const T& val)
            {
                os << SSTEST_TO_STRING_FUNCTION_NAME(val);
            }
        };
        
        struct pointerPrinter
        {
            static void print(std::ostream& os, std::nullptr_t)
            {
                os << "nullptr";
            }  
            
            template <typename Ret, typename ... Args>
            static void print(std::ostream& os, Ret(*fptr)(Args...))
            {
                if (fptr == nullptr)
                {
                    os << "nullptr (function)";
                }
                else  os << "0x" << hexstr(reinterpret_cast<const void*>(fptr)) << " (function)";//reinterpret_cast<const void*>(fptr);
            }  

            template <typename P, typename = typename std::enable_if<std::is_pointer<P>::value>::type>
            static void print(std::ostream& os, const P& ptr)
            {
                if (ptr == nullptr)
                {
                    os << "nullptr";
                }
                else  os << "0x" << hexstr(ptr);// static_cast<const void*>(ptr);
            }  
        };
        
        struct integralPrinter
        {
            template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type>
            static void print(std::ostream& os, T val)
            {
                os << std::to_string(val);
            }
        };


        struct floatingPointPrinter
        {
            template <typename T, typename = typename std::enable_if<std::is_floating_point<T>::value, T>::type>
            static void print(std::ostream& os, T val)
            {
                os << float_to_string(val);
            }
        };
        
        struct arrayPrinter
        {
            template <typename A, size_t N, typename = typename std::enable_if<std::is_array<A[N]>::value>::type>
            static void print(std::ostream& os, const A (&arr)[N])
            {
                size_t arr_size = sizeof(arr) / sizeof(arr[0]);
                os << "{ ";
                for (size_t i = 0; i < arr_size; i++)
                {
                    SSTestPrint(os, arr[i]);
                    os << ", ";
                }
                os << "}";
            }  
        };
        
        struct containerPrinter
        {
            template <typename C, typename = typename std::enable_if<is_container<C>::value>::type>
            static void print(std::ostream& os, const C& cont)
            {
                os << "{ ";
                for (const auto &i : cont)
                {
                    SSTestPrint(os, i);
                    os << ", ";
                }
                os << "}";
            }
        };
        
        // impl. from Google Test
        // using SFINAE, find first printer for which substituion with Printer::print works (like if_enable)
        // iterate through printers until substituion works below this one
        template <typename T, typename E, typename Printer, typename... Printers>
        struct findPrinter : findPrinter<T, E, Printers...> {};
        
        template <typename T, typename Printer, typename... Printers>
        struct findPrinter <T, decltype(Printer::print(std::declval<std::ostream&>(), std::declval<const T&>())), Printer, Printers...>
        {
            using type = Printer;
        };

    }
    
    template <typename T>
    void SSTestPrint(std::ostream& os, const T& val) 
    {
        using namespace print;

        using Printer = typename findPrinter<T, void, 
            containerPrinter, 
            arrayPrinter, 
            pointerPrinter, 
            floatingPointPrinter,
            integralPrinter,
            toStringPrinter, 
            streamprint_without_lexical_lookup::streamPrinter, 
            bytePrinter>::type;
        Printer::print(os, val);
    }
    
    /**
     * \brief Print any type as a string, as useful represention as possible
     * 
     * \tparam T Type to print
     * \param val value to p rint, of type T
     * \return Formatted string
     */
    template <typename T>
    std::string formatTypeAsString(const T& val)
    {
        std::stringstream ss;
        SSTestPrint(ss, val);
        return ss.str();
    }
    

}

#endif // _SSTEST_PRINTER_H_