#include "Timer.h"
#include "../src/Integrator.h"
#include "../src/Random.h"

using namespace mtk;

constexpr bool PASS = true;
constexpr bool FAIL = !PASS;
constexpr Real DELTA = 1e-4;

const Real f(const Real &x);
const Real F(const Real &x);

const Real f(const Real &x)
{
    return (pow(x, 3) + 2.0 * pow(x, 2) + exp(x)) + cos(x) + 1 / (x + 10);
}

const Real F(const Real &x)
{
    return pow(x, 4) / 4.0 + 2.0 * pow(x, 3) / 3.0 + exp(x) + sin(x) + log(x + 10);
}

int main()
{
    Random random;
    bool flag = PASS;
    int t;
    List<Real> res;
    NewtonCotesIntegrator nc;
    nc.setRange(-1, 1);
    nc.setStep(2000000);
    nc.setNumWorker(1);

    timer();
    flag = PASS;
    if (std::abs(nc.trapezoidal<Real>(f) - (F(1) - F(-1))) > DELTA)
    {
        MTK_ALERT
        flag = FAIL;
    }
    if (std::abs(nc.midpoint<Real>(f) - (F(1) - F(-1))) > DELTA)
    {
        MTK_ALERT
        flag = FAIL;
    }
    if (std::abs(nc.simpson<Real>(f) - (F(1) - F(-1))) > DELTA)
    {
        MTK_ALERT
        flag = FAIL;
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). Integrator::NewtonCotesIntegrator.\n", t);
    }

    nc.setNumWorker(8);

    timer();
    flag = PASS;
    if (std::abs(nc.trapezoidal<Real>(f) - (F(1) - F(-1))) > DELTA)
    {
        MTK_ALERT
        flag = FAIL;
    }
    if (std::abs(nc.midpoint<Real>(f) - (F(1) - F(-1))) > DELTA)
    {
        MTK_ALERT
        flag = FAIL;
    }
    if (std::abs(nc.simpson<Real>(f) - (F(1) - F(-1))) > DELTA)
    {
        MTK_ALERT
        flag = FAIL;
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). Integrator::NewtonCotesIntegrator.\n", t);
    }

    return 0;
}