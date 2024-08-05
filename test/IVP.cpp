#include "Timer.h"
#include "../src/IVP.h"

using namespace mtk;

constexpr bool PASS = true;
constexpr bool FAIL = !PASS;
constexpr Real DELTA = 1e-4;

Vector<Real> u(const Real &t)
{
    return Vector<Real>::Ones(1) * (-t - 1);
}

Vector<Real> f(const Vector<Real> &u, const Real &t)
{
    return u + Vector<Real>::Ones(u.rows()) * t;
}

int main()
{
    bool flag;
    size_t t;
    LMM lmm;
    RK rk;
    lmm.setRHS(f);
    rk.setRHS(f);
    std::vector<std::pair<Vector<Real>, Real>> init = {{u(0), 0.0}, {u(0.005), 0.005}};

    timer();
    flag = PASS;
    lmm.setInitValue(init);
    lmm.setMethod(LMM::ForwardEuler);
    lmm.solve(0.5, 0.005);
    if ((lmm(0.5) - u(0.5)).lpNorm<2>() > DELTA)
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
    lmm.setInitValue(init);
    lmm.setMethod(LMM::Trapezoidal);
    lmm.solve(0.5, 0.005);
    if ((lmm(0.5) - u(0.5)).lpNorm<2>() > DELTA)
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
    lmm.setInitValue(init);
    lmm.setMethod(LMM::BackwardEuler);
    lmm.solve(0.5, 0.005);
    if ((lmm(0.5) - u(0.5)).lpNorm<2>() > DELTA)
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
    lmm.setInitValue(init);
    lmm.setMethod(LMM::Midpoint);
    lmm.solve(0.5, 0.005);
    if ((lmm(0.5) - u(0.5)).lpNorm<2>() > DELTA)
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
    rk.setMethod(RK::HeunThirdOrder);
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
    rk.setMethod(RK::ClassicalFourthOrder);
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