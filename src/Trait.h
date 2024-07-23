#ifndef MTK_TYPE_H
#define MTK_TYPE_H

#include <Eigen/Eigen>

namespace mtk
{
    template <typename Real>
    using Matrix = Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic>;
    template <typename Real>
    using Vector = Eigen::Vector<Real, Eigen::Dynamic>;

    template <typename Type>
    const Type zero(const Type &x = 0);
    template <typename Type>
    const Type identity(const Type &x = 1);
    template <typename Type>
    const Type basis(const size_t &n = 0);
};

#include "Trait.hpp"

#endif