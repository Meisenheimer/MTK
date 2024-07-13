#ifndef MW_OPTIMIZER_HPP
#define MW_OPTIMIZER_HPP

#include "Optimizer.h"

namespace mtk
{
    inline const Pair<Real, Real> Optimizer::advanceAndRetreat(const Vector &x, const Vector &p) const
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

    inline const Vector Optimizer::trivial(const Vector &x, const Vector &p) const
    {
        return (x + 0.1 * p);
    }

    inline const Vector Optimizer::goldenSection(const Vector &x, const Vector &p) const
    {
        const Vector d = p.normalized();
        const Real rate = (std::sqrt(5.0) - 1.0) / 2.0;
        Pair<Real, Real> interval = advanceAndRetreat(x, d);
        Real l = interval.first;
        Real r = interval.second;
        Real a = r + rate * (l - r);
        Real b = l + rate * (r - l);
        Real fa = f(x + a * d);
        Real fb = f(x + b * d);
        for (long int k = 0; k < max_loop_num; k++)
        {
            if (fa > fb)
            {
                if ((r - a) < delta)
                {
                    return (x + b * d);
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
                    return (x + a * d);
                }
                r = b;
                b = a;
                a = r + rate * (l - r);
                fb = fa;
                fa = f(x + a * d);
            }
        }
        MTK_ERROR
    }

    inline const Vector Optimizer::fibonacci(const Vector &x, const Vector &p) const
    {
        const Vector d = p.normalized();
        Real rate = 0.6;
        Pair<Real, Real> interval = advanceAndRetreat(x, d);
        Real l = interval.first;
        Real r = interval.second;
        Real a = r + rate * (l - r);
        Real b = l + rate * (r - l);
        Real fa = f(x + a * d);
        Real fb = f(x + b * d);
        for (long int k = 0; k < max_loop_num; k++)
        {
            if (fa > fb)
            {
                if ((r - a) < delta)
                {
                    return (x + b * d);
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
                    return (x + a * d);
                }
                r = b;
                b = a;
                a = r + rate * (l - r);
                fb = fa;
                fa = f(x + a * d);
            }
            rate = 1.0 / (1.0 + rate);
        }
        MTK_ERROR
    }

    inline const Vector Optimizer::newton(const Vector &x, const Vector &p) const
    {
        const Vector d = p.normalized();
        Real t = 0.0;
        for (long int k = 0; k < max_loop_num; k++)
        {
            Vector grad = g(x + t * d);
            Real gt = grad.dot(d);
            Real Gt = (G(x + t * d) * d).dot(d);
            if (std::abs(gt) < epsilon || grad.lpNorm<2>() < epsilon)
            {
                return x + t * d;
            }
            t = t - (gt / Gt);
        }
        MTK_ERROR
    }

    inline const Vector Optimizer::bisection(const Vector &x, const Vector &p) const
    {
        const Vector d = p.normalized();
        Pair<Real, Real> interval = advanceAndRetreat(x, d);
        Real l = interval.first;
        Real r = interval.second;
        Real m = (l + r) / 2.0;
        Real u = g(x + l * d).dot(d);
        Real w = g(x + m * d).dot(d);
        for (long int k = 0; k < max_loop_num; k++)
        {
            w = f(x + m * d);
            if ((std::abs(r - l) < delta) || (std::abs(w) < epsilon))
            {
                return x + m * d;
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
        MTK_ERROR
    }

    inline const Vector Optimizer::gradientDescent(const Vector &x, const LineSearch &line_search) const
    {
        Vector t = x;
        for (Int i = 0; i < max_loop_num; i++)
        {
            Vector grad(g(t));
            if (grad.lpNorm<2>() <= epsilon)
            {
                return t;
            }
            t = lineSearch(t, -grad);
        }
        MTK_ERROR
    }

    inline const Vector Optimizer::newton(const Vector &x, const LineSearch &line_search) const
    {
        Vector t = x;
        for (Int i = 0; i < max_loop_num; i++)
        {
            Vector grad(g(t));
            if (grad.lpNorm<2>() <= epsilon)
            {
                return t;
            }
            Matrix Grad = G(t);
            t = lineSearch(t, Grad.fullPivHouseholderQr().solve(-grad));
        }
        MTK_ERROR
    }

    inline const Vector Optimizer::quasiNewton(const Vector &x, const LineSearch &line_search) const
    {
        const Int n = x.rows();
        Vector t(x);
        Vector new_grad(g(t));
        Matrix H(Matrix::Identity(n, n));
        for (Int i = 0; i < max_loop_num; i++)
        {
            Vector grad(new_grad);
            if (grad.lpNorm<2>() <= epsilon)
            {
                return t;
            }
            Vector d = -H * grad;
            Vector nt = lineSearch(t, d);
            Vector s = nt - t;
            new_grad = g(nt);
            Vector y = new_grad - grad;
            Matrix A = Matrix::Identity(n, n) - (s * y.transpose()) / (s.dot(y));
            H = A * H * A.transpose() + s * s.transpose() / s.dot(y);
            t = nt;
        }
        MTK_ERROR
    }

    inline Optimizer::Optimizer() : max_loop_num(_max_loop_num), epsilon(_epsilon), step(_step), delta(_delta),
                                    trivial_step(_trivial_step), line_search(_line_search), method(_method),
                                    f(_f), g(_g), G(_G)
    {
        this->_max_loop_num = MAX<short>;
        this->_epsilon = EPS<float>;
        this->_step = std::sqrt(EPS<float> * EPS<double>);
        this->_delta = EPS<float>;
        this->_trivial_step = 1.0 / MAX<char>;
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

    inline void Optimizer::setFunction(const Func<Real, Vector> &f,
                                       const Func<Vector, Vector> &g,
                                       const Func<Matrix, Vector> &G)
    {
        this->_f = f;
        if (g != nullptr)
        {
            this->_g = g;
        }
        else
        {
            this->_g = [&f = this->f, &step = this->step](const Vector &x) -> Vector
            {
                const Int n = x.rows();
                Vector g(n);
                Vector u = x;
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
            this->_G = [&g = this->g, &step = this->step](const Vector &x) -> Matrix
            {
                const Int n = x.rows();
                Matrix G(n, n);
                Vector u = x;
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

    inline const Vector Optimizer::lineSearch(const Vector &x, const Vector &p) const
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
        MTK_ERROR
    }

    inline const Vector Optimizer::solve(const Vector &x)
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
        MTK_ERROR
    }
};

#endif