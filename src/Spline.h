#ifndef MTK_SPLINE_H
#define MTK_SPLINE_H

#include "Config.h"
#include "Polynomial.h"

namespace mtk
{
    class Spline;

    Spline fitSpline(const Int &degree, const ConditionList &c, const Bool &periodic = false);

    class Spline
    {
    private:
        List<Polynomial> _poly;
        List<Real> _knot;

    public:
        const Int degree;
        const List<Polynomial> &poly;
        const List<Real> &knot;

    private:
        const Int find(const Real &x) const;

    public:
        Spline(const Int &degree);
        Spline(const Spline &s);

        const Real operator()(const Real &x) const;

        friend Spline fitSpline(const Int &degree, const ConditionList &c, const Bool &periodic);
    };
};

#include "Spline.hpp"

#endif
