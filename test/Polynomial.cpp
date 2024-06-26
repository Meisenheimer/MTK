#include "Timer.h"
#include "../src/Polynomial.h"

using namespace mtk;

constexpr Bool PASS = true;
constexpr Bool FAIL = !PASS;
constexpr Real DELTA = std::numeric_limits<float>::epsilon();

int main()
{
    Bool flag = PASS;
    Int t;
    timer();
    Polynomial p1({1.0, 2.0, 1.0});
    Polynomial p2({-1.0, 0.0, 1.0});
    Polynomial p3({0.0, 2.0, 2.0});
    Polynomial p4({0.0, -2.0, -2.0});
    Polynomial p5({-1.0, 0.0, -2.0, 2.0, 1.0});
    if (p3 != -p4 || p1 + p2 != p3 || p3 - p1 != p2 || -1 * p4 != p3 || p4 / -1 != p3)
    {
        MTK_ALERT
        flag = FAIL;
    }
    Polynomial p = p1 * p2;
    auto root = p.root(std::sqrt(EPS<float>));
    if (root.size() != 4 || !p.isRoot(root[0]) || !p.isRoot(root[1]) || !p.isRoot(root[2]) || !p.isRoot(root[3]))
    {
        MTK_ALERT
        flag = FAIL;
    }
    p += 10.0;
    root = p.root(std::sqrt(EPS<float>));
    if (root.size() != 0)
    {
        MTK_ALERT
        flag = FAIL;
    }

    ConditionList c({{0.0, ConditionList::Condition({{0, p1(0)}})},
                     {1.0, ConditionList::Condition({{0, p1(1)}})},
                     {2.0, ConditionList::Condition({{0, p1(2)}})}});
    Polynomial tmp = fitPolynomial(2, c);
    if (std::abs(tmp(0) - p1(0)) > EPS<float> ||
        std::abs(tmp(1) - p1(1)) > EPS<float> ||
        std::abs(tmp(2) - p1(2)) > EPS<float>)
    {
        MTK_ALERT
        flag = FAIL;
    }

    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). Optimizer.\n", t);
    }
    return 0;
}