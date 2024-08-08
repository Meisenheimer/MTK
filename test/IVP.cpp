#include "Timer.h"
#include "../src/IVP.h"

using namespace mtk;

using Real = long double;

constexpr bool PASS = true;
constexpr bool FAIL = !PASS;
constexpr Real DELTA = 1e-4;

Vector<Real> u(const Real &t)
{
    return Vector<Real>::Ones(1) * (-t - 1);
}

Vector<Var<Real>> f(const Vector<Var<Real>> &u, const Real &t)
{
    return u + Vector<Real>::Ones(u.rows()) * t;
}

int main()
{
    bool flag;
    size_t t;
    LM<Real> lm;
    RK<Real> rk;
    lm.setRHS(f);
    lm.getOpt().setLineSearch(LineSearch::Newton);
    lm.getOpt().setMethod(OptimizeMethod::Newton);
    rk.setRHS(f);
    rk.getOpt().setLineSearch(LineSearch::Newton);
    rk.getOpt().setMethod(OptimizeMethod::Newton);
    std::vector<std::pair<Vector<Real>, Real>> init = {{u(0), 0.0}, {u(0.005), 0.005}};

    timer();
    flag = PASS;
    lm.setInitValue(init);
    lm.setMethod(LMM::ForwardEuler);
    lm.solve(0.5, 0.005);
    if ((lm(0.5) - u(0.5)).lpNorm<2>() > DELTA)
    {
        printf("Error at: file %s line %d.", __FILE__, __LINE__);
        flag = FAIL;
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). IVP::LMM::ForwardEuler.\n", t);
    }

    timer();
    lm.setInitValue(init);
    lm.setMethod(LMM::Trapezoidal);
    lm.solve(0.5, 0.005);
    if ((lm(0.5) - u(0.5)).lpNorm<2>() > DELTA)
    {
        printf("Error at: file %s line %d.", __FILE__, __LINE__);
        flag = FAIL;
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). IVP::LMM::Trapezoidal.\n", t);
    }

    timer();
    lm.setInitValue(init);
    lm.setMethod(LMM::BackwardEuler);
    lm.solve(0.5, 0.005);
    if ((lm(0.5) - u(0.5)).lpNorm<2>() > DELTA)
    {
        printf("Error at: file %s line %d.", __FILE__, __LINE__);
        flag = FAIL;
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). IVP::LMM::BackwardEuler.\n", t);
    }

    timer();
    lm.setInitValue(init);
    lm.setMethod(LMM::Midpoint);
    lm.solve(0.5, 0.005);
    if ((lm(0.5) - u(0.5)).lpNorm<2>() > DELTA)
    {
        printf("Error at: file %s line %d.", __FILE__, __LINE__);
        flag = FAIL;
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). IVP::LMM::Midpoint.\n", t);
    }

    timer();
    rk.setInitValue(init);
    rk.setMethod(RKM::HeunThirdOrder);
    rk.solve(0.5, 0.005);
    if ((rk(0.5) - u(0.5)).lpNorm<2>() > DELTA)
    {
        printf("Error at: file %s line %d.", __FILE__, __LINE__);
        flag = FAIL;
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). IVP::RK::HeunThirdOrder.\n", t);
    }

    timer();
    rk.setInitValue(init);
    rk.setMethod(RKM::ClassicalFourthOrder);
    rk.solve(0.5, 0.005);
    if ((rk(0.5) - u(0.5)).lpNorm<2>() > DELTA)
    {
        printf("Error at: file %s line %d.", __FILE__, __LINE__);
        flag = FAIL;
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). IVP::RK::ClassicalFourthOrder.\n", t);
    }

    return 0;
}