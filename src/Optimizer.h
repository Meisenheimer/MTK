#ifndef MTK_OPTIMIZER_H
#define MTK_OPTIMIZER_H

#include <functional>

#include "Trait.h"

static_assert(__cplusplus >= 201700, "C++17 or higher is required.");

namespace mtk
{
    using Int = long long int;
    using Real = long double;

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

        std::function<const Real(const Vector<Real> &)> _f;
        std::function<const Vector<Real>(const Vector<Real> &)> _g;
        std::function<const Matrix<Real>(const Vector<Real> &)> _G;

    public:
        const Int &max_loop_num;
        const Real &epsilon;
        const Real &step;
        const Real &delta;
        const Real &trivial_step;
        const LineSearch &line_search;
        const Method &method;

        const std::function<const Real(const Vector<Real> &)> &f;
        const std::function<const Vector<Real>(const Vector<Real> &)> &g;
        const std::function<const Matrix<Real>(const Vector<Real> &)> &G;

    private:
        const std::pair<Real, Real> advanceAndRetreat(const Vector<Real> &x, const Vector<Real> &p) const;
        const Vector<Real> trivial(const Vector<Real> &x, const Vector<Real> &p) const;
        const Vector<Real> goldenSection(const Vector<Real> &x, const Vector<Real> &p) const;
        const Vector<Real> fibonacci(const Vector<Real> &x, const Vector<Real> &p) const;
        const Vector<Real> newton(const Vector<Real> &x, const Vector<Real> &p) const;
        const Vector<Real> bisection(const Vector<Real> &x, const Vector<Real> &p) const;

        const Vector<Real> gradientDescent(const Vector<Real> &x, const LineSearch &line_search) const;
        const Vector<Real> newton(const Vector<Real> &x, const LineSearch &line_search) const;
        const Vector<Real> quasiNewton(const Vector<Real> &x, const LineSearch &line_search) const;

    public:
        Optimizer();

        void setMaxLoopNum(const Int &max_loop_num);
        void setEpsilon(const Real &epsilon);
        void setStep(const Real &step);
        void setDelta(const Real &delta);
        void setTrivialStep(const Real &trivial_step);
        void setFunction(const std::function<const Real(const Vector<Real> &)> &f = nullptr,
                         const std::function<const Vector<Real>(const Vector<Real> &)> &g = nullptr,
                         const std::function<const Matrix<Real>(const Vector<Real> &)> &G = nullptr);
        void setLineSearch(const LineSearch &line_search);
        void setMethod(const Method &method);
        void setMethod(const Method &method, const LineSearch &line_search);

        const Vector<Real> lineSearch(const Vector<Real> &x, const Vector<Real> &p) const;
        const Vector<Real> solve(const Vector<Real> &x);
    };
};

#include "Optimizer.hpp"

#endif