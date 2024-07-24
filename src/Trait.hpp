#ifndef MTK_TYPE_HPP
#define MTK_TYPE_HPP

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
    inline const Vector<Real> Trait<Vector<Real>>::zero(const Vector<Real> &x)
    {
        return Vector<Real>::Zero(x.rows(), x.cols());
    }

    template <typename Real>
    inline const Vector<Real> Trait<Vector<Real>>::identity(const Vector<Real> &x)
    {
        return Vector<Real>::Identity(x.rows(), x.cols());
    }
};

#endif