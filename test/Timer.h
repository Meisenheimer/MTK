#ifndef MTK_TEST_TIMER_H
#define MTK_TEST_TIMER_H

#include "../src/Config.h"

namespace mtk
{
    Int timer();

    inline Int timer()
    {
        static std::chrono::_V2::steady_clock::time_point time = std::chrono::steady_clock::now();
        std::chrono::_V2::steady_clock::time_point now = std::chrono::steady_clock::now();
        Int duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - time).count();
        time = now;
        return duration;
    }
};

#endif