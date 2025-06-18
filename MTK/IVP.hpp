#ifndef MTK_IVP_HPP
#define MTK_IVP_HPP

#include "IVP.h"

namespace mtk
{
    template <typename Real>
    inline IVP<Real>::IVP() : f(_f), res(_res), opt(_opt) {}

    template <typename Real>
    inline IVP<Real>::IVP(const IVP<Real> &ivp) : f(_f), res(_res), opt(_opt), _f(ivp.f), _res(ivp.res), _opt(ivp.opt) {}

    template <typename Real>
    inline void IVP<Real>::setRHS(const std::function<const Vector<Var<Real>>(const Vector<Var<Real>> &, const Real &)> &f)
    {
        this->_f = f;
        return;
    }

    template <typename Real>
    inline void IVP<Real>::setInitValue(const std::vector<std::pair<Vector<Real>, Real>> &init_value)
    {
        this->_res.assign(init_value.begin(), init_value.end());
        return;
    }

    template <typename Real>
    inline Optimizer<Real> &IVP<Real>::getOpt()
    {
        return _opt;
    }

    template <typename Real>
    inline const Vector<Real> IVP<Real>::operator()(const Real &t) const
    {
        const size_t n = res.size();
        if (n <= 0)
        {
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        Real begin = res.front().second;
        Real end = res.back().second;
        if (t < begin || t > end)
        {
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
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

    template <typename Real>
    inline IVP<Real> &IVP<Real>::operator=(const IVP &ivp)
    {
        if (this != &ivp)
        {
            this->_f = ivp.f;
            this->_res = ivp.res;
            this->_opt = ivp.opt;
        }
        return (*this);
    }

    template <typename Real>
    inline LM<Real>::LM() : IVP<Real>(), alpha(_alpha), beta(_beta) {}

    template <typename Real>
    inline LM<Real>::LM(const LM &lmm) : IVP<Real>(lmm), alpha(_alpha), beta(_beta), _alpha(lmm.alpha), _beta(lmm.beta) {}

    template <typename Real>
    inline void LM<Real>::setMethod(const LMM &name)
    {
        switch (name)
        {
        case LMM::ForwardEuler:
            _alpha = {0.0, 1.0};
            _beta = {0.0, 1.0};
            break;
        case LMM::BackwardEuler:
            _alpha = {0.0, 1.0};
            _beta = {1.0};
            break;
        case LMM::Trapezoidal:
            _alpha = {0.0, 1.0};
            _beta = {0.5, 0.5};
            break;
        case LMM::Midpoint:
            _alpha = {0.0, 0.0, 1.0};
            _beta = {0.0, 2.0};
            break;
        default:
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        return;
    }

    template <typename Real>
    inline void LM<Real>::setMethod(const std::vector<Real> &alpha, const std::vector<Real> &beta)
    {
        this->_alpha = alpha;
        this->_beta = beta;
        return;
    }

    template <typename Real>
    inline bool LM<Real>::isExplicit() const
    {
        return std::abs(beta.front()) < std::numeric_limits<float>::epsilon();
    }

    template <typename Real>
    inline bool LM<Real>::isImplicit() const
    {
        return !isExplicit();
    }

    template <typename Real>
    inline void LM<Real>::solve(const Real &end, const Real &k)
    {
        Real t = this->res.back().second;
        if (k <= 0.0 || this->res.size() <= 0)
        {
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        for (size_t i = 1; i < this->res.size(); i++)
        {
            if (std::abs(k - (this->res[i].second - this->res[i - 1].second) > std::sqrt(Trait<float>::epsilon())))
            {
                printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
                exit(0);
            }
        }
        if (isExplicit())
        {
            while (t < end)
            {
                const size_t n = this->res.size();
                Vector<Real> u = Vector<Real>::Zero(this->res.back().first.rows());
                for (size_t i = 1; i < alpha.size(); i++)
                {
                    u += (alpha[i] * this->res[n - i].first);
                }
                for (size_t i = 1; i < beta.size(); i++)
                {
                    u += (k * beta[i] * Trait<Vector<Var<Real>>>::vector(this->f(this->res[n - i].first, this->res[n - i].second)));
                }
                this->_res.push_back(std::make_pair(u, t + k));
                t += k;
            }
        }
        else
        {
            while (t < end)
            {
                std::function<const Var<Real>(const Vector<Var<Real>> &)> F =
                    [&res = this->res, &k, &t, &alpha = alpha, &beta = beta, &f = this->f](const Vector<Var<Real>> &u) -> Var<Real>
                {
                    const size_t n = res.size();
                    Vector<Var<Real>> e = u;
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
                this->_opt.setFunction(F);
                this->_res.push_back(std::make_pair(this->_opt.solve(this->res.back().first), t + k));
                t += k;
            }
        }
        return;
    }

    template <typename Real>
    inline LM<Real> &LM<Real>::operator=(const LM &lmm)
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

    template <typename Real>
    inline RK<Real>::RK() : IVP<Real>(), a(_a), b(_b), c(_c) {}

    template <typename Real>
    inline RK<Real>::RK(const RK &rk) : IVP<Real>(rk), a(_a), b(_b), c(_c), _a(rk.a), _b(rk.b), _c(rk.c) {}

    template <typename Real>
    inline void RK<Real>::setMethod(const RKM &name)
    {
        switch (name)
        {
        case RKM::HeunThirdOrder:
            _a = Trait<Matrix<Real>>::make({{0, 0, 0},
                                            {1.0 / 3.0, 0.0, 0.0},
                                            {0.0, 2.0 / 3.0, 0.0}});
            _b = Trait<Vector<Real>>::make({1.0 / 4.0, 0.0, 3.0 / 4.0});
            _c = Trait<Vector<Real>>::make({0.0, 1.0 / 3.0, 2.0 / 3.0});
            break;
        case RKM::ClassicalFourthOrder:
            _a = Trait<Matrix<Real>>::make({{0.0, 0.0, 0.0, 0.0},
                                            {0.5, 0.0, 0.0, 0.0},
                                            {0.0, 0.5, 0.0, 0.0},
                                            {0.0, 0.0, 1.0, 0.0}});
            _b = Trait<Vector<Real>>::make({1.0 / 6.0, 1.0 / 3.0, 1.0 / 3.0, 1.0 / 6.0});
            _c = Trait<Vector<Real>>::make({0.0, 1.0 / 2.0, 1.0 / 2.0, 1.0});
            break;
        default:
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        return;
    }

    template <typename Real>
    inline void RK<Real>::setMethod(const Matrix<Real> &a, const Vector<Real> &b, const Vector<Real> &c)
    {
        this->_a = a;
        this->_b = b;
        this->_c = c;
        return;
    }

    template <typename Real>
    inline bool RK<Real>::isExplicit() const
    {
        if (std::abs(c(0)) > std::numeric_limits<float>::epsilon())
        {
            return false;
        }
        const size_t n = _b.rows();
        for (size_t i = 0; i < n; i++)
        {
            for (size_t j = i + 1; j < n; j++)
            {
                if (std::abs(a(i, j)) > std::numeric_limits<float>::epsilon())
                {
                    return false;
                }
            }
        }
        return true;
        // TODO.
    }

    template <typename Real>
    inline bool RK<Real>::isImplicit() const
    {
        return !isExplicit();
    }

    template <typename Real>
    inline void RK<Real>::solve(const Real &end, const Real &k)
    {
        Real t = this->res.back().second;
        if (k <= 0.0 && this->res.size() <= 0)
        {
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        for (size_t i = 1; i < this->res.size(); i++)
        {
            if ((std::abs(k - (this->res[i].second - this->res[i - 1].second) > std::sqrt(Trait<float>::epsilon()))))
            {
                printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
                exit(0);
            }
        }
        const size_t n = _b.rows();
        const size_t m = this->res.back().first.rows();
        if (isExplicit())
        {
            while (t < end)
            {
                Vector<Real> u = Vector<Real>::Zero((n + 1) * m);
                u.tail(m) = this->res.back().first;

                Vector<Real> y;
                for (size_t i = 0; i < n; i++)
                {
                    y = this->res.back().first;
                    for (size_t j = 0; j < i; j++)
                    {
                        y += k * a(i, j) * u.segment(j * m, m);
                    }
                    u.segment(i * m, m) = Trait<Vector<Var<Real>>>::vector(this->f(y, t + c(i) * k));
                }

                y = this->res.back().first;
                for (size_t i = 0; i < n; i++)
                {
                    y += (k * b(i) * u.segment(i * m, m));
                }
                u.tail(m) = y;

                this->_res.push_back(std::make_pair(u.tail(m), t + k));
                t += k;
            }
        }
        else
        {
            while (t < end)
            {
                Vector<Real> u = Vector<Real>::Zero((n + 1) * m);
                u.tail(m) = this->res.back().first;
                std::function<const Var<Real>(const Vector<Var<Real>> &)> F =
                    [&v = this->res.back().first, &t, &a = _a, &b = _b, &c = _c, &f = this->f, &k, &n, &m](const Vector<Var<Real>> &u) -> Var<Real>
                {
                    Var<Real> error = 0.0;
                    Vector<Var<Real>> y = u.tail(m);
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
                this->_opt.setFunction(F);
                this->_res.push_back(std::make_pair(this->_opt.solve(u).tail(m), t + k));
                t += k;
            }
        }
        return;
    }

    template <typename Real>
    inline RK<Real> &RK<Real>::operator=(const RK &rk)
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