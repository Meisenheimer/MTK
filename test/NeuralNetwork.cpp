#include "Timer.h"
#include "../src/NeuralNetwork.h"

using namespace mtk;

constexpr bool PASS = true;
constexpr bool FAIL = !PASS;
constexpr Real DELTA = 1e-4;

int main()
{
    Int t;
    bool flag = PASS;

    NeuralNetwork model;
    model.push_back(new Linear(2, 3));
    model.load("test/model");
    Array<Real> x(2);
    Array<Real> res(3);

    timer();
    x(0) = 1.0;
    x(1) = 0.0;
    res = model(x);
    if (std::abs(res[0] + 0.2949085831642151) >= DELTA ||
        std::abs(res[1] + 0.6427672505378723) >= DELTA ||
        std::abs(res[2] - 0.9013500213623047) >= DELTA)
    {
        printf("Error at: file %s line %d.", __FILE__, __LINE__);
        flag = FAIL;
    }
    x(0) = 0.0;
    x(1) = 1.0;
    res = model(x);
    if (std::abs(res[0] + 0.12623098492622375) >= DELTA ||
        std::abs(res[1] + 0.6869048476219177) >= DELTA ||
        std::abs(res[2] - 0.7919000387191772) >= DELTA)
    {
        printf("Error at: file %s line %d.", __FILE__, __LINE__);
        flag = FAIL;
    }
    t = timer();
    if (flag == PASS)
    {
        printf("PASS Time: %6d(ms). NeuralNetwork::NeuralNetwork.\n", t);
    }

    return 0;
}