#ifndef MTK_IVP_HPP
#define MTK_IVP_HPP

#include "IVP.h"

namespace mtk
{
    inline IVP::IVP() : f(_f), res(_res)
    {
    }

    inline Optimizer<Real> &IVP::opt()
    {
        return this->_opt;
    }

    inline const Optimizer<Real> &IVP::opt() const
    {
        return this->_opt;
    }

    inline void IVP::setRHS(const std::function<const Vector<Real>(const Vector<Real> &, const Real &)> &f)
    {
        this->_f = f;
        return;
    }

    inline void IVP::setInitValue(const std::vector<std::pair<Vector<Real>, Real>> &init_value)
    {
        this->_res.assign(init_value.begin(), init_value.end());
        return;
    }

    inline const Vector<Real> IVP::operator()(const Real &t) const
    {
        Int n = res.size();
        if (n <= 0)
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        Real begin = res.front().second;
        Real end = res.back().second;
        if (t < begin || t > end)
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        std::pair<Vector<Real>, Real> l = res.front(), r = res.back();
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

    inline void LMM::setMethod(const std::vector<Real> &alpha, const std::vector<Real> &beta)
    {
        this->alpha = alpha;
        this->beta = beta;
        return;
    }

    inline void LMM::solve(const Real &end, const Real &k)
    {
        Real t = res.back().second;
        if (k <= 0.0 || res.size() <= 0)
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        for (Int i = 1; i < (Int)res.size(); i++)
        {
            if (std::abs(k - (res[i].second - res[i - 1].second) > std::sqrt(Trait<float>::epsilon())))
            {
                printf("Error at: file %s line %d.", __FILE__, __LINE__);
                exit(0);
            }
        }
        while (t < end)
        {
            std::function<const Real(const Vector<Real> &)> F =
                [&res = res, &k, &t, &alpha = alpha, &beta = beta, &f = f](const Vector<Real> &u) -> Real
            {
                Int n = res.size();
                Vector<Real> e = u;
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
            _res.push_back(std::make_pair(_opt.solve(res.back().first), t + k));
            t += k;
        }
        return;
    }

    inline void RK::setMethod(const Int &name)
    {
        switch (name)
        {
        case Method::HeunThirdOrder:
            a = Trait<Matrix<Real>>::make({{0, 0, 0},
                                           {1.0 / 3.0, 0.0, 0.0},
                                           {0.0, 2.0 / 3.0, 0.0}});
            b = Trait<Vector<Real>>::make({1.0 / 4.0, 0.0, 3.0 / 4.0});
            c = Trait<Vector<Real>>::make({0.0, 1.0 / 3.0, 2.0 / 3.0});
            break;
        case Method::ClassicalFourthOrder:
            a = Trait<Matrix<Real>>::make({{0.0, 0.0, 0.0, 0.0},
                                           {0.5, 0.0, 0.0, 0.0},
                                           {0.0, 0.5, 0.0, 0.0},
                                           {0.0, 0.0, 1.0, 0.0}});
            b = Trait<Vector<Real>>::make({1.0 / 6.0, 1.0 / 3.0, 1.0 / 3.0, 1.0 / 6.0});
            c = Trait<Vector<Real>>::make({0.0, 1.0 / 2.0, 1.0 / 2.0, 1.0});
            break;
        default:
            break;
        }
        return;
    }

    inline void RK::setMethod(const Matrix<Real> &a, const Vector<Real> &b, const Vector<Real> &c)
    {
        this->a = a;
        this->b = b;
        this->c = c;
        return;
    }

    inline void RK::solve(const Real &end, const Real &k)
    {
        Real t = res.back().second;
        if (k <= 0.0 && res.size() <= 0)
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        for (Int i = 1; i < (Int)res.size(); i++)
        {
            if ((std::abs(k - (res[i].second - res[i - 1].second) > std::sqrt(Trait<float>::epsilon()))))
            {
                printf("Error at: file %s line %d.", __FILE__, __LINE__);
                exit(0);
            }
        }
        Int n = b.rows();
        Int m = res.back().first.rows();
        while (t < end)
        {
            Vector<Real> u = Vector<Real>::Zero((n + 1) * m);
            u.tail(m) = res.back().first;
            std::function<const Real(const Vector<Real> &)> F =
                [&v = res.back().first, &t, &a = a, &b = b, &c = c, &f = f, &k, &n, &m](const Vector<Real> &u) -> Real
            {
                Real error = 0.0;
                Vector<Real> y = u.tail(m);
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
            _res.push_back(std::make_pair(_opt.solve(u).tail(m), t + k));
            t += k;
        }
        return;
    }
};

#endif