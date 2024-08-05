#ifndef MTK_SPLINE_H
#define MTK_SPLINE_H

#include "Polynomial.h"

static_assert(__cplusplus >= 201700, "C++17 or higher is required.");

namespace mtk
{
    using Real = long double;

    class Spline;

    Spline fitSpline(const size_t &degree, const ConditionList &c, const bool &periodic = false);

    class Spline
    {
    private:
        size_t _degree;
        std::vector<Polynomial> _poly;
        std::vector<Real> _knot;

    public:
        const size_t &degree;
        const std::vector<Polynomial> &poly;
        const std::vector<Real> &knot;

    private:
        const size_t find(const Real &x) const;

    public:
        Spline(const size_t &degree);
        Spline(const Spline &s);

        const Real operator()(const Real &x) const;

        Spline &operator=(const Spline &s);

        friend Spline fitSpline(const size_t &degree, const ConditionList &c, const bool &periodic);
    };
};

#include "Spline.hpp"

#endif
