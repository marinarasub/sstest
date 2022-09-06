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

#ifndef _SSTEST_STRING_H_
#define _SSTEST_STRING_H_

#include <cstdint>
#include <cstring>
#include <iterator>
#include <limits>
#include <string>
#include <ostream>
#include <algorithm>
#include "sstest_utility.h"
#include "sstest_config.h"


/**
 * \file sstest_string.h
 * \brief Definition for custom string view implementation for C++11 and related functions and classes
 * 
 */

namespace hash_functions
{

    /**
     * \brief MurmurHash2, 64-bit versions, by Austin Appleby
     * The same caveats as 32-bit MurmurHash2 apply here - beware of alignment 
     * and endian-ness issues if used across multiple platforms.
     * https://sites.google.com/site/murmurhash/
     */
    namespace murmur
    {

        // endian netural   
        uint32_t murmurHashNeutral32 ( const void * key, int len, unsigned int seed );

        // 32 bit
        uint32_t murmurHash32 ( const void * key, int len, unsigned int seed );

        // 64-bit hash for 64-bit platforms
        uint64_t murmurHash64A ( const void * key, int len, unsigned int seed );

        // 64-bit hash for 32-bit platforms
        uint64_t murmurHash64B ( const void * key, int len, unsigned int seed );
    }

}

namespace sstest 
{

    /**
     * \brief A constexpr version fo C library strlen()
     * 
     * \param str Pointer to cstring (must be null terminated) 
     * \return length of str, not including null terminator 
     */
    constexpr size_t cstrlen(const char* str) noexcept
    {
        return (*str == '\0') ? 0 : 1 + cstrlen(&str[1]);
    }

    /**
     * \brief A consexpr version of C library memcmp()
     * 
     * \param lhs Pointer to first byte array to compare
     * \param rhs Pointer to second byte array to compare
     * \param n Maximum number of bytes to compare
     * \return 0 if two arrays are equal, < 0 if lhs is less than rhs, else > 0
     * \note If the two arrays are equal up to n characters, the shorter string is less then the longer string 
     */
    constexpr int cmemcmp(const void* lhs, const void* rhs, size_t n) noexcept
    {
        return 
        (n == 0) ? 0 :
            (static_cast<const char*>(lhs)[0] < static_cast<const char*>(rhs)[0]) ? -1 :
                (static_cast<const char*>(lhs)[0] > static_cast<const char*>(rhs)[0]) ? 1 :
                    cmemcmp(&static_cast<const char*>(lhs)[1], &static_cast<const char*>(rhs)[1], n-1);
    }

    /**
     * \brief Custom string view implementaiton for C++11, does not own the string it views.
     * \note The string viewed may not be null terminated (it may contain null characters)
     */
    class StringView 
    {
    public:

        class const_iterator : public std::iterator<
                                                    std::forward_iterator_tag,
                                                    const char&,
                                                    size_t,
                                                    const char*,
                                                    const char&
                                                    > 
        {
        public:
            explicit const_iterator(const StringView& s, size_t i = 0) noexcept
                : s(s), i(i) {}

            const_iterator& operator++() noexcept
            {
                ++i;
                return *this;
            }

            bool operator==(const const_iterator& other) const noexcept
            {
                return (this->s == other.s) && (this->i == other.i);
            }

            bool operator!=(const const_iterator& other) const noexcept
            {
                return !(*this == other);
            }

            const char& operator*() const noexcept
            { 
                return s[i];
            }

        private:
            const StringView& s;
            size_t i;
        };
        
        typedef std::reverse_iterator<const_iterator> const_reverse_iterator;
        typedef const_iterator iterator;
        typedef const_reverse_iterator reverse_iterator;

        static constexpr size_t npos = std::numeric_limits<size_t>::max();

        /**
         * \brief Create a string view of the empty string, which has length 0
         * 
         */
        constexpr StringView() noexcept
            : data_(nullptr), length_(0) {}

        /**
         * \brief Construct a string view over a C-string, which means the string must be null terminated
         * Length of string view will not include null terminator
         */
        constexpr StringView(const char* str) noexcept
            : data_(str), length_(cstrlen(str)) {}

        /**
         * \brief Construct a string view over characters pointer to by str with length size
         * 
         * \param str Pointer to character string, may contain null terminator(s)
         * \param size Size of the string
         */
        constexpr StringView(const char* str, size_t size) noexcept
            : data_(str), length_(size) {}

        constexpr StringView(std::nullptr_t) = delete;

        constexpr StringView(const StringView&) noexcept = default;
        StringView& operator=(const StringView&) noexcept = default;

        const_iterator begin() const noexcept;

        const_iterator end() const noexcept;

        /**
         * \sa at()
         */
        constexpr const char& operator[](size_t n) const
        {
            return at(n);
        }

        /**
         * \brief Boolean operator, check if string is not empty
         * 
         * \return true if string is not empty
         * \return false if string is empty
         * \sa empty()
         */
        constexpr operator bool() const
        {
            return !this->empty();
        }

