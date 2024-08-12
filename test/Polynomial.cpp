#include "Timer.h"
#include "../src/Polynomial.h"

using namespace mtk;

using Real = long double;

constexpr bool PASS = true;
constexpr bool FAIL = !PASS;
constexpr Real DELTA = std::numeric_limits<float>::epsilon();

int main()
{
    bool flag = PASS;
    size_t t;
    timer();
    Polynomial<Real> p1({1.0, 2.0, 1.0});
    Polynomial<Real> p2({-1.0, 0.0, 1.0});
    Polynomial<Real> p3({0.0, 2.0, 2.0});
    Polynomial<Real> p4({0.0, -2.0, -2.0});
    Polynomial<Real> p5({-1.0, 0.0, -2.0, 2.0, 1.0});
    if (p3 != -p4 || p1 + p2 != p3 || p3 - p1 != p2 || -1 * p4 != p3 || p4 / -1 != p3)
    {
        printf("Error at: file %s line %d.", __FILE__, __LINE__);
        flag = FAIL;
    }
    Polynomial<Real> p = p1 * p2;
    auto root = p.root(std::sqrt(Trait<float>::epsilon()));
    if (root.size() != 4 || !p.isRoot(root[0]) || !p.isRoot(root[1]) || !p.isRoot(root[2]) || !p.isRoot(root[3]))
    {
        printf("Error at: file %s line %d.", __FILE__, __LINE__);
        flag = FAIL;
    }
    p += 10.0;
    root = p.root(std::sqrt(Trait<float>::epsilon()));
    if (root.size() != 0)
    {
        printf("Error at: file %s line %d.", __FILE__, __LINE__);
        flag = FAIL;
    }

    ConditionList<Real> c({{0.0, ConditionList<Real>::Condition({{0, p1(0)}})},
                           {1.0, ConditionList<Real>::Condition({{0, p1(1)}})},
                           {2.0, ConditionList<Real>::Condition({{0, p1(2)}})}});
    Polynomial<Real> tmp = fitPolynomial(2, c);
    if (std::abs(tmp(0) - p1(0)) > Trait<float>::epsilon() ||
        std::abs(tmp(1) - p1(1)) > Trait<float>::epsilon() ||
        std::abs(tmp(2) - p1(2)) > Trait<float>::epsilon())
    {
        printf("Error at: file %s line %d.", __FILE__, __LINE__);
        flag = FAIL;
    }

    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). Polynomial::Polynomial.\n", t);
    }
    return 0;
}