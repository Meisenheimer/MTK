#ifndef MTK_TYPE_H
#define MTK_TYPE_H

#include <Eigen/Eigen>

namespace mtk
{
    template <typename Type>
    class Trait;

    template <typename Real>
    using Matrix = Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic>;
    template <typename Real>
    using Vector = Eigen::Vector<Real, Eigen::Dynamic>;

    template <typename Type>
    class Trait
    {
    public:
        static const Type zero(const Type &x = 0);
        static const Type identity(const Type &x = 1);
        static const Type basis(const size_t &n = 0);
    };

    template <typename Real>
    class Trait<Matrix<Real>>
    {
    public:
        static const Matrix<Real> zero(const Matrix<Real> &x = 0);
        static const Matrix<Real> identity(const Matrix<Real> &x = 1);
        static const Matrix<Real> basis(const size_t &n = 0);
    };

    template <typename Real>
    class Trait<Vector<Real>>
    {
    public:
        static const Vector<Real> zero(const Vector<Real> &x = 0);
        static const Vector<Real> identity(const Vector<Real> &x = 1);
        static const Vector<Real> basis(const size_t &n = 0);
    };
};

#include "Trait.hpp"

#endif