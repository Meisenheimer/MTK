#include "Timer.h"
#include "../src/Random.h"

#include <chrono>
#include <iostream>

#include <Eigen/Eigen>

using namespace mtk;

constexpr bool PASS = true;
constexpr bool FAIL = !PASS;
constexpr double DELTA = std::numeric_limits<float>::epsilon();

int main()
{
    size_t t;
    bool flag = PASS;
    timer();
    Random random_num;
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). Random.\n", t);
    }
    return 0;
}