        /**
         * \brief Access the character at position n in string
         * 
         * \param n 0-based index of character to access
         * \return Reference to character at position n
         * \warning Caller is responsible for checking buffer overflow/underflow
         */
        constexpr const char& at(size_t n) const
        {
            return data_[n];
        }

        /**
         * \brief Access internal character buffer held by string view
         * 
         * \return Return const pointer to character buffer 
         */
        constexpr const char* data() const noexcept
        {
            return data_;
        }

        /**
         * \return true if the string lenbth is 0
         * \return false if the string length is not 0
         */
        constexpr bool empty() const noexcept
        {
            return (length() == 0);
        }      

        /**
         * \brief Get the length of string in view
         * 
         * \return Length of string
         */
        constexpr size_t length() const noexcept
        {
            return length_;
        }

        /**
         * \sa length() 
         */
        constexpr size_t size() const noexcept
        {
            return length();
        }

        /**
         * \param pos 0-based offset (starting index) of string
         * \param count Maximum length of substring from pos
         * Will not overflow buffer
         * \return String view over substring of this, or empty string if pos >= length()
         */
        constexpr StringView substr(size_t pos = 0,
                                    size_t count = npos) const noexcept
        {
            return (pos >= length_) ? "" : StringView(&data_[pos], min(length_ - pos, count));
        }

        operator std::string() const;

        friend constexpr int compare(const StringView&, const StringView&) noexcept;
        friend constexpr int compare(const StringView&, const StringView&, size_t n) noexcept;
        friend constexpr bool operator<(const StringView&, const StringView&) noexcept;
        friend constexpr bool operator>(const StringView&, const StringView&) noexcept;
        friend constexpr bool operator==(const StringView&, const StringView&) noexcept;
        friend constexpr bool operator!=(const StringView&, const StringView&) noexcept;

    private:

        const char* data_;
        size_t length_;
    };
    
    std::ostream& operator<<(std::ostream&, const StringView&);

    // TODO size_t may be larger than int .
    constexpr int compare(const StringView& str1, const StringView& str2) noexcept
    {
        return (str1 == str2) ? 0 : (str1 < str2) ? -1 : 1;
    }

    constexpr int compare(const StringView& str1, const StringView& str2, size_t n) noexcept
    {
        return compare(str1.substr(0, n), str2.substr(0, n));
    }

    constexpr bool operator<(const StringView& str1, const StringView& str2) noexcept
    {
        return (cmemcmp(str1.data(), str2.data(), min(str1.length(), str2.length())) < 0)
            || (cmemcmp(str1.data(), str2.data(), min(str1.length(), str2.length())) == 0 && str1.length() < str2.length());
    }

    constexpr bool operator>(const StringView& str1, const StringView& str2) noexcept
    {
        return (cmemcmp(str1.data(), str2.data(), min(str1.length(), str2.length())) > 0)
            || (cmemcmp(str1.data(), str2.data(), min(str1.length(), str2.length())) == 0 && str1.length() > str2.length());
    }

    // constexpr bool operator>(const StringView& str1, const StringView& str2) noexcept
    // {
    //     return (str1.length() > str2.length()) || (memcmp(str1.data(), str2.data(), str1.length()) > 0); 
    // }

    constexpr bool operator==(const StringView& str1, const StringView& str2) noexcept
    {
        return (str1.length() == str2.length()) && (cmemcmp(str1.data(), str2.data(), str1.length()) == 0); 
    }

    constexpr bool operator!=(const StringView& str1, const StringView& str2) noexcept
    {
        return !(str1 == str2);
    }
}

template <>
struct std::hash<const sstest::StringView>
{

    size_t operator()(const sstest::StringView& s) const noexcept
    {
        uint64_t result = 0;
        // for (char c : s)
        // {
        //     result = result ^ std::hash<char>{}(c);
        // }
        // TODO decide based on platform
        int len = static_cast<int>(std::min(static_cast<size_t>(std::numeric_limits<int>::max()), s.length()));
        result = hash_functions::murmur::murmurHash64A(s.data(), len, murmurSeed);
        return static_cast<size_t>(result);
    }

    static const unsigned int murmurSeed;
    
};

template <>
struct std::hash<sstest::StringView>
{

    size_t operator()(const sstest::StringView& s) const noexcept
    {
        uint64_t result = 0;
        // for (char c : s)
        // {
        //     result = result ^ std::hash<char>{}(c);
        // }
        // TODO decide based on platform
        int len = static_cast<int>(std::min(static_cast<size_t>(std::numeric_limits<int>::max()), s.length()));
        result = hash_functions::murmur::murmurHash64A(s.data(), len, murmurSeed);
        return static_cast<size_t>(result);
    }

    static const unsigned int murmurSeed;
    
};

#endif // _SSTEST_STRING_H_