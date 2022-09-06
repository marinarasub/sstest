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

#ifndef _EXAMPLE_COMMON_H_
#define _EXAMPLE_COMMON_H_

#include <cmath>
#include <string>
#include <limits>
#include <cstdint>

/**
 * \file example_common.h
 * \brief Common type definitions that are used in SSTest examples
 * 
 */


#ifdef _MSC_VER
#   define IGNORE_WARNING_UNREACHABLE_BEGIN \
           __pragma(warning(push)) \
           __pragma(warning(disable : 4702)) 
#   define IGNORE_WARNING_UNREACHABLE_END \
           __pragma(warning(pop))
#else
#   define IGNORE_WARNING_UNREACHABLE_BEGIN
#   define IGNORE_WARNING_UNREACHABLE_END
#endif

namespace example
{

    /////////////////// linked_list //////////////////////////////

    // a simple linked list implementation
    template <typename T>
    class linked_list
    {
        struct node
        {
            node(const T& data)
                : data(data), next(nullptr) {}

            T data;
            node* next;
        };

    public:
        
        struct const_iterator
        {
            using iterator_category = std::forward_iterator_tag;
            using value_type = T;
            using difference_type = size_t;
            using pointer = T*;
            using reference = T&;

            const_iterator(node* node)
                : curr(node) {}

            bool operator==(const const_iterator& other) const noexcept
            {
                return curr == other.curr;
            }

            bool operator !=(const const_iterator& other) const noexcept
            {
                return curr != other.curr;
            }

            const_iterator& operator++()
            {
                curr = curr->next;
                return (*this);
            }

            T& operator*() const noexcept
            {
                return curr->data;
            }

        private:
            node* curr;

        };

        using iterator = const_iterator;

        linked_list() noexcept
            : length(0), head(nullptr), tail(nullptr) {}

        linked_list(const std::initializer_list<T>& init_list)
            : linked_list()
        {
            for (const T& t : init_list)
            {
                push_back(t);
            }
        }

        // copy constructor const&, new copy of each element
        linked_list(const linked_list& other)
            : linked_list()
        {
            copy(other);
        }

        // move constructor &&, can just take existing pointers
        linked_list(linked_list&& other) noexcept
            : linked_list()
        {
            move(std::move(other));
        }

        linked_list& operator=(const linked_list& other)
        {
            copy(other);
            return *this;
        }

         linked_list& operator=(linked_list&& other)
        {
            move(std::move(other));
            return *this;
        }

        ~linked_list() 
        {
            clear();
        }

        // clear all elements in the linked list
        void clear()
        {
            node* curr = head;
            while (curr != nullptr)
            {
                node* next = curr->next;
                delete curr;
                curr = next;
                --length;
            }
            head = nullptr;
            tail = nullptr;
        }

        // return the size of the linked list
        size_t size() const noexcept
        {
            return length;
        }

        // return true if the linked list is empty (contains 0 elements)
        bool empty() const noexcept
        {
            return length == 0;
        }

        const_iterator begin() const noexcept
        {
            return const_iterator(head);
        }

        const_iterator end() const noexcept
        {
            return const_iterator(nullptr);
        }

        // add element to the front of the linked list, at index 0. Existing elements' indexes will be therefore incremented by 1.
        T& push_front(const T& data)
        {
            node* new_node = new node(data);
            if (head == nullptr)
            {
                tail = new_node;
            }
            else
            {
                new_node->next = head;
            }
            head = new_node;
            ++length;
            return new_node->data;
        }

        // add element to back of the list, at index size()
        T& push_back(const T& data)
        {
            node* new_node = new node(data);
            if (head == nullptr)
            {
                head = new_node;
            }
            else
            {
                tail->next = new_node;
            }
            tail = new_node;
            ++length;
            return new_node->data;
        }

        // remove the first element in the list and return the value stored
        // calling pop_front() on empty list is undefined behavior
        T pop_front() noexcept
        {
            node* next = head->next;
            T popped = head->data;
            delete head;
            head = next;
            --length;
            return popped;
        }

