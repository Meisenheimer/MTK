#include "Timer.h"
#include "../src/Optimizer.h"

using namespace mtk;

constexpr bool PASS = true;
constexpr bool FAIL = !PASS;
constexpr Real DELTA = 1e-4;

const Real f(const Vector<Real> &x)
{
    return x(0) * x(0) + 2 * x(1) * x(1) + x(0) + x(1);
}

const Vector<Real> g(const Vector<Real> &x)
{
    Vector<Real> v(2);
    v(0) = 2 * x(0) + 1;
    v(1) = 4 * x(1) + 1;
    return v;
}

const Matrix<Real> G(const Vector<Real> &x)
{
    Matrix<Real> m = Matrix<Real>::Zero(2, 2);
    m(0, 0) = 2;
    m(1, 1) = 4;
    return m;
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
    Vector<Real> x0(2);
    x0(0) = 1;
    x0(1) = -1;
    Vector<Real> r(2);
    r(0) = -0.5;
    r(1) = -0.25;
    Optimizer solver;
    solver.setFunction(f, g, G);
    std::vector<Vector<Real>> res;

    timer();
    for (Int i = 0; i < (Int)method.size(); i++)
    {
        solver.setMethod(method[i].first, method[i].second);
        if ((solver.solve(x0) - r).lpNorm<2>() > DELTA)
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
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