#include "Timer.h"
#include "../src/Number.h"

using namespace mtk;

constexpr bool PASS = true;
constexpr bool FAIL = !PASS;

int main()
{
    Int t;
    bool flag = PASS;
    Prime p(25);
    List<Int> list;

    timer();
    list = {2, 3, 5, 7, 11, 13, 17, 19, 23};
    for (Int i = 0; i < list.size(); i++)
    {
        if (!p(list[i]) || p.index(list[i]) != i || !isPrime(list[i]))
        {
            MTK_ALERT
            flag = FAIL;
        }
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). Number::Prime.\n", t);
    }

    timer();
    list = {29, 31, 37, 577, 997};
    for (Int i = 0; i < list.size(); i++)
    {
        if (!p(list[i]) || !isPrime(list[i]))
        {
            MTK_ALERT
            flag = FAIL;
        }
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). Number::Prime.\n", t);
    }

    timer();
    list = {4, 6, 8, 9, 10, 12, 30, 49, 700, 800, 900};
    for (Int i = 0; i < list.size(); i++)
    {
        if (p(list[i]) || isPrime(list[i]))
        {
            MTK_ALERT
            flag = FAIL;
        }
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). Number::Prime.\n", t);
    }
    return 0;
}