        // remove the element and the back of the list and return the value stored
        // calling pop_back() on empty list is undefined behavior
        T pop_back() noexcept
        {
            if (length == 1)
            {
                T popped = head->data;
                delete head;
                head = nullptr;
                tail = nullptr;
                length = 0;
                return popped;
            }
            else
            {
                node* tail_prev = find(length - 2);
                T popped = tail->data;
                delete tail;
                tail = tail_prev;
                --length;
                return popped;
            }
        }

        // add element to either front or back of the list
        T& push(const T& data, bool front = false)
        {
            if (front)
            {
                return push_front(data);
            }
            else
            {
                return push_back(data);
            }
        }

        // remove element from either front or back of the list
        // calling pop() on empty list is undefined behavior
        T pop(bool front = false) noexcept
        {
            if (front)
            {
                return pop_front();
            }
            else
            {
                return pop_back();
            }
        }

        // insert an element at specified index in the list
        // index must exist, or undefined behavior
        T& insert(const T& data, size_t index)
        {
            if (index == 0) return push_front(data);
            if (index == length) return push_back(data);
            node* curr = find(index - 1);
            node* new_node = new node(data);
            new_node->next = curr->next;
            curr->next = new_node;
            ++length;
            return new_node->data;
        }

        // return a reference to the value stored in first element in the list
        // list cannot be empty
        T& front() const noexcept
        {
            return head->data;
        }

        // return a reference to the value stored in last element in the list
        // list cannot be empty
        T& back() const noexcept
        {
            return tail->data;
        }

        // return a reference to the value stored at index specified. 
        // element at specified index must exist
        T& at(size_t index) const noexcept
        {
            return find(index)->data;
        }

        // equivalent to at()
        T& operator[](size_t index) const noexcept
        {
            return at(index);
        }

        // reverse the linked list such the elements are in reverse order (back to front)
        void reverse() noexcept
        {
            // reverse next pointers
            node* prev = nullptr;
            node* curr = head;
            while (curr != nullptr)
            {
                node* next = curr->next;
                curr->next = prev;
                prev = curr;
                curr = next;
            }
            // swap head and tail
            curr = tail;
            tail = head;
            head = curr;
        }

        // apply given function to all elements in the list
        // The map function must take a single parameter which can take type T (does not necessarily have to be , i.e. T&, const T&, or convertible from T)
        template <typename MapFunc>
        void map(const MapFunc& map_func) const noexcept
        {
            node* curr = head;
            while (curr != nullptr)
            {
                curr->data = map_func(curr->data);
                curr = curr->next;
            }
        }

    protected:

        void copy(const linked_list& other)
        {
            clear();
            for (const T& t : other)
            {
                this->push_back(t);
            }
        }

        void move(linked_list&& other)
        {
            clear();
            this->head = other.head;
            this->tail = other.tail;
            this->length = other.length;
            other.head = nullptr;
            other.tail = nullptr;
            other.length = 0;
        }

    private:

        node* find(size_t index) const noexcept
        {
            size_t i = index;
            node* curr = head;
            while (i > 0)
            {
                curr = curr->next;
                --i;
            }
            return curr;
        }

        size_t length;
        node* head;
        node* tail;
    };

    ////////////////////////// vector 2D ////////////////////////////////////

    // simple vector2 class to test different types of operators
    // a vector is a good example as there are many ways to compare two vectors
    template <typename Rep = float>
    struct vector2
    {
        using rep = Rep;

        constexpr vector2() noexcept
            : x(0), y(0) {}

        constexpr vector2(Rep x, Rep y)  noexcept
            : x(x), y(y) {}

        constexpr vector2(const vector2&) = default;
        vector2& operator=(const vector2&) = default;

        static constexpr double pi = 3.141592653589793238462643383279502884;

        //
        // vector functions
        //

        // return the zero vector
        static constexpr vector2 zero() noexcept
        {
            return vector2(0, 0);
        }

        // check if parallel within some lenience
        static constexpr bool is_parallel(vector2 lhs, vector2 rhs) noexcept
        {
            return std::abs(std::abs((lhs * rhs) * (lhs * rhs)) - std::abs((rhs * rhs) * (lhs * lhs))) 
                <= 64 * std::max(lhs.magnitude_sqr(), rhs.magnitude_sqr()) * std::numeric_limits<Rep>::epsilon();
        }

