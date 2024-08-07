#ifndef MTK_TRAIT_HPP
#define MTK_TRAIT_HPP

#include "Trait.h"

namespace mtk
{
    template <typename Type>
    inline constexpr Type Trait<Type>::zero(const Type &x)
    {
        return Type(0);
    }

    template <typename Type>
    inline constexpr Type Trait<Type>::identity(const Type &x)
    {
        return Type(1);
    }

    template <typename Type>
    inline constexpr Type Trait<Type>::max()
    {
        return std::numeric_limits<Type>::max();
    }

    template <typename Type>
    inline constexpr Type Trait<Type>::min()
    {
        return std::numeric_limits<Type>::min();
    }

    template <typename Type>
    inline constexpr Type Trait<Type>::epsilon()
    {
        return std::numeric_limits<Type>::epsilon();
    }

    template <typename Real>
    inline const Matrix<Real> Trait<Matrix<Real>>::zero(const Matrix<Real> &x)
    {
        return Matrix<Real>::Zero(x.rows(), x.cols());
    }

    template <typename Real>
    inline const Matrix<Real> Trait<Matrix<Real>>::identity(const Matrix<Real> &x)
    {
        return Matrix<Real>::Identity(x.rows(), x.cols());
    }

    template <typename Real>
    inline const Matrix<Real> Trait<Matrix<Real>>::make(const std::vector<std::vector<Real>> &m)
    {
        size_t row = m.size();
        size_t col = 0;
        for (size_t i = 0; i < row; i++)
        {
            col = std::max<size_t>(col, m[i].size());
        }
        Matrix<Real> res = Matrix<Real>::Zero(row, col);
        for (size_t i = 0; i < row; i++)
        {
            col = m[i].size();
            for (size_t j = 0; j < col; j++)
            {
                res(i, j) = m[i][j];
            }
        }
        return res;
    }

    template <typename Real>
    inline const Matrix<Variable<Real>> variable(const Matrix<Real> &x)
    {
        Matrix<Variable<Real>> res(x.rows(), x.cols());
        for (size_t i = 0; i < x.rows(); i++)
        {
            for (size_t j = 0; j < x.cols(); j++)
            {
                res(i, j) = x(i, j);
            }
        }
        return res;
    }

    template <typename Real>
    inline const Vector<Real> Trait<Vector<Real>>::zero(const Vector<Real> &x)
    {
        return Vector<Real>::Zero(x.rows(), x.cols());
    }

    template <typename Real>
    inline const Vector<Real> Trait<Vector<Real>>::identity(const Vector<Real> &x)
    {
        return Vector<Real>::Identity(x.rows(), x.cols());
    }

    template <typename Real>
    inline const Vector<Real> Trait<Vector<Real>>::make(const std::vector<Real> &v)
    {
        size_t n = v.size();
        Vector<Real> res(n);
        for (size_t i = 0; i < n; i++)
        {
            res(i) = v.at(i);
        }
        return res;
    }

    template <typename Real>
    inline const Vector<Variable<Real>> variable(const Vector<Real> &x)
    {
        Vector<Variable<Real>> res(x.rows());
        for (size_t i = 0; i < x.rows(); i++)
        {
            res(i) = x(i);
        }
        return res;
    }

    template <typename Real>
    inline const Vector<Real> vector(const Vector<Variable<Real>> &x)
    {
        Vector<Real> res(x.rows());
        for (size_t i = 0; i < x.rows(); i++)
        {
            res(i) = x(i);
        }
        return res;
    }

    template <typename Real>
    inline const Matrix<Real> matrix(const Matrix<Variable<Real>> &x)
    {
        Matrix<Real> res(x.rows(), x.cols());
        for (size_t i = 0; i < x.rows(); i++)
        {
            for (size_t j = 0; j < x.cols(); j++)
            {
                res(i, j) = x(i, j);
            }
        }
        return res;
    }
};

#endif