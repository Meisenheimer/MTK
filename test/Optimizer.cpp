#include "Timer.h"
#include "../src/Optimizer.h"

using namespace mtk;

using Real = long double;

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

const std::vector<std::pair<OptimizeMethod, LineSearch>> method =
    {
        {OptimizeMethod::GradientDescent, LineSearch::Bisection},
        {OptimizeMethod::GradientDescent, LineSearch::Fibonacci},
        {OptimizeMethod::GradientDescent, LineSearch::Newton},
        {OptimizeMethod::GradientDescent, LineSearch::Trivial},
        {OptimizeMethod::Newton, LineSearch::Bisection},
        {OptimizeMethod::Newton, LineSearch::Fibonacci},
        {OptimizeMethod::Newton, LineSearch::Newton},
        {OptimizeMethod::Newton, LineSearch::Trivial},
        {OptimizeMethod::QuasiNewton, LineSearch::Bisection},
        {OptimizeMethod::QuasiNewton, LineSearch::Fibonacci},
        {OptimizeMethod::QuasiNewton, LineSearch::Newton},
        {OptimizeMethod::QuasiNewton, LineSearch::Trivial},
};

int main()
{
    size_t t;
    bool flag = PASS;
    Vector<Real> x0(2);
    x0(0) = 1;
    x0(1) = -1;
    Vector<Real> r(2);
    r(0) = -0.5;
    r(1) = -0.25;
    Optimizer<Real> solver;
    solver.setFunction(f, g, G);
    std::vector<Vector<Real>> res;

    timer();
    for (size_t i = 0; i < method.size(); i++)
    {
        solver.setMethod(method[i].first, method[i].second);
        if ((solver.solve(x0) - r).lpNorm<2>() > DELTA)
        {
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
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