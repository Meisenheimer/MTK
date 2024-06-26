#ifndef MTK_CONFIG_H
#define MTK_CONFIG_H

#define _USE_MATH_DEFINES

#include <functional>
#include <set>
#include <iostream>
#include <cmath>
#include <map>

#include <Eigen/Eigen>

#define MTK_ALERT printf("Alert at: %s %d\n", __FILE__, __LINE__);
#define MTK_ERROR                                    \
    printf("Error at: %s %d\n", __FILE__, __LINE__); \
    exit(0);
#define MTK_WARNING printf("Warning at: %s %d\n", __FILE__, __LINE__);

namespace mtk
{
    using Bool = bool;
    using Byte = unsigned char;
    using Int = long long int;
    using Real = long double;
    using String = std::string;

    template <typename Type>
    using List = std::vector<Type>;
    template <typename Type>
    using Set = std::set<Type>;
    template <typename Type1, typename Type2>
    using Pair = std::pair<Type1, Type2>;
    template <typename Type1, typename Type2>
    using Map = std::map<Type1, Type2>;

    using Matrix = Eigen::Matrix<Real, Eigen::Dynamic, Eigen::Dynamic>;
    using Vector = Eigen::Vector<Real, Eigen::Dynamic>;

    template <typename ResType, typename... Type>
    using Func = std::function<ResType(Type...)>;

    template <typename Type>
    constexpr Type MAX = std::numeric_limits<Type>::max();
    template <typename Type>
    constexpr Type MIN = std::numeric_limits<Type>::min();
    template <typename Type>
    constexpr Type EPS = std::numeric_limits<Type>::epsilon();

    Int setEigenNumWorker(const Int &n);

    constexpr Int MIN_NUM_WORKER = 1;
    constexpr Int MAX_NUM_WORKER = 12;
    static_assert(MIN_NUM_WORKER >= 1, "MIN_NUM_WORKER should equal or greater than 1.\n");
    static_assert(MAX_NUM_WORKER >= MIN_NUM_WORKER, "MAX_NUM_WORKER should equal or greater than MIN_NUM_WORKER.\n");
    inline Int EIGEN_NUM_WORKER = setEigenNumWorker(MAX_NUM_WORKER);

    inline const Int EIGEN_INIT_PARALLEL = []()
    { Eigen::initParallel(); return 0; }();

    template <typename Type1, typename Type2>
    Pair<Type1, Type2> makePair(const Type1 &first, const Type2 &second);
    Vector makeVector(const List<Real> &v);
    Matrix makeMatrix(const List<List<Real>> &m);

    template <typename Type>
    const Type zero(const Type &x = 0);
    template <typename Type>
    const Type identity(const Type &x = 1);
    template <typename Type>
    const Type basis(const Int &n = 0);

    template <typename Type>
    const Type pow(const Type &x, const Int &n);
    template <typename Type>
    const Type pow(const Type &x, const Int &n, const Type &mod);

    template <typename Type>
    const Type gcd(const Type &x, const Type &y);
};

#include "Config.hpp"

#endif