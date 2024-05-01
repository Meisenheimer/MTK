#ifndef MTK_RANDOM_H
#define MTK_RANDOM_H

#include <random>
#include <chrono>

#include "Config.hpp"

namespace mtk
{
    class Random;

    class Random
    {
    public:
        Int max_loop_num;
        Real epsilon;
        Real step;
        Real delta;

    private:
        std::default_random_engine random_engine;

    public:
        Random();

    public:
        void seed(const Int &seed);

        const Int UniformInt(const Int &min, const Int &max);
        const Real UniformReal(const Real &min, const Real &max);
        const Real Normal(const Real &expectation, const Real &variance);
        const bool Bernoulli(const Real &p);
        const Int Binomial(const Int &n, const Real &p);
        const Int Geometric(const Real &p);
        const Real Exponential(const Real &lambda);
        const Int Poisson(const Real &lambda);
        template <typename OutputType>
        const OutputType DiscreteProbability(const List<Pair<OutputType, Real>> &p);
        const Real ContinuousProbability(const Real &min, const Real &max, const Func<Real, Real> &p);
        const Real ContinuousDistribution(const Real &min, const Real &max, const Func<Real, Real> &f);
    };
};

#include "Random.hpp"

#endif
