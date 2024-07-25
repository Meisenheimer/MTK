#ifndef MTK_RANDOM_HPP
#define MTK_RANDOM_HPP

#include <vector>

#include "Random.h"

namespace mtk
{
    inline size_t Random::max_loop_num = std::numeric_limits<short>::max();
    inline std::default_random_engine Random::random_engine((size_t)std::chrono::system_clock::now().time_since_epoch().count());

    inline void Random::seed(const size_t &seed)
    {
        random_engine.seed((size_t)seed);
    }

    template <typename Type>
    inline const Type Random::uniform(const Type &min, const Type &max)
    {
        static_assert(std::is_integral_v<Type> || std::is_floating_point_v<Type>);
        if constexpr (std::is_integral_v<Type>)
        {
            return std::uniform_int_distribution<Type>(min, max)(random_engine);
        }
        if constexpr (std::is_floating_point_v<Type>)
        {
            return std::uniform_real_distribution<Type>(min, max)(random_engine);
        }
    }

    template <typename Real>
    inline const Real Random::normal(const Real &expectation, const Real &variance)
    {
        return std::normal_distribution<Real>(expectation, variance)(random_engine);
    }

    template <typename Real>
    inline const bool Random::bernoulli(const Real &p)
    {
        return std::bernoulli_distribution((double)p)(random_engine);
    }

    template <typename Real>
    inline const size_t Random::binomial(const size_t &n, const Real &p)
    {
        return std::binomial_distribution<size_t>(n, (double)p)(random_engine);
    }

    template <typename Real>
    inline const size_t Random::geometric(const Real &p)
    {
        return std::geometric_distribution<size_t>((double)p)(random_engine);
    }

    template <typename Real>
    inline const Real Random::exponential(const Real &lambda)
    {
        return std::exponential_distribution<Real>(lambda)(random_engine);
    }

    template <typename Real>
    inline const size_t Random::poisson(const Real &lambda)
    {
        return std::poisson_distribution<size_t>((double)lambda)(random_engine);
    }

    template <typename OutputType, typename Real>
    inline const OutputType Random::discreteProbability(const std::vector<std::pair<OutputType, Real>> &p)
    {
        size_t k = 0;
        size_t res = 0;
        if (p.size() == 0)
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        for (k = 0; k < max_loop_num; k++)
        {
            res = Random::uniform<size_t>(size_t(0), size_t(p.size() - 1));
            if (Random::uniform<Real>(Real(0.0), Real(1.0)) <= p.at(res).second)
            {
                break;
            }
        }
        if (k == max_loop_num)
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        return p.at(res).first;
    }

    template <typename Real>
    inline const Real Random::continuousProbability(const Real &min, const Real &max,
                                                    const std::function<const Real(const Real &)> &p,
                                                    const Real &step)
    {
        Real s = 0.0;
        Real x = min;
        Real y = Random::uniform<Real>(Real(0.0), Real(1.0));
        while (x < max)
        {
            s = s + step * (p(x) + p(x + step)) / 2;
            if (s > y)
            {
                break;
            }
            x += step;
        }
        if (x > max)
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        return Random::uniform<Real>(x, x + step);
    }

    template <typename Real>
    inline const Real Random::continuousDistribution(const Real &min, const Real &max,
                                                     const std::function<const Real(const Real &)> &f,
                                                     const Real &epsilon, const Real &delta)
    {
        size_t k = 0;
        Real l = min;
        Real r = max;
        Real h = (r - l) / 2;
        Real y = Random::uniform<Real>(Real(0.0), Real(1.0));
        Real m = l + h;
        for (k = 0; k < max_loop_num; k++)
        {
            m = l + h;
            Real u = f(m);
            if ((r - l) <= delta || (std::abs(u - y) <= epsilon))
            {
                break;
            }
            if (u > y)
            {
                r = m;
            }
            else
            {
                l = m;
            }
            h /= 2;
        }
        if (k == max_loop_num)
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        return m;
    }
};

#endif
