#ifndef MTK_RANDOM_HPP
#define MTK_RANDOM_HPP

#include "Random.h"

namespace mtk
{
    inline Random::Random()
    {
        Int max_loop_num = MAX<short>;
        Real epsilon = EPS<double>;
        Real step = EPS<float>;
        Real delta = EPS<double>;
        random_engine.seed((Int)std::chrono::system_clock::now().time_since_epoch().count());
    }

    inline void Random::seed(const Int &seed)
    {
        random_engine.seed((Int)seed);
    }

    inline const Int Random::UniformInt(const Int &min, const Int &max)
    {
        return std::uniform_int_distribution<Int>(min, max)(random_engine);
    }

    inline const Real Random::UniformReal(const Real &min, const Real &max)
    {
        return std::uniform_real_distribution<Real>(min, max)(random_engine);
    }

    inline const Real Random::Normal(const Real &expectation, const Real &variance)
    {
        return std::normal_distribution<Real>(expectation, variance)(random_engine);
    }

    inline const bool Random::Bernoulli(const Real &p)
    {
        return std::bernoulli_distribution((double)p)(random_engine);
    }

    inline const Int Random::Binomial(const Int &n, const Real &p)
    {
        return std::binomial_distribution<Int>(n, (double)p)(random_engine);
    }

    inline const Int Random::Geometric(const Real &p)
    {
        return std::geometric_distribution<Int>((double)p)(random_engine);
    }

    inline const Real Random::Exponential(const Real &lambda)
    {
        return std::exponential_distribution<Real>(lambda)(random_engine);
    }

    inline const Int Random::Poisson(const Real &lambda)
    {
        return std::poisson_distribution<Int>((double)lambda)(random_engine);
    }

    template <typename OutputType>
    inline const OutputType Random::DiscreteProbability(const List<Pair<OutputType, Real>> &p)
    {
        for (Int i = 0; i < max_loop_num; i++)
        {
            Int res = UniformInt(Int(0), Int(p.size() - 1));
            if (UniformReal(Real(0.0), Real(1.0)) <= p.at(res).second)
            {
                return p.at(res).first;
            }
        }
        MTK_ERROR
    }

    inline const Real Random::ContinuousProbability(const Real &min, const Real &max, const Func<Real, Real> &p)
    {
        Real s = 0.0;
        Real x = min;
        Real y = UniformReal(Real(0.0), Real(1.0));
        while (x < max)
        {
            s = s + step * (p(x) + p(x + step)) / 2;
            if (s > y)
            {
                return UniformReal(x, x + step);
            }
            x += step;
        }
        MTK_ERROR
    }

    inline const Real Random::ContinuousDistribution(const Real &min, const Real &max, const Func<Real, Real> &f)
    {
        Real l = min;
        Real r = max;
        Real h = (r - l) / 2;
        Real y = UniformReal(Real(0.0), Real(1.0));
        for (Int i = 0; i < max_loop_num; i++)
        {
            Real m = l + h;
            Real u = f(m);
            if ((r - l) <= delta || (std::abs(u - y) <= epsilon))
            {
                return m;
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
        MTK_ERROR
    }
};

#endif
