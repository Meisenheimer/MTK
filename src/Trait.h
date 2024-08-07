#ifndef MTK_TRAIT_H
#define MTK_TRAIT_H

#include <Eigen/Eigen>

#include <autodiff/reverse/var.hpp>
#include <autodiff/reverse/var/eigen.hpp>

static_assert(__cplusplus >= 201700, "C++17 or higher is required.");

namespace mtk
{
    template <typename Type>
    class Trait;

    template <typename Real>
    using Matrix = Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic>;
    template <typename Real>
    using Vector = Eigen::Vector<Real, Eigen::Dynamic>;
    template <typename Real>
    using Variable = autodiff::Variable<Real>;

    template <typename Type>
    class Trait
    {
    public:
        Trait() = delete;

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
        Trait() = delete;

        static const Matrix<Real> zero(const Matrix<Real> &x);
        static const Matrix<Real> identity(const Matrix<Real> &x);
        static const Matrix<Real> make(const std::vector<std::vector<Real>> &m);
        static const Matrix<Variable<Real>> variable(const Matrix<Real> &x);
    };

    template <typename Real>
    class Trait<Vector<Real>>
    {
    public:
        Trait() = delete;

        static const Vector<Real> zero(const Vector<Real> &x);
        static const Vector<Real> identity(const Vector<Real> &x);
        static const Vector<Real> make(const std::vector<Real> &v);
        static const Vector<Variable<Real>> variable(const Vector<Real> &x);
    };

    template <typename Real>
    class Trait<Vector<Variable<Real>>>
    {
    public:
        Trait() = delete;

        static const Vector<Real> vector(const Vector<Variable<Real>> &x);
    };

    template <typename Real>
    class Trait<Matrix<Variable<Real>>>
    {
    public:
        Trait() = delete;

        static const Matrix<Real> matrix(const Matrix<Variable<Real>> &x);
    };
};

#include "Trait.hpp"

#endif