#ifndef MTK_SPLINE_HPP
#define MTK_SPLINE_HPP

#include <set>

#include "Spline.h"

namespace mtk
{
    inline Spline fitSpline(const size_t &degree, const ConditionList &c, const bool &periodic)
    {
        std::vector<Real> factor = {1.0, 1.0};
        for (size_t i = 2; i <= degree; i++)
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
        for (size_t i = 1; i < knot.size(); i++)
        {
            poly.push_back(Polynomial(degree));
        }
        const size_t N = c.list.size() + (knot.size() - 2) * degree + (periodic ? degree : 0) + smooth.size();
        Matrix<Real> A = Matrix<Real>::Zero(N, (degree + 1) * poly.size());
        Vector<Real> b = Vector<Real>::Zero(N);
        size_t k = 0, i = 0;
        for (auto it = c.list.begin(); it != c.list.end(); it++, i++, k++)
        {
            const Real x = it->first;
            const std::map<size_t, Real> y = it->second.y;
            const size_t index = s.find(x);
            for (auto jt = y.begin(); jt != y.end(); jt++)
            {
                size_t order = jt->first;
                for (size_t j = order; j <= degree; j++)
                {
                    A(k, index * (degree + 1) + j) = factor[j] * std::pow(x, j - order) / factor[j - order];
                }
                b(k) = jt->second;
            }
        }
        for (size_t i = 1; i < knot.size() - 1; i++)
        {
            Real x = knot[i];
            for (size_t order = 0; order < degree; order++, k++)
            {
                for (size_t j = order; j <= degree; j++)
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
            for (size_t order = 0; order < degree; order++, k++)
            {
                for (size_t j = order; j <= degree; j++)
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
        Vector<Real> x = A.fullPivHouseholderQr().solve(b);
        for (size_t i = 0; i < poly.size(); i++)
        {
            for (size_t j = 0; j <= degree; j++)
            {
                poly[i][j] = x(i * (degree + 1) + j);
            }
        }
        return s;
    }

    inline const size_t Spline::find(const Real &x) const
    {
        const size_t n = poly.size();
        for (size_t i = 0; i < n; i++)
        {
            if ((knot[i]) <= x && x <= knot[i + 1])
            {
                return i;
            }
        }
        printf("Error at: file %s line %d.", __FILE__, __LINE__);
        exit(0);
    }

    inline Spline::Spline(const size_t &degree) : degree(degree), poly(_poly), knot(_knot) {}

    inline Spline::Spline(const Spline &s) : degree(s.degree), poly(_poly), knot(_knot)
    {
        _poly.assign(s.poly.begin(), s.poly.end());
        _knot.assign(s.knot.begin(), s.knot.end());
    }

    inline const Real Spline::operator()(const Real &x) const
    {
        const size_t i = find(x);
        if (i < 0 || i >= poly.size())
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        return poly[i](x);
    }
};

#endif
