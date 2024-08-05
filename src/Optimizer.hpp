#ifndef MTK_OPTIMIZER_HPP
#define MTK_OPTIMIZER_HPP

#include "Optimizer.h"

namespace mtk
{
    template <typename Real>
    inline const std::pair<Real, Real> Optimizer<Real>::advanceAndRetreat(const Vector<Real> &x, const Vector<Real> &p) const
    {
        const Real t = 2.0;
        Real r = 0.0;
        Real l = 0.0;
        Real h = step;
        Real u = f(x + l * p);
        Real v;
        for (long int n = 0; n < max_loop_num; n++)
        {
            l = r;
            r = r + h;
            v = f(x + r * p);
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
        return (x + 0.1 * p);
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
        Real fa = f(x + a * d);
        Real fb = f(x + b * d);
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
                fb = f(x + b * d);
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
                fa = f(x + a * d);
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
        Real fa = f(x + a * d);
        Real fb = f(x + b * d);
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
                fb = f(x + b * d);
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
                fa = f(x + a * d);
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
            Vector<Real> grad = g(x + t * d);
            Real gt = grad.dot(d);
            Real Gt = (G(x + t * d) * d).dot(d);
            if (std::abs(gt) < epsilon || grad.template lpNorm<2>() < epsilon)
            {
                res = x + t * d;
                break;
            }
            t = t - (gt / Gt);
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
        Real u = g(x + l * d).dot(d);
        Real w = g(x + m * d).dot(d);
        Vector<Real> res;
        for (k = 0; k < max_loop_num; k++)
        {
            w = f(x + m * d);
            if ((std::abs(r - l) < delta) || (std::abs(w) < epsilon))
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
    inline const Vector<Real> Optimizer<Real>::gradientDescent(const Vector<Real> &x, const LineSearch &line_search) const
    {
        size_t k;
        Vector<Real> t = x;
        for (k = 0; k < max_loop_num; k++)
        {
            Vector<Real> grad(g(t));
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
            Vector<Real> grad(g(t));
            if (grad.template lpNorm<2>() <= epsilon)
            {
                break;
            }
            Matrix<Real> Grad = G(t);
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
        Vector<Real> t(x);
        Vector<Real> new_grad(g(t));
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
            new_grad = g(nt);
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
                                          f(_f), g(_g), G(_G)
    {
        this->_max_loop_num = Trait<short>::max();
        this->_epsilon = Trait<float>::epsilon();
        this->_step = std::sqrt(Trait<float>::epsilon() * Trait<double>::epsilon());
        this->_delta = Trait<float>::epsilon();
        this->_trivial_step = 1.0 / Trait<char>::max();
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
    inline void Optimizer<Real>::setFunction(const std::function<const Real(const Vector<Real> &)> &f,
                                             const std::function<const Vector<Real>(const Vector<Real> &)> &g,
                                             const std::function<const Matrix<Real>(const Vector<Real> &)> &G)
    {
        this->_f = f;
        if (g != nullptr)
        {
            this->_g = g;
        }
        else
        {
            this->_g = [&f = this->f, &step = this->step](const Vector<Real> &x) -> Vector<Real>
            {
                const size_t n = x.rows();
                Vector<Real> g(n);
                Vector<Real> u = x;
                for (size_t i = 0; i < n; i++)
                {
                    u(i) += step;
                    g(i) = f(u);
                    u(i) -= (2.0 * step);
                    g(i) -= f(u);
                    u(i) += step;
                    g(i) /= (2.0 * step);
                }
                return g;
            };
        }
        if (G != nullptr)
        {
            this->_G = G;
        }
        else
        {
            this->_G = [&g = this->g, &step = this->step](const Vector<Real> &x) -> Matrix<Real>
            {
                const size_t n = x.rows();
                Matrix<Real> G(n, n);
                Vector<Real> u = x;
                for (size_t i = 0; i < n; i++)
                {
                    u(i) += step;
                    G.col(i) = g(u);
                    u(i) -= (2.0 * step);
                    G.col(i) -= g(u);
                    u(i) += step;
                    G.col(i) /= (2.0 * step);
                }
                return G;
            };
        }
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
    inline const Vector<Real> Optimizer<Real>::solve(const Vector<Real> &x)
    {
        switch (method)
        {
        case OptimizeMethod::GradientDescent:
            return gradientDescent(x, line_search);
        case OptimizeMethod::Newton:
            return newton(x, line_search);
        case OptimizeMethod::QuasiNewton:
            return quasiNewton(x, line_search);
        default:
            break;
        }
        printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
        exit(0);
    }
};

#endif