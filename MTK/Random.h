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
    private:
        std::default_random_engine _random_engine;

    public:
        const std::default_random_engine &random_engine;

    public:
        Random();
        Random(const size_t &seed);
        Random(const Random &random);

        void seed(const size_t &seed);

        template <typename Type>
        const Type uniform(const Type &min, const Type &max);
        template <typename Real>
        const Real normal(const Real &expectation, const Real &variance);
        template <typename Real>
        const bool bernoulli(const Real &p);
        template <typename Real>
        const size_t binomial(const size_t &n, const Real &p);
        template <typename Real>
        const size_t geometric(const Real &p);
        template <typename Real>
        const Real exponential(const Real &lambda);
        template <typename Real>
        const size_t poisson(const Real &lambda);
        template <typename OutputType, typename Real>
        const OutputType discreteProbability(const std::vector<std::pair<OutputType, Real>> &p,
                                             const size_t &max_loop_num = std::numeric_limits<short>::max());
        template <typename Real>
        const Real continuousProbability(const Real &min, const Real &max,
                                         const std::function<const Real(const Real &)> &p,
                                         const Real &step = std::numeric_limits<float>::epsilon(),
                                         const size_t &max_loop_num = std::numeric_limits<short>::max());
        template <typename Real>
        const Real continuousDistribution(const Real &min, const Real &max,
                                          const std::function<const Real(const Real &)> &f,
                                          const Real &epsilon = std::numeric_limits<float>::epsilon(),
                                          const Real &delta = std::numeric_limits<float>::epsilon(),
                                          const size_t &max_loop_num = std::numeric_limits<short>::max());

        Random &operator=(const Random &random);
    };
};

#include "Random.hpp"

#endif
