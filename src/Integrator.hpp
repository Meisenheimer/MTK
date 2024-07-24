#ifndef MTK_INTEGRAL_HPP
#define MTK_INTEGRAL_HPP

#include "Integrator.h"

#include "Config.hpp"
#include "Polynomial.hpp"

namespace mtk
{
    template <typename ResType>
    const ResType trapezoidal(const Real &min, const Real &max, const std::function<const ResType(const Real &)> &f, const Int &step)
    {
        const Real k = (max - min) / (Real)step;
        ResType s = zero<ResType>();
        ResType fr = f(min);
        for (Int i = 1; i <= step; i++)
        {
            ResType fl = fr;
            fr = f(min + i * k);
            s += k * (fl + fr) / 2.0;
        }
        return s;
    }

    template <typename ResType>
    const ResType midpoint(const Real &min, const Real &max, const std::function<const ResType(const Real &)> &f, const Int &step)
    {
        const Real k = (max - min) / (Real)step;
        ResType s = zero<ResType>();
        for (Int i = 0; i < step; i++)
        {
            s += k * f(min + i * k + k / 2.0);
        }
        return s;
    }

    template <typename ResType>
    const ResType simpson(const Real &min, const Real &max, const std::function<const ResType(const Real &)> &f, const Int &step)
    {
        const Real k = (max - min) / step;
        ResType s = zero<ResType>();
        ResType fr = f(min);
        for (Int i = 1; i <= step; i++)
        {
            ResType fl = fr;
            ResType fm = f(min + i * k - k / 2.0);
            fr = f(min + i * k);
            s += k * (fl + 4.0 * fm + fr) / 6.0;
        }
        return s;
    }

    inline void NewtonCotesIntegrator::check()
    {
        const Real k = std::abs((max - min) / step);
        if (_delta >= k)
        {
            _delta = k / MAX<char>;
        }
        return;
    }

    inline NewtonCotesIntegrator::NewtonCotesIntegrator(const Real &min, const Real &max)
        : min(_min), max(_max), delta(_delta), step(_step), num_worker(_num_worker), weight(_weight)
    {
        this->_min = min;
        this->_max = max;
        this->_step = MAX<short>;
        this->_delta = EPS<float>;
        this->_weight = TRIVIAL_WEIGHT;
        this->_num_worker = MIN_NUM_WORKER;
        check();
    }

    inline void NewtonCotesIntegrator::setRange(const Real &min, const Real &max)
    {
        this->_min = min;
        this->_max = max;
        check();
        return;
    }

    inline void NewtonCotesIntegrator::setDelta(const Real &delta)
    {
        this->_delta = std::abs(delta);
        check();
        return;
    }

    inline void NewtonCotesIntegrator::setStep(const Int &step)
    {
        this->_step = step;
        check();
        return;
    }

    inline void NewtonCotesIntegrator::setNumWorker(const Int &num_worker)
    {
        this->_num_worker = num_worker;
        if (this->_num_worker < MIN_NUM_WORKER)
        {
            this->_num_worker = MIN_NUM_WORKER;
        }
        if (this->_num_worker > MAX_NUM_WORKER)
        {
            this->_num_worker = MAX_NUM_WORKER;
        }
        return;
    }

    inline void NewtonCotesIntegrator::setWeight(const std::function<const Real(const Real &)> &weight)
    {
        if (weight == nullptr)
        {
            this->_weight = TRIVIAL_WEIGHT;
        }
        else
        {
            this->_weight = weight;
        }
        return;
    }

    template <typename ResType>
    inline const ResType NewtonCotesIntegrator::trapezoidal(const std::function<const ResType(const Real &)> &f) const
    {
        const Real k = (max - min) / step;
        const Real split = (max - min) / num_worker;
        ResType res[num_worker];
        ResType s = zero<ResType>();
#pragma omp parallel for
        for (Int i = 0; i < num_worker; i++)
        {
            Real begin = min + i * split;
            Real end = begin + split;
            res[i] = s;
            Real x = begin;
            Real y = begin + k;
            ResType fx = weight(x) * f(x);
            ResType fy = weight(y) * f(y);
            while (std::abs(x - end) >= delta)
            {
                res[i] += k * (fx + fy) / 2.0;
                x = y;
                fx = fy;
                y += k;
                fy = weight(y) * f(y);
            }
        }
        for (Int i = 0; i < num_worker; i++)
        {
            s += res[i];
        }
        return s;
    }

    template <typename ResType>
    inline const ResType NewtonCotesIntegrator::midpoint(const std::function<const ResType(const Real &)> &f) const
    {
        const Real k = (max - min) / step;
        const Real split = (max - min) / num_worker;
        ResType res[num_worker];
        ResType s = zero<ResType>();
#pragma omp parallel for
        for (Int i = 0; i < num_worker; i++)
        {
            Real begin = min + i * split;
            Real end = begin + split;
            res[i] = s;
            Real x = begin;
            while (std::abs(x - end) >= delta)
            {
                Real m = x + k / 2.0;
                res[i] += k * weight(m) * f(m);
                x += k;
            }
        }
        for (Int i = 0; i < num_worker; i++)
        {
            s += res[i];
        }
        return s;
    }

    template <typename ResType>
    inline const ResType NewtonCotesIntegrator::simpson(const std::function<const ResType(const Real &)> &f) const
    {
        const Real k = (max - min) / step;
        const Real split = (max - min) / num_worker;
        ResType res[num_worker];
        ResType s = zero<ResType>();
#pragma omp parallel for
        for (Int i = 0; i < num_worker; i++)
        {
            Real begin = min + i * split;
            Real end = min + (i + 1) * split;
            res[i] = s;
            Real x = begin;
            Real m = x + k / 2.0;
            Real y = x + k;
            ResType fx = weight(x) * f(x);
            ResType fy = weight(y) * f(y);
            ResType fm = weight(m) * f(m);
            while (std::abs(x - end) >= delta)
            {
                res[i] += k * (fx + 4.0 * fm + fy) / 6.0;
                x = y;
                y += k;
                m = x + k / 2.0;
                fx = fy;
                fm = weight(m) * f(m);
                fy = weight(y) * f(y);
            }
        }
        for (Int i = 0; i < num_worker; i++)
        {
            s += res[i];
        }
        return s;
    }

    inline GaussianIntegrator::GaussianIntegrator(const OrthogonalPolynomial &op)
    {
        range = op.range;
        std::vector<Real> root = op.poly.back().root();
        const Int n = root.size();
        Matrix A = Matrix::Zero(n, n);
        Vector b = Vector::Zero(n);
        b(0) = 1.0;
        for (Int i = 0; i < n; i++)
        {
            for (Int j = 0; j < n; j++)
            {
                A(i, j) = op.poly[i](root[j]);
            }
        }
        Vector x = A.fullPivHouseholderQr().solve(b);
        for (Int i = 0; i < n; i++)
        {
            coefs.push_back(makePair(x(i), root[i]));
        }
    }

    template <typename ResType>
    inline const ResType GaussianIntegrator::operator()(const std::function<const ResType(const Real &)> &f) const
    {
        ResType res = zero<ResType>();
        for (Int i = 0; i < (Int)coefs.size(); i++)
        {
            res += (coefs[i].first * f(coefs[i].second));
        }
        return res;
    }
};

#endif
