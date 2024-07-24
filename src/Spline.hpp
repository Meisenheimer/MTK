#ifndef MTK_SPLINE_HPP
#define MTK_SPLINE_HPP

#include "Spline.h"

namespace mtk
{
    inline Spline fitSpline(const Int &degree, const ConditionList &c, const bool &periodic)
    {
        std::vector<Real> factor = {1.0, 1.0};
        for (Int i = 2; i <= degree; i++)
        {
            factor.push_back(factor.back() * i);
        }
        Spline s(degree);
        std::vector<Polynomial> &poly = s._poly;
        std::vector<Real> &knot = s._knot;
        Real M = -INFINITY, m = INFINITY;
        std::set<Real> smooth;
        for (auto it = c.list.begin(); it != c.list.end(); it++)
        {
            Real x = it->first;
            if (it->second.knot)
            {
                knot.push_back(x);
                if (it->second.smooth)
                {
                    smooth.insert(x);
                }
            }
            M = std::max(M, x);
            m = std::min(m, x);
        }
        knot.push_back(M);
        knot.push_back(m);
        std::sort(knot.begin(), knot.end());
        knot.erase(std::unique(knot.begin(), knot.end()), knot.end());
        for (Int i = 1; i < knot.size(); i++)
        {
            poly.push_back(Polynomial(degree));
        }
        const Int N = c.list.size() + (knot.size() - 2) * degree + (periodic ? degree : 0) + smooth.size();
        Matrix A = Matrix::Zero(N, (degree + 1) * poly.size());
        Vector b = Vector::Zero(N);
        Int k = 0, i = 0;
        for (auto it = c.list.begin(); it != c.list.end(); it++, i++, k++)
        {
            const Real x = it->first;
            const std::map<Int, Real> y = it->second.y;
            const Int index = s.find(x);
            for (auto jt = y.begin(); jt != y.end(); jt++)
            {
                Int order = jt->first;
                for (Int j = order; j <= degree; j++)
                {
                    A(k, index * (degree + 1) + j) = factor[j] * std::pow(x, j - order) / factor[j - order];
                }
                b(k) = jt->second;
            }
        }
        for (Int i = 1; i < knot.size() - 1; i++)
        {
            Real x = knot[i];
            for (Int order = 0; order < degree; order++, k++)
            {
                for (Int j = order; j <= degree; j++)
                {
                    A(k, (i - 1) * (degree + 1) + j) = factor[j] * std::pow(x, j - order) / factor[j - order];
                    A(k, i * (degree + 1) + j) = -factor[j] * std::pow(x, j - order) / factor[j - order];
                }
            }
            if (smooth.find(x) != smooth.end())
            {
                A(k, (i - 1) * (degree + 1) + degree) = 1;
                A(k, i * (degree + 1) + degree) = -1;
                k++;
            }
        }
        if (periodic)
        {
            Real x1 = knot.front();
            Real x2 = knot.back();
            for (Int order = 0; order < degree; order++, k++)
            {
                for (Int j = order; j <= degree; j++)
                {
                    A(k, j) = factor[j] * std::pow(x1, j - order) / factor[j - order];
                    A(k, (poly.size() - 1) * (degree + 1) + j) = -factor[j] * std::pow(x2, j - order) / factor[j - order];
                }
            }
            if (smooth.find(x1) != smooth.end() || smooth.find(x2) != smooth.end())
            {
                A(k, degree) = 1;
                A(k, (poly.size() - 1) * (degree + 1) + degree) = -1;
                k++;
            }
        }
        Vector x = A.fullPivHouseholderQr().solve(b);
        for (Int i = 0; i < poly.size(); i++)
        {
            for (Int j = 0; j <= degree; j++)
            {
                poly[i][j] = x(i * (degree + 1) + j);
            }
        }
        return s;
    }

    inline const Int Spline::find(const Real &x) const
    {
        const Int n = poly.size();
        for (Int i = 0; i < n; i++)
        {
            if ((knot[i]) <= x && x <= knot[i + 1])
            {
                return i;
            }
        }
        MTK_ALERT
        return -1;
    }

    inline Spline::Spline(const Int &degree) : degree(degree), poly(_poly), knot(_knot) {}

    inline Spline::Spline(const Spline &s) : degree(s.degree), poly(_poly), knot(_knot)
    {
        _poly.assign(s.poly.begin(), s.poly.end());
        _knot.assign(s.knot.begin(), s.knot.end());
    }

    inline const Real Spline::operator()(const Real &x) const
    {
        const Int i = find(x);
        MTK_ASSERT(i >= 0 && i < poly.size())
        return poly[i](x);
    }
};

#endif
