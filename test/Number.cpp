#include "Timer.h"
#include "../src/Number.h"

using namespace mtk;

constexpr bool PASS = true;
constexpr bool FAIL = !PASS;

int main()
{
    size_t t;
    bool flag = PASS;
    Prime p(25);
    std::vector<size_t> list;

    timer();
    flag = PASS;
    for (size_t i = 0; i < Trait<unsigned short>::max(); i++)
    {
        double x = Random::uniform<double>(0, 8);
        size_t n = Random::uniform<size_t>(0, 8);
        if (std::pow(x, n) != pow(x, n))
        {

            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            flag = FAIL;
        }
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). Number::pow.\n", t);
    }

    timer();
    flag = PASS;
    for (size_t i = 0; i < Trait<unsigned short>::max(); i++)
    {
        size_t x = Random::uniform<size_t>(0, 8);
        size_t n = Random::uniform<size_t>(0, 8);
        size_t m = Random::uniform<size_t>(1, 128);
        if (((size_t)std::pow(x, n) % m) != pow(x, n, m))
        {

            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            flag = FAIL;
        }
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). Number::pow(mod).\n", t);
    }

    timer();
    flag = PASS;
    for (size_t i = 0; i < Trait<unsigned short>::max(); i++)
    {
        size_t x = Random::uniform<size_t>(0, Trait<size_t>::max());
        size_t y = Random::uniform<size_t>(0, Trait<size_t>::max());
        if (std::gcd(x, y) != gcd(x, y))
        {

            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            flag = FAIL;
        }
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). Number::gcd.\n", t);
    }

    timer();
    list = {2, 3, 5, 7, 11, 13, 17, 19, 23};
    for (size_t i = 0; i < list.size(); i++)
    {
        if (!p(list[i]) || p.index(list[i]) != i || !isPrime(list[i]))
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
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
    for (size_t i = 0; i < list.size(); i++)
    {
        if (!p(list[i]) || !isPrime(list[i]))
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
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
    for (size_t i = 0; i < list.size(); i++)
    {
        if (p(list[i]) || isPrime(list[i]))
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
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