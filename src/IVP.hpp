#ifndef MTK_IVP_HPP
#define MTK_IVP_HPP

#include "IVP.h"

#include "Config.hpp"
#include "Optimizer.hpp"

namespace mtk
{
    inline IVP::IVP() : f(_f), res(_res)
    {
    }

    inline Optimizer &IVP::opt()
    {
        return this->_opt;
    }

    inline const Optimizer &IVP::opt() const
    {
        return this->_opt;
    }

    inline void IVP::setRHS(const Func<const Vector, const Vector &, const Real &> &f)
    {
        this->_f = f;
        return;
    }

    inline void IVP::setInitValue(const List<Pair<Vector, Real>> &init_value)
    {
        this->_res.assign(init_value.begin(), init_value.end());
        return;
    }

    inline const Vector IVP::operator()(const Real &t) const
    {
        Int n = res.size();
        MTK_ASSERT(n > 0)
        Real begin = res.front().second;
        Real end = res.back().second;
        MTK_ASSERT(t >= begin && t <= end)
        Pair<Vector, Real> l = res.front(), r = res.back();
        for (Int i = 0; i < n; i++)
        {
            if (res[i].second <= t && res[i + 1].second >= t)
            {
                l = res[i];
                r = res[i + 1];
                break;
            }
        }
        Real k = (r.second - t) / (r.second - l.second);
        return (k * l.first + ((Real)1.0 - k) * r.first);
    }

    inline void LMM::setMethod(const Int &name)
    {
        switch (name)
        {
        case Method::ForwardEuler:
            alpha = {0.0, 1.0};
            beta = {0.0, 1.0};
            break;
        case Method::BackwardEuler:
            alpha = {0.0, 1.0};
            beta = {1.0};
            break;
        case Method::Trapezoidal:
            alpha = {0.0, 1.0};
            beta = {0.5, 0.5};
            break;
        case Method::Midpoint:
            alpha = {0.0, 0.0, 1.0};
            beta = {0.0, 2.0};
            break;
        default:
            break;
        }
        return;
    }

    inline void LMM::setMethod(const List<Real> &alpha, const List<Real> &beta)
    {
        this->alpha = alpha;
        this->beta = beta;
        return;
    }

    inline void LMM::solve(const Real &end, const Real &k)
    {
        Real t = res.back().second;
        MTK_ASSERT(k > 0.0 && res.size() > 0)
        for (Int i = 1; i < (Int)res.size(); i++)
        {
            MTK_ASSERT(std::abs(k - (res[i].second - res[i - 1].second) <= std::sqrt(EPS<float>)))
        }
        while (t < end)
        {
            Func<Real, Vector> F = [&res = res, &k, &t, &alpha = alpha, &beta = beta, &f = f](const Vector &u) -> Real
            {
                Int n = res.size();
                Vector e = u;
                for (Int i = 1; i < (Int)alpha.size(); i++)
                {
                    e -= (alpha[i] * res[n - i].first);
                }
                e -= (k * beta[0] * f(u, t + k));
                for (Int i = 1; i < (Int)beta.size(); i++)
                {
                    e -= (k * beta[i] * f(res[n - i].first, res[n - i].second));
                }
                return e.dot(e);
            };
            _opt.setFunction(F);
            _res.push_back(makePair(_opt.solve(res.back().first), t + k));
            t += k;
        }
        return;
    }

    inline void RK::setMethod(const Int &name)
    {
        switch (name)
        {
        case Method::HeunThirdOrder:
            a = makeMatrix({{0, 0, 0},
                            {1.0 / 3.0, 0.0, 0.0},
                            {0.0, 2.0 / 3.0, 0.0}});
            b = makeVector({1.0 / 4.0, 0.0, 3.0 / 4.0});
            c = makeVector({0.0, 1.0 / 3.0, 2.0 / 3.0});
            break;
        case Method::ClassicalFourthOrder:
            a = makeMatrix({{0.0, 0.0, 0.0, 0.0},
                            {0.5, 0.0, 0.0, 0.0},
                            {0.0, 0.5, 0.0, 0.0},
                            {0.0, 0.0, 1.0, 0.0}});
            b = makeVector({1.0 / 6.0, 1.0 / 3.0, 1.0 / 3.0, 1.0 / 6.0});
            c = makeVector({0.0, 1.0 / 2.0, 1.0 / 2.0, 1.0});
            break;
        default:
            break;
        }
        return;
    }

    inline void RK::setMethod(const Matrix &a, const Vector &b, const Vector &c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
        return;
    }

    inline void RK::solve(const Real &end, const Real &k)
    {
        Real t = res.back().second;
        MTK_ASSERT(k > 0.0 && res.size() > 0)
        for (Int i = 1; i < (Int)res.size(); i++)
        {
            MTK_ASSERT((std::abs(k - (res[i].second - res[i - 1].second) <= std::sqrt(EPS<float>))));
        }
        Int n = b.rows();
        Int m = res.back().first.rows();
        while (t < end)
        {
            Vector u = Vector::Zero((n + 1) * m);
            u.tail(m) = res.back().first;
            Func<Real, Vector> F = [&v = res.back().first, &t, &a = a, &b = b, &c = c, &f = f, &k, &n, &m](const Vector &u) -> Real
            {
                Real error = 0.0;
                Vector y = u.tail(m);
                y -= v;
                for (Int i = 0; i < n; i++)
                {
                    y -= (k * b(i) * u.segment(i * m, m));
                }
                error += y.dot(y);
                for (Int i = 0; i < n; i++)
                {
                    y = v;
                    for (Int j = 0; j < n; j++)
                    {
                        y += k * a(i, j) * u.segment(j * m, m);
                    }
                    y = f(y, t + c(i) * k) - u.segment(i * m, m);
                    error += y.dot(y);
                }
                return error;
            };
            _opt.setFunction(F);
            _res.push_back(makePair(_opt.solve(u).tail(m), t + k));
            t += k;
        }
        return;
    }
};

#endif