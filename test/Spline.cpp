#include "Timer.h"
#include "../MTK/Spline.h"

#include <chrono>
#include <iostream>

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
    Spline s = fitSpline(3,
                         ConditionList<Real>({{-2.0, ConditionList<Real>::Condition({{0, -1.0}})},
                                              {-1.0, ConditionList<Real>::Condition({{0, 0.0}})},
                                              {0.0, ConditionList<Real>::Condition({{0, 1.0}})},
                                              {1.0, ConditionList<Real>::Condition({{0, 0.0}})},
                                              {2.0, ConditionList<Real>::Condition({{0, -1.0}})}}),
                         true);

    if (std::abs(-1 - s(-2)) > Trait<float>::epsilon() ||
        std::abs(0 - s(-1)) > Trait<float>::epsilon() ||
        std::abs(1 - s(0)) > Trait<float>::epsilon() ||
        std::abs(0 - s(1)) > Trait<float>::epsilon() ||
        std::abs(-1 - s(2)) > Trait<float>::epsilon())
    {
        printf("Error at: file %s line %d.", __FILE__, __LINE__);
        flag = FAIL;
    }

    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6ld(ms). Spline::Spline.\n", t);
    }
    return 0;
}