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

#ifndef _SSTEST_TIMER_H_
#define _SSTEST_TIMER_H_

#include <chrono>
#include <ratio>
#include <string>
#include "sstest_config.h"

/**
 * \file sstest_timer.h
 * \brief Contains a stopwatch class
 * 
 */

namespace sstest
{

    /**
     * \brief The Stopwatch class provides stopwatch funcitonality for measuring time intervals
     */
    class Stopwatch
    {
    public:

        /**
         * \brief Create a stopwatch object
         * 
         */
        Stopwatch();

        /**
         * \brief Reset the stopwatch state to default state
         * 
         */
        void reset();

        /**
         * \brief Start the timer, marks time when called as start time  
         * 
         */
        void start();

        using DefaultDurationType = std::chrono::duration<double, std::ratio<1>>;

        /**
         * \brief Lap the stopwatch
         * 
         * \tparam Duration A std::duration<Rep, Period> type 
         * \return Duration lap time in as Duration type
         */
        template <class Duration = DefaultDurationType>
        Duration lap()
        {
            using namespace std::chrono;

            Duration elapsed;
            if (running) 
            {
                steady_clock::time_point now_point = steady_clock::now();
                elapsed = std::chrono::duration_cast<Duration>(now_point - last_lap_point);
                last_lap_point = now_point;
            }
            else 
            {
                elapsed = std::chrono::duration_cast<Duration>(stop_point - last_lap_point);
            }
            return elapsed;
        }

        // TODO this is not reallya split,
        /**
         * \brief Split time since last lap
         * 
         * \tparam Duration 
         * \return Duration 
         */
        template <class Duration = DefaultDurationType>
        Duration split() const
        {
            using namespace std::chrono;

            Duration elapsed;
            if (running) 
            {
                steady_clock::time_point now_point = steady_clock::now();
                elapsed = std::chrono::duration_cast<Duration>(now_point - last_lap_point);
            }
            else 
            {
                elapsed = std::chrono::duration_cast<Duration>(stop_point - last_lap_point);
            }
            return elapsed;
        }

        /**
         * \brief Query the current running time (split) without modifying the stopwatch
         * 
         * \tparam Duration 
         * \return Duration 
         */
        template <class Duration = DefaultDurationType>
        Duration time() const
        {
            using namespace std::chrono;

            Duration elapsed;
            if (running) 
            {
                steady_clock::time_point now_point = steady_clock::now();
                elapsed = std::chrono::duration_cast<Duration>(now_point - start_point);
            }
            else 
            {
                elapsed = std::chrono::duration_cast<Duration>(stop_point - start_point);
            }
            return elapsed;
        }

        /**
         * \brief Stop the stopwatch. Any subsequent calls to query time will not include time after a call to stop()
         * \note May not be resumed with start(), which resets the stopwatch
         * 
         * \tparam Duration 
         * \return Duration 
         */
        template <class Duration = DefaultDurationType>
        Duration stop()
        {
            using namespace std::chrono;

            if (running) 
            {
                stop_point = steady_clock::now();
            }
            running = false;
            return time<Duration>();
        }

    private:

        std::chrono::steady_clock::time_point start_point;
        std::chrono::steady_clock::time_point last_lap_point;
        std::chrono::steady_clock::time_point stop_point;
        bool running;

    };

    // TODO
    // pretty print times
    // template <class T, class Period = std::ratio<1>>
    // std::string timeFormat(const T& value) // isoTime
    // {
    //     //std::milli;
    //     constexpr 
    // }
    
}

#endif // _SSTEST_TIMER_H_