#ifndef MTK_RANDOM_H
#define MTK_RANDOM_H

#include <chrono>
#include <cstddef>
#include <functional>
#include <random>

static_assert(__cplusplus >= 201700, "C++17 or higher is required.");

namespace mtk
{
    class Random;

    class Random
    {
    public:
        static size_t max_loop_num;
        static std::default_random_engine random_engine;

    public:
        static void seed(const size_t &seed);

        template <typename Type>
        static const Type uniform(const Type &min, const Type &max);
        template <typename Real>
        static const Real normal(const Real &expectation, const Real &variance);
        template <typename Real>
        static const bool bernoulli(const Real &p);
        template <typename Real>
        static const size_t binomial(const size_t &n, const Real &p);
        template <typename Real>
        static const size_t geometric(const Real &p);
        template <typename Real>
        static const Real exponential(const Real &lambda);
        template <typename Real>
        static const size_t poisson(const Real &lambda);
        template <typename OutputType, typename Real>
        static const OutputType discreteProbability(const std::vector<std::pair<OutputType, Real>> &p);
        template <typename Real>
        static const Real continuousProbability(const Real &min, const Real &max,
                                                const std::function<const Real(const Real &)> &p,
                                                const Real &step = std::numeric_limits<float>::epsilon());
        template <typename Real>
        static const Real continuousDistribution(const Real &min, const Real &max,
                                                 const std::function<const Real(const Real &)> &f,
                                                 const Real &epsilon = std::numeric_limits<float>::epsilon(),
                                                 const Real &delta = std::numeric_limits<float>::epsilon());
    };
};

#include "Random.hpp"

#endif
