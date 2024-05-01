#include "Timer.h"
#include "../src/IVP.h"

using namespace mtk;

constexpr bool PASS = true;
constexpr bool FAIL = !PASS;
constexpr Real DELTA = 1e-4;

Vector u(const Real &t)
{
    return Vector::Ones(1) * (-t - 1);
}

Vector f(const Vector &u, const Real &t)
{
    return u + Vector::Ones(u.rows()) * t;
}

int main()
{
    Bool flag;
    Int t;
    LMM lmm;
    RK rk;
    lmm.setRHS(f);
    rk.setRHS(f);
    std::vector<Pair<Vector, Real>> init = {{u(0), 0.0}, {u(0.005), 0.005}};

    timer();
    flag = PASS;
    lmm.setInitValue(init);
    lmm.setMethod(LMM::Method::ForwardEuler);
    lmm.solve(0.5, 0.005);
    if ((lmm(0.5) - u(0.5)).lpNorm<2>() > DELTA)
    {
        MTK_ALERT
        flag = FAIL;
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). IVP::LMM::ForwardEuler.\n", t);
    }

    timer();
    lmm.setInitValue(init);
    lmm.setMethod(LMM::Method::Trapezoidal);
    lmm.solve(0.5, 0.005);
    if ((lmm(0.5) - u(0.5)).lpNorm<2>() > DELTA)
    {
        MTK_ALERT
        flag = FAIL;
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). IVP::LMM::Trapezoidal.\n", t);
    }

    timer();
    lmm.setInitValue(init);
    lmm.setMethod(LMM::Method::BackwardEuler);
    lmm.solve(0.5, 0.005);
    if ((lmm(0.5) - u(0.5)).lpNorm<2>() > DELTA)
    {
        MTK_ALERT
        flag = FAIL;
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). IVP::LMM::BackwardEuler.\n", t);
    }

    timer();
    lmm.setInitValue(init);
    lmm.setMethod(LMM::Method::Midpoint);
    lmm.solve(0.5, 0.005);
    if ((lmm(0.5) - u(0.5)).lpNorm<2>() > DELTA)
    {
        MTK_ALERT
        flag = FAIL;
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). IVP::LMM::Midpoint.\n", t);
    }

    timer();
    rk.setInitValue(init);
    rk.setMethod(RK::Method::HeunThirdOrder);
    rk.solve(0.5, 0.005);
    if ((rk(0.5) - u(0.5)).lpNorm<2>() > DELTA)
    {
        MTK_ALERT
        flag = FAIL;
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). IVP::RK::HeunThirdOrder.\n", t);
    }

    timer();
    rk.setInitValue(init);
    rk.setMethod(RK::Method::ClassicalFourthOrder);
    rk.solve(0.5, 0.005);
    if ((rk(0.5) - u(0.5)).lpNorm<2>() > DELTA)
    {
        MTK_ALERT
        flag = FAIL;
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). IVP::RK::ClassicalFourthOrder.\n", t);
    }

    return 0;
}