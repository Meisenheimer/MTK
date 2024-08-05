#ifndef MTK_OPTIMIZER_H
#define MTK_OPTIMIZER_H

#include <functional>

#include "Trait.h"

static_assert(__cplusplus >= 201700, "C++17 or higher is required.");

namespace mtk
{
    template <typename Real>
    class Optimizer;

    enum class LineSearch
    {
        Trivial,
        GoldenSection,
        Fibonacci,
        Newton,
        Bisection,
    };

    enum class OptimizeMethod
    {
        GradientDescent,
        Newton,
        QuasiNewton,
    };

    template <typename Real>
    class Optimizer
    {
    private:
        size_t _max_loop_num;
        Real _epsilon;
        Real _step;
        Real _delta;
        Real _trivial_step;
        LineSearch _line_search;
        OptimizeMethod _method;

        std::function<const Real(const Vector<Real> &)> _f;
        std::function<const Vector<Real>(const Vector<Real> &)> _g;
        std::function<const Matrix<Real>(const Vector<Real> &)> _G;

    public:
        const size_t &max_loop_num;
        const Real &epsilon;
        const Real &step;
        const Real &delta;
        const Real &trivial_step;
        const LineSearch &line_search;
        const OptimizeMethod &method;

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
        Optimizer(const Optimizer &opt);

        void setMaxLoopNum(const size_t &max_loop_num);
        void setEpsilon(const Real &epsilon);
        void setStep(const Real &step);
        void setDelta(const Real &delta);
        void setTrivialStep(const Real &trivial_step);
        void setFunction(const std::function<const Real(const Vector<Real> &)> &f = nullptr,
                         const std::function<const Vector<Real>(const Vector<Real> &)> &g = nullptr,
                         const std::function<const Matrix<Real>(const Vector<Real> &)> &G = nullptr);
        void setLineSearch(const LineSearch &line_search);
        void setMethod(const OptimizeMethod &method);
        void setMethod(const OptimizeMethod &method, const LineSearch &line_search);

        const Vector<Real> lineSearch(const Vector<Real> &x, const Vector<Real> &p) const;
        const Vector<Real> solve(const Vector<Real> &x);
    };
};

#include "Optimizer.hpp"

#endif