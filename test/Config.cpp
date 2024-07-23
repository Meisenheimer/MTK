#include "Timer.h"
#include "../src/Config.h"
#include "../src/Random.h"

using namespace mtk;

constexpr bool PASS = true;
constexpr bool FAIL = !PASS;
constexpr Real DELTA = 1e-4;

int main()
{
    bool flag = PASS;
    int t;

    timer();
    flag = PASS;
    for (Int i = 0; i < MAX<unsigned short>; i++)
    {
        Real x = Random::Uniform<Real>(0, 8);
        Int n = Random::Uniform<size_t>(0, 8);
        if (std::pow(x, n) != pow(x, n))
        {
            MTK_ALERT
            flag = FAIL;
        }
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). Config::pow.\n", t);
    }

    timer();
    flag = PASS;
    for (Int i = 0; i < MAX<unsigned short>; i++)
    {
        Int x = Random::Uniform<size_t>(0, 8);
        Int n = Random::Uniform<size_t>(0, 8);
        Int m = Random::Uniform<size_t>(1, 128);
        if (((Int)std::pow(x, n) % m) != pow(x, n, m))
        {
            MTK_ALERT
            flag = FAIL;
        }
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). Config::pow(mod).\n", t);
    }

    timer();
    flag = PASS;
    for (Int i = 0; i < MAX<unsigned short>; i++)
    {
        Int x = Random::Uniform<size_t>(0, MAX<Int>);
        Int y = Random::Uniform<size_t>(0, MAX<Int>);
        if (std::gcd(x, y) != gcd(x, y))
        {
            MTK_ALERT
            flag = FAIL;
        }
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). Config::gcd.\n", t);
    }

    return 0;
}