#ifndef MTK_TEST_TIMER_H
#define MTK_TEST_TIMER_H

#include <cstddef>
#include <chrono>

namespace mtk
{
    size_t timer();

    inline size_t timer()
    {
        static std::chrono::_V2::steady_clock::time_point time = std::chrono::steady_clock::now();
        std::chrono::_V2::steady_clock::time_point now = std::chrono::steady_clock::now();
        size_t duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - time).count();
        time = now;
        return duration;
    }
};

#endif