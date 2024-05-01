#include "Timer.h"
#include "../src/Random.h"

#include <chrono>
#include <iostream>

#include <Eigen/Eigen>

using namespace mtk;

constexpr bool PASS = true;
constexpr bool FAIL = !PASS;
constexpr Real DELTA = std::numeric_limits<float>::epsilon();

int main()
{
    Int t;
    Bool flag = PASS;
    timer();
    Random random_num;
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). Random.\n", t);
    }
    return 0;
}