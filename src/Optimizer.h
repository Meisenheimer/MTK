#ifndef MW_OPTIMIZER_H
#define MW_OPTIMIZER_H

#include "Config.hpp"

namespace mtk
{
    class Optimizer;

    class Optimizer
    {
    public:
        enum class LineSearch
        {
            Trivial,
            GoldenSection,
            Fibonacci,
            Newton,
            Bisection,
        };

        enum class Method
        {
            GradientDescent,
            Newton,
            QuasiNewton,
        };

    private:
        Int _max_loop_num;
        Real _epsilon;
        Real _step;
        Real _delta;
        Real _trivial_step;
        LineSearch _line_search;
        Method _method;

        Func<const Real, const Vector &> _f;
        Func<const Vector, const Vector &> _g;
        Func<const Matrix, const Vector &> _G;

    public:
        const Int &max_loop_num;
        const Real &epsilon;
        const Real &step;
        const Real &delta;
        const Real &trivial_step;
        const LineSearch &line_search;
        const Method &method;

        const Func<const Real, const Vector &> &f;
        const Func<const Vector, const Vector &> &g;
        const Func<const Matrix, const Vector &> &G;

    private:
        const std::pair<Real, Real> advanceAndRetreat(const Vector &x, const Vector &p) const;
        const Vector trivial(const Vector &x, const Vector &p) const;
        const Vector goldenSection(const Vector &x, const Vector &p) const;
        const Vector fibonacci(const Vector &x, const Vector &p) const;
        const Vector newton(const Vector &x, const Vector &p) const;
        const Vector bisection(const Vector &x, const Vector &p) const;

        const Vector gradientDescent(const Vector &x, const LineSearch &line_search) const;
        const Vector newton(const Vector &x, const LineSearch &line_search) const;
        const Vector quasiNewton(const Vector &x, const LineSearch &line_search) const;

    public:
        Optimizer();

        void setMaxLoopNum(const Int &max_loop_num);
        void setEpsilon(const Real &epsilon);
        void setStep(const Real &step);
        void setDelta(const Real &delta);
        void setTrivialStep(const Real &trivial_step);
        void setFunction(const Func<const Real, const Vector &> &f = nullptr,
                         const Func<const Vector, const Vector &> &g = nullptr,
                         const Func<const Matrix, const Vector &> &G = nullptr);
        void setLineSearch(const LineSearch &line_search);
        void setMethod(const Method &method);
        void setMethod(const Method &method, const LineSearch &line_search);

        const Vector lineSearch(const Vector &x, const Vector &p) const;
        const Vector solve(const Vector &x);
    };
};

#include "Optimizer.hpp"

#endif