#ifndef MTK_IVP_HPP
#define MTK_IVP_HPP

#include "IVP.h"

namespace mtk
{
    inline IVP::IVP() : f(_f), res(_res), opt(_opt) {}

    inline IVP::IVP(const IVP &ivp) : f(_f), res(_res), opt(_opt), _f(ivp.f), _res(ivp.res), _opt(ivp.opt) {}

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
        const size_t n = res.size();
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
        for (size_t i = 0; i < n; i++)
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

    inline IVP &IVP::operator=(const IVP &ivp)
    {
        if (this != &ivp)
        {
            this->_f = ivp.f;
            this->_res = ivp.res;
            this->_opt = ivp.opt;
        }
        return (*this);
    }

    inline LMM::LMM() : IVP(), alpha(_alpha), beta(_beta) {}

    inline LMM::LMM(const LMM &lmm) : IVP(lmm), alpha(_alpha), beta(_beta), _alpha(lmm.alpha), _beta(lmm.beta) {}

    inline void LMM::setMethod(const size_t &name)
    {
        switch (name)
        {
        case ForwardEuler:
            _alpha = {0.0, 1.0};
            _beta = {0.0, 1.0};
            break;
        case BackwardEuler:
            _alpha = {0.0, 1.0};
            _beta = {1.0};
            break;
        case Trapezoidal:
            _alpha = {0.0, 1.0};
            _beta = {0.5, 0.5};
            break;
        case Midpoint:
            _alpha = {0.0, 0.0, 1.0};
            _beta = {0.0, 2.0};
            break;
        default:
            break;
        }
        return;
    }

    inline void LMM::setMethod(const std::vector<Real> &alpha, const std::vector<Real> &beta)
    {
        this->_alpha = alpha;
        this->_beta = beta;
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
        for (size_t i = 1; i < res.size(); i++)
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
                [&res = res, &k, &t, &alpha = _alpha, &beta = _beta, &f = f](const Vector<Real> &u) -> Real
            {
                size_t n = res.size();
                Vector<Real> e = u;
                for (size_t i = 1; i < alpha.size(); i++)
                {
                    e -= (alpha[i] * res[n - i].first);
                }
                e -= (k * beta[0] * f(u, t + k));
                for (size_t i = 1; i < beta.size(); i++)
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

    inline LMM &LMM::operator=(const LMM &lmm)
    {
        if (this != &lmm)
        {
            this->_f = lmm.f;
            this->_res = lmm.res;
            this->_opt = lmm.opt;
            this->_alpha = lmm.alpha;
            this->_beta = lmm.beta;
        }
        return (*this);
    }

    inline RK::RK() : IVP(), a(_a), b(_b), c(_c) {}

    inline RK::RK(const RK &rk) : IVP(rk), a(_a), b(_b), c(_c), _a(rk.a), _b(rk.b), _c(rk.c) {}

    inline void RK::setMethod(const size_t &name)
    {
        switch (name)
        {
        case HeunThirdOrder:
            _a = Trait<Matrix<Real>>::make({{0, 0, 0},
                                            {1.0 / 3.0, 0.0, 0.0},
                                            {0.0, 2.0 / 3.0, 0.0}});
            _b = Trait<Vector<Real>>::make({1.0 / 4.0, 0.0, 3.0 / 4.0});
            _c = Trait<Vector<Real>>::make({0.0, 1.0 / 3.0, 2.0 / 3.0});
            break;
        case ClassicalFourthOrder:
            _a = Trait<Matrix<Real>>::make({{0.0, 0.0, 0.0, 0.0},
                                            {0.5, 0.0, 0.0, 0.0},
                                            {0.0, 0.5, 0.0, 0.0},
                                            {0.0, 0.0, 1.0, 0.0}});
            _b = Trait<Vector<Real>>::make({1.0 / 6.0, 1.0 / 3.0, 1.0 / 3.0, 1.0 / 6.0});
            _c = Trait<Vector<Real>>::make({0.0, 1.0 / 2.0, 1.0 / 2.0, 1.0});
            break;
        default:
            break;
        }
        return;
    }

    inline void RK::setMethod(const Matrix<Real> &a, const Vector<Real> &b, const Vector<Real> &c)
    {
        this->_a = a;
        this->_b = b;
        this->_c = c;
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
        for (size_t i = 1; i < res.size(); i++)
        {
            if ((std::abs(k - (res[i].second - res[i - 1].second) > std::sqrt(Trait<float>::epsilon()))))
            {
                printf("Error at: file %s line %d.", __FILE__, __LINE__);
                exit(0);
            }
        }
        const size_t n = _b.rows();
        const size_t m = res.back().first.rows();
        while (t < end)
        {
            Vector<Real> u = Vector<Real>::Zero((n + 1) * m);
            u.tail(m) = res.back().first;
            std::function<const Real(const Vector<Real> &)> F =
                [&v = res.back().first, &t, &a = _a, &b = _b, &c = _c, &f = f, &k, &n, &m](const Vector<Real> &u) -> Real
            {
                Real error = 0.0;
                Vector<Real> y = u.tail(m);
                y -= v;
                for (size_t i = 0; i < n; i++)
                {
                    y -= (k * b(i) * u.segment(i * m, m));
                }
                error += y.dot(y);
                for (size_t i = 0; i < n; i++)
                {
                    y = v;
                    for (size_t j = 0; j < n; j++)
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

    inline RK &RK::operator=(const RK &rk)
    {
        if (this != &rk)
        {
            this->_f = rk.f;
            this->_res = rk.res;
            this->_opt = rk.opt;
            this->_a = rk.a;
            this->_b = rk.b;
            this->_c = rk.c;
        }
        return (*this);
    }
};

#endif