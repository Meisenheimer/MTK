#ifndef MTK_OPTIMIZER_HPP
#define MTK_OPTIMIZER_HPP

#include <iostream>

#include "Optimizer.h"

namespace mtk
{
    template <typename Real>
    inline const std::pair<Real, Real> Optimizer<Real>::advanceAndRetreat(const Vector<Real> &x, const Vector<Real> &p) const
    {
        constexpr Real t = 2.0;
        Real r = 0.0;
        Real l = 0.0;
        Real h = step;
        Real u = Real(f(x + l * p));
        Real v;
        for (size_t n = 0; n < max_loop_num; n++)
        {
            l = r;
            r = r + h;
            v = Real(f(x + r * p));
            if (v < u)
            {
                h = h * t;
                u = v;
            }
            else
            {
                if (n == 0)
                {
                    l = r = 0.0;
                    h = -h;
                }
                else
                {
                    break;
                }
            }
        }
        return std::make_pair(l, r);
    }

    template <typename Real>
    inline const Vector<Real> Optimizer<Real>::trivial(const Vector<Real> &x, const Vector<Real> &p) const
    {
        return (x + trivial_step * p);
    }

    template <typename Real>
    inline const Vector<Real> Optimizer<Real>::goldenSection(const Vector<Real> &x, const Vector<Real> &p) const
    {
        const Vector<Real> d = p.normalized();
        const Real rate = (std::sqrt(5.0) - 1.0) / 2.0;
        std::pair<Real, Real> interval = advanceAndRetreat(x, d);
        size_t k = 0;
        Real l = interval.first;
        Real r = interval.second;
        Real a = r + rate * (l - r);
        Real b = l + rate * (r - l);
        Real fa = Real(f(x + a * d));
        Real fb = Real(f(x + b * d));
        Vector<Real> res;
        for (k = 0; k < max_loop_num; k++)
        {
            if (fa > fb)
            {
                if ((r - a) < delta)
                {
                    res = (x + b * d);
                    break;
                }
                l = a;
                a = b;
                b = l + rate * (r - l);
                fa = fb;
                fb = Real(f(x + b * d));
            }
            else
            {
                if ((b - l) < delta)
                {
                    res = (x + a * d);
                    break;
                }
                r = b;
                b = a;
                a = r + rate * (l - r);
                fb = fa;
                fa = Real(f(x + a * d));
            }
        }
        if (k == max_loop_num)
        {
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        return res;
    }

    template <typename Real>
    inline const Vector<Real> Optimizer<Real>::fibonacci(const Vector<Real> &x, const Vector<Real> &p) const
    {
        const Vector<Real> d = p.normalized();
        Real rate = 0.6;
        std::pair<Real, Real> interval = advanceAndRetreat(x, d);
        size_t k = 0;
        Real l = interval.first;
        Real r = interval.second;
        Real a = r + rate * (l - r);
        Real b = l + rate * (r - l);
        Real fa = Real(f(x + a * d));
        Real fb = Real(f(x + b * d));
        Vector<Real> res;
        for (k = 0; k < max_loop_num; k++)
        {
            if (fa > fb)
            {
                if ((r - a) < delta)
                {
                    res = (x + b * d);
                    break;
                }
                l = a;
                a = b;
                b = l + rate * (r - l);
                fa = fb;
                fb = Real(f(x + b * d));
            }
            else
            {
                if ((b - l) < delta)
                {
                    res = (x + a * d);
                    break;
                }
                r = b;
                b = a;
                a = r + rate * (l - r);
                fb = fa;
                fa = Real(f(x + a * d));
            }
            rate = 1.0 / (1.0 + rate);
        }
        if (k == max_loop_num)
        {
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        return res;
    }

    template <typename Real>
    inline const Vector<Real> Optimizer<Real>::newton(const Vector<Real> &x, const Vector<Real> &p) const
    {
        const Vector<Real> d = p.normalized();
        size_t k = 0;
        Real t = 0.0;
        Vector<Real> res;
        for (k = 0; k < max_loop_num; k++)
        {
            Var<Real> tmp = t;
            auto [g] = derivativesx(f(x + tmp * d), wrt(tmp));
            auto [G] = derivativesx(g, wrt(tmp));
            if (std::abs(Real(g)) < epsilon)
            {
                res = x + t * d;
                break;
            }
            t = t - (Real(g) / Real(G));
        }
        if (k == max_loop_num)
        {
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        return res;
    }

    template <typename Real>
    inline const Vector<Real> Optimizer<Real>::bisection(const Vector<Real> &x, const Vector<Real> &p) const
    {
        const Vector<Real> d = p.normalized();
        std::pair<Real, Real> interval = advanceAndRetreat(x, d);
        size_t k = 0;
        Real l = interval.first;
        Real r = interval.second;
        Real m = (l + r) / 2.0;
        Var<Real> tmp = l;
        Real u = Real(derivatives(f(x + tmp * d), wrt(tmp))[0]);
        tmp = m;
        Real w = Real(derivatives(f(x + tmp * d), wrt(tmp))[0]);
        Vector<Real> res;
        for (k = 0; k < max_loop_num; k++)
        {
            tmp = m;
            w = Real(derivatives(f(x + tmp * d), wrt(tmp))[0]);
            if ((std::abs(r - l) < delta) || (std::abs(Real(w)) < epsilon))
            {
                res = x + m * d;
                break;
            }
            else if ((w > 0.0) != (u > 0.0))
            {
                r = m;
            }
            else
            {
                l = m;
                u = w;
            }
            m = ((l + r) / 2.0);
        }
        if (k == max_loop_num)
        {
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        return res;
    }

    template <typename Real>
    inline const Vector<Real> Optimizer<Real>::lineSearch(const Vector<Real> &x, const Vector<Real> &p) const
    {
        switch (line_search)
        {
        case LineSearch::Trivial:
            return trivial(x, p);
        case LineSearch::GoldenSection:
            return goldenSection(x, p);
        case LineSearch::Fibonacci:
            return fibonacci(x, p);
        case LineSearch::Newton:
            return newton(x, p);
        case LineSearch::Bisection:
            return bisection(x, p);
        default:
            break;
        }
        printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
        exit(0);
    }

    template <typename Real>
    inline const Vector<Real> Optimizer<Real>::gradientDescent(const Vector<Real> &x, const LineSearch &line_search) const
    {
        size_t k;
        Vector<Real> t = x;
        for (k = 0; k < max_loop_num; k++)
        {
            Vector<Var<Real>> tmp = t;
            Vector<Real> grad = Trait<Vector<Var<Real>>>::vector(gradient(f(tmp), tmp));
            if (grad.template lpNorm<2>() <= epsilon)
            {
                break;
            }
            t = lineSearch(t, -grad);
        }
        if (k == max_loop_num)
        {
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        return t;
    }

    template <typename Real>
    inline const Vector<Real> Optimizer<Real>::newton(const Vector<Real> &x, const LineSearch &line_search) const
    {
        Vector<Real> t = x;
        size_t k = 0;
        for (k = 0; k < max_loop_num; k++)
        {
            Vector<Var<Real>> tmp = t;
            Vector<Real> grad = Trait<Vector<Var<Real>>>::vector(gradient(f(tmp), tmp));
            if (grad.template lpNorm<2>() <= epsilon)
            {
                break;
            }
            Matrix<Real> Grad = Trait<Matrix<Var<Real>>>::matrix(hessian(f(tmp), tmp));
            t = lineSearch(t, Grad.fullPivHouseholderQr().solve(-grad));
        }
        if (k == max_loop_num)
        {
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        return t;
    }

    template <typename Real>
    inline const Vector<Real> Optimizer<Real>::quasiNewton(const Vector<Real> &x, const LineSearch &line_search) const
    {
        const size_t n = x.rows();
        size_t k = 0;
        Vector<Real> t = x;
        Vector<Var<Real>> tmp = t;
        Vector<Real> new_grad = Trait<Vector<Var<Real>>>::vector(gradient(f(tmp), tmp));
        Matrix<Real> H(Matrix<Real>::Identity(n, n));
        for (k = 0; k < max_loop_num; k++)
        {
            Vector<Real> grad(new_grad);
            if (grad.template lpNorm<2>() <= epsilon)
            {
                break;
            }
            Vector<Real> d = -H * grad;
            Vector<Real> nt = lineSearch(t, d);
            Vector<Real> s = nt - t;
            Vector<Var<Real>> tmp = nt;
            new_grad = Trait<Vector<Var<Real>>>::vector(gradient(f(tmp), tmp));
            Vector<Real> y = new_grad - grad;
            Matrix<Real> A = Matrix<Real>::Identity(n, n) - (s * y.transpose()) / (s.dot(y));
            H = A * H * A.transpose() + s * s.transpose() / s.dot(y);
            t = nt;
        }
        if (k == max_loop_num)
        {
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        return t;
    }

    template <typename Real>
    inline Optimizer<Real>::Optimizer() : max_loop_num(_max_loop_num), epsilon(_epsilon), step(_step), delta(_delta),
                                          trivial_step(_trivial_step), line_search(_line_search), method(_method),
                                          f(_f)
    {
        this->_max_loop_num = Trait<short>::max();
        this->_epsilon = Trait<float>::epsilon();
        this->_step = std::sqrt(Trait<float>::epsilon() * Trait<double>::epsilon());
        this->_delta = Trait<float>::epsilon();
        this->_trivial_step = 0.1;
        this->_line_search = LineSearch::Fibonacci;
        this->_method = OptimizeMethod::QuasiNewton;
    }

    template <typename Real>
    inline Optimizer<Real>::Optimizer(const Optimizer<Real> &opt) : Optimizer()
    {
        this->_max_loop_num = opt.max_loop_num;
        this->_epsilon = opt.epsilon;
        this->_step = opt.step;
        this->_delta = opt.delta;
        this->_trivial_step = opt.trivial_step;
        this->_line_search = opt.line_search;
        this->_method = opt.method;
        this->_f = opt.f;
        this->_g = opt.g;
        this->_G = opt.G;
    }

    template <typename Real>
    inline void Optimizer<Real>::setMaxLoopNum(const size_t &max_loop_num)
    {
        this->_max_loop_num = max_loop_num;
        return;
    }

    template <typename Real>
    inline void Optimizer<Real>::setEpsilon(const Real &epsilon)
    {
        this->_epsilon = epsilon;
        return;
    }

    template <typename Real>
    inline void Optimizer<Real>::setStep(const Real &step)
    {
        this->_step = step;
        return;
    }

    template <typename Real>
    inline void Optimizer<Real>::setDelta(const Real &delta)
    {
        this->_delta = delta;
        return;
    }

    template <typename Real>
    inline void Optimizer<Real>::setTrivialStep(const Real &trivial_step)
    {
        this->_trivial_step = trivial_step;
        return;
    }

    template <typename Real>
    inline void Optimizer<Real>::setFunction(const std::function<const Var<Real>(const Vector<Var<Real>> &)> &f)
    {
        if (f == nullptr)
        {
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        this->_f = f;
        return;
    }

    template <typename Real>
    inline void Optimizer<Real>::setLineSearch(const LineSearch &line_search)
    {
        this->_line_search = line_search;
        return;
    }

    template <typename Real>
    inline void Optimizer<Real>::setMethod(const OptimizeMethod &method)
    {
        this->_method = method;
        return;
    }

    template <typename Real>
    inline void Optimizer<Real>::setMethod(const OptimizeMethod &method, const LineSearch &line_search)
    {
        this->_line_search = line_search;
        this->_method = method;
        return;
    }

    template <typename Real>
    inline const Vector<Real> Optimizer<Real>::solve(const Vector<Real> &x)
    {
        switch (method)
        {
        case OptimizeMethod::GradientDescent:
            return Trait<Vector<Var<Real>>>::vector(gradientDescent(x, line_search));
        case OptimizeMethod::Newton:
            return Trait<Vector<Var<Real>>>::vector(newton(x, line_search));
        case OptimizeMethod::QuasiNewton:
            return Trait<Vector<Var<Real>>>::vector(quasiNewton(x, line_search));
        default:
            break;
        }
        printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
        exit(0);
    }

    template <typename Real>
    inline Optimizer<Real> &Optimizer<Real>::operator=(const Optimizer<Real> &opt)
    {
        if (this != &opt)
        {
            this->_max_loop_num = opt.max_loop_num;
            this->_epsilon = opt.epsilon;
            this->_step = opt.step;
            this->_delta = opt.delta;
            this->_trivial_step = opt.trivial_step;
            this->_line_search = opt.line_search;
            this->_method = opt.method;
            this->_f = opt.f;
            this->_g = opt.g;
            this->_G = opt.G;
        }
        return (*this);
    }
};

#endif