        // check if perpindicular within some lenience
        static constexpr bool is_perp(vector2 lhs, vector2 rhs) noexcept
        {
            return std::abs(lhs * rhs) 
                <= 8 * std::max(lhs.magnitude_sqr(), rhs.magnitude_sqr()) * std::numeric_limits<Rep>::epsilon();
        }

        // return squared magnitude. save an sqrt() if just comparing magnitudes 
        constexpr Rep magnitude_sqr() const noexcept
        {
            return x * x + y * y;
        }

        // equivalent to magnitude_sqr()
        constexpr Rep length_sqr() const noexcept
        {
            return magnitude_sqr();
        }

        // return the scalar magnitude of the vector
        constexpr Rep magnitude() const noexcept
        {
            return *this ? std::sqrt(magnitude_sqr()) : 0;
        }

        // equivalent to magnitude()
        constexpr Rep length() const noexcept
        {
            return magnitude();
        }
        
        // return the unit vector from this, that is vector of length 1
        constexpr vector2 unit() const noexcept
        {
            return *this ? vector2(x / magnitude(), y / magnitude()) : *this;
        }

        // equivalent to unit()
        constexpr vector2 direction() const noexcept
        {
            return unit();
        }

        // return angle in radians [0, 2pi)
        constexpr double angle() const noexcept
        {
            return *this ? (y < 0 ? (std::atan2(y, x) + 2 * pi) : (std::atan2(y, x))) : 0;
        }

        constexpr vector2 scale(Rep scalar) const noexcept
        {
            return vector2(x * scalar, y * scalar);
        }

        // return projection of other on this
        constexpr vector2 proj(vector2 other) const noexcept
        {
            return *this ? ((*this * other) / magnitude_sqr()) * (*this) : *this;
        }
        
        // return rejection of other on this
        constexpr vector2 rej(vector2 other) const noexcept
        {
            return *this ? other - proj(other) : *this;
        }

        // return a perpendicular vector (does not normalize to unit vector)
        constexpr vector2 perp() const noexcept
        {   
            // rotate by pi/2 radians
            return vector2(-y, x);
        }

        //
        // unary operations
        // 

        // return negative vector of this
        constexpr vector2 operator-() const noexcept
        {
            return vector2(-x, -y);
        }
        
        // return true if vector is non-zero
        constexpr operator bool() const noexcept
        {
            return *this != zero();
        }

        //
        // comparison
        //

        // vectors must be equal, not just magnitude. For comparing magnitude, use magnitude()
        template <typename Rep2> friend constexpr bool operator==(vector2<Rep2>, vector2<Rep2>) noexcept;
        template <typename Rep2> friend constexpr bool operator!=(vector2<Rep2>, vector2<Rep2>) noexcept;

        // compare magnitude only
        template <typename Rep2> friend constexpr bool operator<(vector2<Rep2>, vector2<Rep2>) noexcept;
        // compare magnitude only
        template <typename Rep2> friend constexpr bool operator>(vector2<Rep2>, vector2<Rep2>) noexcept;
        // compare magnitude only
        template <typename Rep2> friend constexpr bool operator<=(vector2<Rep2>, vector2<Rep2>) noexcept;
        // compare magnitude only
        template <typename Rep2> friend constexpr bool operator>=(vector2<Rep2>, vector2<Rep2>) noexcept;

        //
        // binary arithmetic operators
        //

        // cross product would require 3D vector, so for sake of simplicitly we don't use it.

        // dot product
        template <typename Rep2> friend constexpr Rep2 operator*(vector2<Rep2>, vector2<Rep2>) noexcept;
        // scalar multiplicaiton
        template <typename Rep2> friend constexpr vector2<Rep2> operator*(Rep2, vector2<Rep2>) noexcept;
        template <typename Rep2> friend constexpr vector2<Rep2> operator+(vector2<Rep2>, vector2<Rep2>) noexcept;
        template <typename Rep2> friend constexpr vector2<Rep2> operator-(vector2<Rep2>, vector2<Rep2>) noexcept;

        // to_string funciton (can be used by SSTest to print value)
        template <typename Rep2> friend std::string to_string(const vector2<Rep2>&);

