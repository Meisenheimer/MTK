#ifndef MTK_OPTIMIZER_HPP
#define MTK_OPTIMIZER_HPP

#include "Optimizer.h"

namespace mtk
{
    inline const std::pair<Real, Real> Optimizer::advanceAndRetreat(const Vector<Real> &x, const Vector<Real> &p) const
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

    inline const Vector<Real> Optimizer::trivial(const Vector<Real> &x, const Vector<Real> &p) const
    {
        return (x + 0.1 * p);
    }

    inline const Vector<Real> Optimizer::goldenSection(const Vector<Real> &x, const Vector<Real> &p) const
    {
        const Vector<Real> d = p.normalized();
        const Real rate = (std::sqrt(5.0) - 1.0) / 2.0;
        std::pair<Real, Real> interval = advanceAndRetreat(x, d);
        Int k = 0;
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
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        return res;
    }

    inline const Vector<Real> Optimizer::fibonacci(const Vector<Real> &x, const Vector<Real> &p) const
    {
        const Vector<Real> d = p.normalized();
        Real rate = 0.6;
        std::pair<Real, Real> interval = advanceAndRetreat(x, d);
        Int k = 0;
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
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        return res;
    }

    inline const Vector<Real> Optimizer::newton(const Vector<Real> &x, const Vector<Real> &p) const
    {
        const Vector<Real> d = p.normalized();
        Int k = 0;
        Real t = 0.0;
        Vector<Real> res;
        for (k = 0; k < max_loop_num; k++)
        {
            Vector<Real> grad = g(x + t * d);
            Real gt = grad.dot(d);
            Real Gt = (G(x + t * d) * d).dot(d);
            if (std::abs(gt) < epsilon || grad.lpNorm<2>() < epsilon)
            {
                res = x + t * d;
                break;
            }
            t = t - (gt / Gt);
        }
        if (k == max_loop_num)
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        return res;
    }

    inline const Vector<Real> Optimizer::bisection(const Vector<Real> &x, const Vector<Real> &p) const
    {
        const Vector<Real> d = p.normalized();
        std::pair<Real, Real> interval = advanceAndRetreat(x, d);
        Int k = 0;
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
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        return res;
    }

    inline const Vector<Real> Optimizer::gradientDescent(const Vector<Real> &x, const LineSearch &line_search) const
    {
        Int k;
        Vector<Real> t = x;
        for (k = 0; k < max_loop_num; k++)
        {
            Vector<Real> grad(g(t));
            if (grad.lpNorm<2>() <= epsilon)
            {
                break;
            }
            t = lineSearch(t, -grad);
        }
        if (k == max_loop_num)
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        return t;
    }

    inline const Vector<Real> Optimizer::newton(const Vector<Real> &x, const LineSearch &line_search) const
    {
        Vector<Real> t = x;
        Int k = 0;
        for (k = 0; k < max_loop_num; k++)
        {
            Vector<Real> grad(g(t));
            if (grad.lpNorm<2>() <= epsilon)
            {
                break;
            }
            Matrix<Real> Grad = G(t);
            t = lineSearch(t, Grad.fullPivHouseholderQr().solve(-grad));
        }
        if (k == max_loop_num)
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        return t;
    }

    inline const Vector<Real> Optimizer::quasiNewton(const Vector<Real> &x, const LineSearch &line_search) const
    {
        const Int n = x.rows();
        Int k = 0;
        Vector<Real> t(x);
        Vector<Real> new_grad(g(t));
        Matrix<Real> H(Matrix<Real>::Identity(n, n));
        for (k = 0; k < max_loop_num; k++)
        {
            Vector<Real> grad(new_grad);
            if (grad.lpNorm<2>() <= epsilon)
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
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        return t;
    }

    inline Optimizer::Optimizer() : max_loop_num(_max_loop_num), epsilon(_epsilon), step(_step), delta(_delta),
                                    trivial_step(_trivial_step), line_search(_line_search), method(_method),
                                    f(_f), g(_g), G(_G)
    {
        this->_max_loop_num = Trait<short>::max();
        this->_epsilon = Trait<float>::epsilon();
        this->_step = std::sqrt(Trait<float>::epsilon() * Trait<double>::epsilon());
        this->_delta = Trait<float>::epsilon();
        this->_trivial_step = 1.0 / Trait<char>::max();
        this->_line_search = LineSearch::Fibonacci;
        this->_method = Method::QuasiNewton;
    }

    inline void Optimizer::setMaxLoopNum(const Int &max_loop_num)
    {
        this->_max_loop_num = max_loop_num;
        return;
    }

    inline void Optimizer::setEpsilon(const Real &epsilon)
    {
        this->_epsilon = epsilon;
        return;
    }

    inline void Optimizer::setStep(const Real &step)
    {
        this->_step = step;
        return;
    }

    inline void Optimizer::setDelta(const Real &delta)
    {
        this->_delta = delta;
        return;
    }

    inline void Optimizer::setTrivialStep(const Real &trivial_step)
    {
        this->_trivial_step = trivial_step;
        return;
    }

    inline void Optimizer::setFunction(const std::function<const Real(const Vector<Real> &)> &f,
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
                const Int n = x.rows();
                Vector<Real> g(n);
                Vector<Real> u = x;
                for (Int i = 0; i < n; i++)
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
                const Int n = x.rows();
                Matrix<Real> G(n, n);
                Vector<Real> u = x;
                for (Int i = 0; i < n; i++)
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

    inline void Optimizer::setLineSearch(const LineSearch &line_search)
    {
        this->_line_search = line_search;
        return;
    }

    inline void Optimizer::setMethod(const Method &method)
    {
        this->_method = method;
        return;
    }

    inline void Optimizer::setMethod(const Method &method, const LineSearch &line_search)
    {
        this->_line_search = line_search;
        this->_method = method;
        return;
    }

    inline const Vector<Real> Optimizer::lineSearch(const Vector<Real> &x, const Vector<Real> &p) const
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
        printf("Error at: file %s line %d.", __FILE__, __LINE__);
        exit(0);
    }

    inline const Vector<Real> Optimizer::solve(const Vector<Real> &x)
    {
        switch (method)
        {
        case Method::GradientDescent:
            return gradientDescent(x, line_search);
        case Method::Newton:
            return newton(x, line_search);
        case Method::QuasiNewton:
            return quasiNewton(x, line_search);
        default:
            break;
        }
        printf("Error at: file %s line %d.", __FILE__, __LINE__);
        exit(0);
    }
};

#endif