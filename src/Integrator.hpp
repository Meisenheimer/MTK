#ifndef MTK_INTEGRAL_HPP
#define MTK_INTEGRAL_HPP

#include "Integrator.h"
#include "Polynomial.hpp"

namespace mtk
{
    template <typename Real>
    inline void NewtonCotesIntegrator<Real>::check()
    {
        const Real k = std::abs((max - min) / step);
        if (_delta >= k)
        {
            _delta = k / Trait<char>::max();
        }
        return;
    }

    template <typename Real>
    inline NewtonCotesIntegrator<Real>::NewtonCotesIntegrator(const Real &min, const Real &max)
        : min(_min), max(_max), delta(_delta), step(_step)
    {
        this->_min = min;
        this->_max = max;
        this->_step = Trait<short>::max();
        this->_delta = Trait<float>::epsilon();
        check();
    }

    template <typename Real>
    inline NewtonCotesIntegrator<Real>::NewtonCotesIntegrator(const NewtonCotesIntegrator<Real> &integrator)
        : NewtonCotesIntegrator<Real>(integrator.min, integrator.max)
    {
        this->_step = integrator.step;
        this->_delta = integrator.delta;
        check();
    }

    template <typename Real>
    inline void NewtonCotesIntegrator<Real>::setRange(const Real &min, const Real &max)
    {
        this->_min = min;
        this->_max = max;
        check();
        return;
    }

    template <typename Real>
    inline void NewtonCotesIntegrator<Real>::setDelta(const Real &delta)
    {
        this->_delta = std::abs(delta);
        check();
        return;
    }

    template <typename Real>
    inline void NewtonCotesIntegrator<Real>::setStep(const size_t &step)
    {
        this->_step = step;
        check();
        return;
    }

    template <typename Real>
    template <typename ResType>
    inline const ResType NewtonCotesIntegrator<Real>::trapezoidal(const std::function<const ResType(const Real &)> &f) const
    {
        const Real k = (max - min) / step;
        ResType s = Trait<ResType>::zero();
        Real x = min;
        Real y = min + k;
        ResType fx = f(x);
        ResType fy = f(y);
        while (std::abs(x - max) >= delta)
        {
            s += k * (fx + fy) / 2.0;
            x = y;
            fx = fy;
            y += k;
            fy = f(y);
        }
        return s;
    }

    template <typename Real>
    template <typename ResType>
    inline const ResType NewtonCotesIntegrator<Real>::midpoint(const std::function<const ResType(const Real &)> &f) const
    {
        const Real k = (max - min) / step;
        ResType s = Trait<ResType>::zero();
        Real x = min;
        while (std::abs(x - max) >= delta)
        {
            Real m = x + k / 2.0;
            s += k * f(m);
            x += k;
        }
        return s;
    }

    template <typename Real>
    template <typename ResType>
    inline const ResType NewtonCotesIntegrator<Real>::simpson(const std::function<const ResType(const Real &)> &f) const
    {
        const Real k = (max - min) / step;
        ResType s = Trait<ResType>::zero();
        Real x = min;
        Real m = min + k / 2.0;
        Real y = min + k;
        ResType fx = f(x);
        ResType fy = f(y);
        ResType fm = f(m);
        while (std::abs(x - max) >= delta)
        {
            s += k * (fx + 4.0 * fm + fy) / 6.0;
            x = y;
            y += k;
            m = x + k / 2.0;
            fx = fy;
            fm = f(m);
            fy = f(y);
        }
        return s;
    }

    template <typename Real>
    NewtonCotesIntegrator<Real> &NewtonCotesIntegrator<Real>::operator=(const NewtonCotesIntegrator<Real> &integrator)
    {
        if (this != &integrator)
        {
            this->_min = min;
            this->_max = max;
            this->_step = Trait<short>::max();
            this->_delta = Trait<float>::epsilon();
            check();
        }
        return (*this);
    }

    inline GaussianIntegrator::GaussianIntegrator(const OrthogonalPolynomial &op) : range(_range), coefs(_coefs)
    {
        _range = op.range;
        std::vector<Real> root = op.poly.back().root();
        const size_t n = root.size();
        Matrix<Real> A = Matrix<Real>::Zero(n, n);
        Vector<Real> b = Vector<Real>::Zero(n);
        b(0) = 1.0;
        for (size_t i = 0; i < n; i++)
        {
            for (size_t j = 0; j < n; j++)
            {
                A(i, j) = op.poly[i](root[j]);
            }
        }
        Vector<Real> x = A.fullPivHouseholderQr().solve(b);
        for (size_t i = 0; i < n; i++)
        {
            _coefs.push_back(std::make_pair(x(i), root[i]));
        }
    }

    inline GaussianIntegrator::GaussianIntegrator(const GaussianIntegrator &integrator) : range(_range), coefs(_coefs)
    {
        _range = integrator.range;
        _coefs = integrator.coefs;
    }

    template <typename ResType>
    inline const ResType GaussianIntegrator::operator()(const std::function<const ResType(const Real &)> &f) const
    {
        ResType res = Trait<ResType>::zero();
        for (size_t i = 0; i < _coefs.size(); i++)
        {
            res += (_coefs[i].first * f(_coefs[i].second));
        }
        return res;
    }

    inline GaussianIntegrator &GaussianIntegrator::operator=(const GaussianIntegrator &integrator)
    {
        if (this != &integrator)
        {
            _range = integrator.range;
            _coefs = integrator.coefs;
        }
        return (*this);
    }
};

#endif
