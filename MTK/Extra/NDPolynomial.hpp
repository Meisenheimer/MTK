#ifndef MTK_NDPOLYNOMIAL_HPP
#define MTK_NDPOLYNOMIAL_HPP

#include "NDPolynomial.h"

namespace mtk
{
    template <typename Real, size_t DIM>
    inline bool NDPolynomial<Real, DIM>::CmpArray::operator()(const std::array<size_t, DIM> &x, const std::array<size_t, DIM> &y)
    {
        for (size_t i = 0; i < DIM; i++)
        {
            if (x[i] < y[i])
            {
                return true;
            }
            else if (x[i] > y[i])
            {
                return false;
            }
        }
        return false;
    }

    template <typename Real, size_t DIM>
    inline NDPolynomial<Real, DIM>::NDPolynomial() : coef(_coef), degree(_degree) {}

    template <typename Real, size_t DIM>
    inline NDPolynomial<Real, DIM>::NDPolynomial(const NDPolynomial &poly) : coef(_coef), degree(_degree)
    {
        this->_coef = poly.coef;
        this->_degree = poly.degree;
    }

    template <typename Real, size_t DIM>
    inline const std::string NDPolynomial<Real, DIM>::print(const Real &precision) const
    {
        std::string res = "";
        bool flag = true;
        for (auto it = coef.begin(); it != coef.end(); it++)
        {
            if (std::abs(it->second) > precision)
            {
                const std::array<size_t, DIM> &order = it->first;
                s += ((_coefs[i] > 0 ? (flag ? "" : "+ ") : "- ") + std::to_string(std::abs(_coefs[i])));
                for (size_t i = 0; i < DIM; i++)
                {
                    if (order[i] > 0)
                    {
                        s += (" x" + std::to_string(i) + "^" + std::to_string(order[i]));
                    }
                }
                s += " ";
                flag = false;
            }
        }
        if (flag)
        {
            s = "0";
        }
        return res;
    }

    template <typename Real, size_t DIM>
    inline Real &NDPolynomial<Real, DIM>::operator()(const std::array<Real, DIM> &point)
    {
        Real res = Trait<Real>::zero();
        std::array<std::vector<Real>, DIM> value;
        for (size_t i = 0; i < DIM; i++)
        {
            Real x = point[i];
            size_t n = degree[i];
            value[i].resize(n + 1);
            value[i][0] = Trait<Real>::identity();
            for (size_t j = 1; j <= n; j++)
            {
                value[i][j] = x * value[i][j - 1]
            }
        }
        for (auto it = coef.begin(); it != coef.end(); it++)
        {
            const std::array<size_t, DIM> &order = it->first;
            Real tmp = Trait<Real>::identity();
            for (size_t i = 0; i < DIM; i++)
            {
                tmp *= value[i][order[i]]
            }
            res += it->second * tmp;
        }
        return res;
    }

    template <typename Real, size_t DIM>
    template <typename... RealTypes>
    inline Real &NDPolynomial<Real, DIM>::operator()(const Real &first, const RealTypes &...other)
    {
        return this->operator()(std::array<Real, DIM>({(Real)first, (Real)other...}));
    }

    template <typename Real, size_t DIM>
    inline Real &NDPolynomial<Real, DIM>::operator[](const std::array<size_t, DIM> &index)
    {
        if (!coef.contain(index))
        {
            for (size_t i = 0; i < DIM; i++)
            {
                if (_degree[i] < index[i])
                {
                    _degree[i] = index[i]
                }
            }
            _coef[index] = Trait<Real>::zero();
        }
        return _coef[index];
    }

    template <typename Real, size_t DIM>
    template <typename... IndexTypes>
    inline Real &NDPolynomial<Real, DIM>::operator[](const size_t &first, const IndexTypes &...other)
    {
        return this->operator[](std::array<size_t, DIM>({(size_t)first, (size_t)other...}));
    }

    template <typename Real, size_t DIM>
    inline const Real &NDPolynomial<Real, DIM>::operator[](const std::array<size_t, DIM> &index) const
    {
        if (coef.contain(index))
        {
            return coef[index];
        }
        return Trait<Real>::zero();
    }

    template <typename Real, size_t DIM>
    template <typename... IndexTypes>
    inline const Real &NDPolynomial<Real, DIM>::operator[](const size_t &first, const IndexTypes &...other) const
    {
        return this->operator[](std::array<size_t, DIM>({(size_t)first, (size_t)other...}));
    }
};

#endif
