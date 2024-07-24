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
        static constexpr Type zero(const Type &x = 0);
        static constexpr Type identity(const Type &x = 1);
        static constexpr Type basis(const size_t &n = 0);
        static constexpr Type max();
        static constexpr Type min();
        static constexpr Type epsilon();
    };

    template <typename Real>
    class Trait<Matrix<Real>>
    {
    public:
        static const Matrix<Real> zero(const Matrix<Real> &x);
        static const Matrix<Real> identity(const Matrix<Real> &x);
    };

    template <typename Real>
    class Trait<Vector<Real>>
    {
    public:
        static const Vector<Real> zero(const Vector<Real> &x);
        static const Vector<Real> identity(const Vector<Real> &x);
    };
};

#include "Trait.hpp"

#endif