#include "Timer.h"
#include "../src/Optimizer.h"

using namespace mtk;

constexpr bool PASS = true;
constexpr bool FAIL = !PASS;
constexpr Real DELTA = 1e-4;

const Real f(const Vector &x)
{
    return x(0) * x(0) + 2 * x(1) * x(1) + x(0) + x(1);
}

const Vector g(const Vector &x)
{
    return makeVector({2 * x(0) + 1, 4 * x(1) + 1});
}

const Matrix G(const Vector &x)
{
    return makeMatrix({{2, 0},
                       {0, 4}});
}

const std::vector<std::pair<Optimizer::Method, Optimizer::LineSearch>> method =
    {
        {Optimizer::Method::GradientDescent, Optimizer::LineSearch::Bisection},
        {Optimizer::Method::GradientDescent, Optimizer::LineSearch::Fibonacci},
        {Optimizer::Method::GradientDescent, Optimizer::LineSearch::Newton},
        {Optimizer::Method::GradientDescent, Optimizer::LineSearch::Trivial},
        {Optimizer::Method::Newton, Optimizer::LineSearch::Bisection},
        {Optimizer::Method::Newton, Optimizer::LineSearch::Fibonacci},
        {Optimizer::Method::Newton, Optimizer::LineSearch::Newton},
        {Optimizer::Method::Newton, Optimizer::LineSearch::Trivial},
        {Optimizer::Method::QuasiNewton, Optimizer::LineSearch::Bisection},
        {Optimizer::Method::QuasiNewton, Optimizer::LineSearch::Fibonacci},
        {Optimizer::Method::QuasiNewton, Optimizer::LineSearch::Newton},
        {Optimizer::Method::QuasiNewton, Optimizer::LineSearch::Trivial},
};

int main()
{
    Int t;
    bool flag = PASS;
    Vector x0 = makeVector({1, -1});
    Optimizer solver;
    solver.setFunction(f, g, G);
    std::vector<Vector> res;

    timer();
    for (Int i = 0; i < (Int)method.size(); i++)
    {
        solver.setMethod(method[i].first, method[i].second);
        if ((solver.solve(x0) - makeVector({-0.5, -0.25})).lpNorm<2>() > DELTA)
        {
            MTK_ALERT
            flag = FAIL;
        }
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). Optimizer.\n", t);
    }
    return 0;
}