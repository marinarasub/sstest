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

#include "sstest/sstest_string.h"

#include <random>
#include <limits>
#include <string>
#include <ostream>
#include <cctype>

namespace hash_functions
{

// disable some warnings for hash functions
#ifdef __clang__
_Pragma("clang diagnostic push")
_Pragma("clang diagnostic ignored \"-Wsign-conversion\"")
#endif 
    namespace murmur
    {

        uint32_t murmurHashNeutral32 ( const void * key, int len, unsigned int seed )
        {
            const uint32_t m = 0x5bd1e995;
            const int r = 24;

            uint32_t h = seed ^ static_cast<uint32_t>(len);

            const unsigned char * data = (const unsigned char *)key;

            while(len >= 4)
            {
                uint32_t k;

                k  = data[0];
                k |= data[1] << 8;
                k |= data[2] << 16;
                k |= data[3] << 24;

                k *= m; 
                k ^= k >> r; 
                k *= m;

                h *= m;
                h ^= k;

                data += 4;
                len -= 4;
            }
            
            switch(len)
            {
            case 3: h ^= data[2] << 16; /* fallthrough */
            case 2: h ^= data[1] << 8; /* fallthrough */
            case 1: h ^= data[0];
                    h *= m;
            };

            h ^= h >> 13;
            h *= m;
            h ^= h >> 15;

            return h;
        } 

        uint32_t murmurHash32 ( const void * key, int len, unsigned int seed )
        {
            // 'm' and 'r' are mixing constants generated offline.
            // They're not really 'magic', they just happen to work well.

            const uint32_t m = 0x5bd1e995;
            const int r = 24;

            // Initialize the hash to a 'random' value

            uint32_t h = seed ^ static_cast<uint32_t>(len);

            // Mix 4 bytes at a time into the hash

            const unsigned char * data = (const unsigned char *)key;

            while(len >= 4)
            {
                uint32_t k = *(uint32_t *)data;

                k *= m; 
                k ^= k >> r; 
                k *= m; 
                
                h *= m; 
                h ^= k;

                data += 4;
                len -= 4;
            }
            
            // Handle the last few bytes of the input array

            switch(len)
            {
            case 3: h ^= data[2] << 16; /* fallthrough */
            case 2: h ^= data[1] << 8; /* fallthrough */
            case 1: h ^= data[0];
                    h *= m;
            };

            // Do a few final mixes of the hash to ensure the last few
            // bytes are well-incorporated.

            h ^= h >> 13;
            h *= m;
            h ^= h >> 15;

            return h;
        } 


        uint64_t murmurHash64A ( const void * key, int len, unsigned int seed )
        {
            const uint64_t m = 0xc6a4a7935bd1e995;
            const int r = 47;

            uint64_t h = seed ^ (len * m);

            const uint64_t * data = (const uint64_t *)key;
            const uint64_t * end = data + (len/8);

            while(data != end)
            {
                uint64_t k = *data++;

                k *= m; 
                k ^= k >> r; 
                k *= m; 
                
                h ^= k;
                h *= m; 
            }

            const unsigned char * data2 = (const unsigned char*)data;

            switch(len & 7)
            {
            case 7: h ^= uint64_t(data2[6]) << 48; /* fallthrough */
            case 6: h ^= uint64_t(data2[5]) << 40; /* fallthrough */
            case 5: h ^= uint64_t(data2[4]) << 32; /* fallthrough */
            case 4: h ^= uint64_t(data2[3]) << 24; /* fallthrough */
            case 3: h ^= uint64_t(data2[2]) << 16; /* fallthrough */
            case 2: h ^= uint64_t(data2[1]) << 8; /* fallthrough */
            case 1: h ^= uint64_t(data2[0]);
                    h *= m;
            };
        
            h ^= h >> r;
            h *= m;
            h ^= h >> r;

            return h;
        } 

        // 64-bit hash for 32-bit platforms

        uint64_t murmurHash64B ( const void * key, int len, unsigned int seed )
        {
            const unsigned int m = 0x5bd1e995;
            const int r = 24;

            unsigned int h1 = seed ^ len;
            unsigned int h2 = 0;

            const unsigned int * data = (const unsigned int *)key;

            while(len >= 8)
            {
                unsigned int k1 = *data++;
                k1 *= m; k1 ^= k1 >> r; k1 *= m;
                h1 *= m; h1 ^= k1;
                len -= 4;

                unsigned int k2 = *data++;
                k2 *= m; k2 ^= k2 >> r; k2 *= m;
                h2 *= m; h2 ^= k2;
                len -= 4;
            }

            if(len >= 4)
            {
                unsigned int k1 = *data++;
                k1 *= m; k1 ^= k1 >> r; k1 *= m;
                h1 *= m; h1 ^= k1;
                len -= 4;
            }

            switch(len)
            {
            case 3: h2 ^= ((unsigned char*)data)[2] << 16; /* fallthrough */
            case 2: h2 ^= ((unsigned char*)data)[1] << 8; /* fallthrough */
            case 1: h2 ^= ((unsigned char*)data)[0]; 
                    h2 *= m;
            };

            h1 ^= h2 >> 18; h1 *= m;
            h2 ^= h1 >> 22; h2 *= m;
            h1 ^= h2 >> 17; h1 *= m;
            h2 ^= h1 >> 19; h2 *= m;

            uint64_t h = h1;

            h = (h << 32) | h2;

            return h;
        } 

    }
#ifdef __clang__
_Pragma("clang diagnostic pop")
#endif 

}

namespace sstest
{

    StringView::const_iterator StringView::begin() const noexcept
    {
        return const_iterator(*this);
    }

    StringView::const_iterator StringView::end() const noexcept
    {
        return const_iterator(*this, length());
    }

    StringView::operator std::string() const
    {
        return std::string(data_, length_);
    }

    std::ostream& operator<<(std::ostream& os, const StringView& str)
    {
        os << str.data();
        return os;
    }
}

namespace 
{
    static std::uniform_int_distribution<unsigned> distribution(
        std::numeric_limits<unsigned>::lowest(), 
        std::numeric_limits<unsigned>::max()
        );

    static std::mt19937 generator;
    static const unsigned int murmurSeed = distribution(generator);
}

const unsigned std::hash<const sstest::StringView>::murmurSeed = murmurSeed;
const unsigned std::hash<sstest::StringView>::murmurSeed = murmurSeed;