        Rep x;
        Rep y;
    };

    using vector2f = vector2<float>;
    using vector2i = vector2<int>;

    template <typename Rep> constexpr double vector2<Rep>::pi;

    // comparison

    template <typename Rep> constexpr bool operator==(vector2<Rep> lhs, vector2<Rep> rhs) noexcept
    {
        return (lhs.x == rhs.x) && (lhs.y == rhs.y);
    }

    template <typename Rep> constexpr bool operator!=(vector2<Rep> lhs, vector2<Rep> rhs) noexcept
    {
        return (lhs.x != rhs.x) || (lhs.y != rhs.y);
    }

    template <typename Rep> constexpr bool operator<(vector2<Rep> lhs, vector2<Rep> rhs) noexcept
    {
        return lhs.magnitude_sqr() < rhs.magnitude_sqr();
    }

    template <typename Rep> constexpr bool operator>(vector2<Rep> lhs, vector2<Rep> rhs) noexcept
    {
        return lhs.magnitude_sqr() > rhs.magnitude_sqr();
    }

    template <typename Rep> constexpr bool operator<=(vector2<Rep> lhs, vector2<Rep> rhs) noexcept
    {
        return lhs.magnitude_sqr() <= rhs.magnitude_sqr();
    }

    template <typename Rep> constexpr bool operator>=(vector2<Rep> lhs, vector2<Rep> rhs) noexcept
    {
        return lhs.magnitude_sqr() >= rhs.magnitude_sqr();
    }

    // arithmetic

    template <typename Rep> constexpr Rep operator*(vector2<Rep> lhs, vector2<Rep> rhs) noexcept
    {
        return lhs.x * rhs.x + lhs.y * rhs.y;
    }

    template <typename Rep> constexpr vector2<Rep> operator*(Rep scalar, vector2<Rep> vec) noexcept
    {
        return vec.scale(scalar);
    }

    template <typename Rep> constexpr vector2<Rep> operator+(vector2<Rep> lhs, vector2<Rep> rhs) noexcept
    {
        return vector2<Rep>(lhs.x + rhs.x, lhs.y + rhs.y);
    }
    
    template <typename Rep> constexpr vector2<Rep> operator-(vector2<Rep> lhs, vector2<Rep> rhs) noexcept
    {
        return vector2<Rep>(lhs.x - rhs.x, lhs.y - rhs.y);
    }

    template <typename Rep> std::string to_string(const vector2<Rep>& vec)
    {
        return std::string() + "(" + std::to_string(vec.x) + ", " + std::to_string(vec.y) + ")";
    }


    // A non-const compare example
    ////////////////////////////// comparison proxy //////////////////////////////////

    // using proxy pattern, we can see how many comparisons are made to an object (i.e. with qsort)
    // this object will be non const
    template <typename T>
    class compare_proxy
    {
    public:

        compare_proxy(const T& obj)
            : obj(obj), times_compared(0) {}

        bool operator<(compare_proxy& other)
        {
            this->times_compared++;
            other.times_compared++;
            return this->obj < other.obj;
        }

        bool operator==(compare_proxy& other)
        {
            this->times_compared++;
            other.times_compared++;
            return this->obj == other.obj;
        }

        size_t get_times_compared() const noexcept
        {
            return times_compared;
        }

        const T& get_object() const noexcept
        {
            return obj;
        }

        template <typename T1, typename T2> friend int compare_proxy_compare(compare_proxy<T1>&, compare_proxy<T2>&);

    private:
        T obj;
        size_t times_compared;
    };

    template <typename T1, typename T2>
    bool compare_proxy_compare(compare_proxy<T1>& lhs, compare_proxy<T2>& rhs)
    {
        return (lhs < rhs);
    }

    // Xorshift32 by George Marsaglia
    // Marsaglia, G. (2003). Xorshift RNGs. Journal of Statistical Software, 8(14), 1�6. https://doi.org/10.18637/jss.v008.i14
    inline uint32_t xorshift32(uint32_t seed)
    {
        static uint32_t x = seed;
        x ^= x << 13;
        x ^= x >> 17;
        x ^= x << 5;
        return x;
    }
}


#endif // _EXAMPLE_COMMON_H_