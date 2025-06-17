#include "Timer.h"
#include "../src/Random.h"

#include <chrono>
#include <iostream>

#include <Eigen/Eigen>

using namespace mtk;

using Real = long double;

constexpr bool PASS = true;
constexpr bool FAIL = !PASS;
constexpr Real DELTA = std::numeric_limits<float>::epsilon();

int main()
{
    size_t t;
    bool flag = PASS;
    timer();
    Random random_num;
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6ld(ms). Random::Random.\n", t);
    }
    return 0;
}