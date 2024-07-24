#ifndef MTK_CONFIG_HPP
#define MTK_CONFIG_HPP

#include <chrono>

#include "Config.h"

namespace mtk
{
    inline Int NUM_WORKER = []()
    {
        Eigen::initParallel();
        return setEigenNumWorker(MAX_NUM_WORKER);
    }();

    inline Int setEigenNumWorker(const Int &n)
    {
        NUM_WORKER = n;
        Eigen::setNbThreads(NUM_WORKER);
        return n;
    }

    template <typename Type1, typename Type2>
    inline std::pair<Type1, Type2> makePair(const Type1 &first, const Type2 &second)
    {
        return std::make_pair(first, second);
    }

    inline Vector makeVector(const std::vector<Real> &v)
    {
        Int n = v.size();
        Vector res(n);
        for (Int i = 0; i < n; i++)
        {
            res(i) = v[i];
        }
        return res;
    }

    inline Matrix makeMatrix(const std::vector<std::vector<Real>> &m)
    {
        Int row = m.size();
        Int col = m[0].size();
        for (Int i = 0; i < row; i++)
        {
            col = std::max<Int>(col, m[i].size());
        }
        Matrix res = Matrix::Zero(row, col);
        for (Int i = 0; i < row; i++)
        {
            col = m[i].size();
            for (Int j = 0; j < col; j++)
            {
                res(i, j) = m[i][j];
            }
        }
        return res;
    }

    template <typename Type>
    inline const Type zero(const Type &x)
    {
        return Type(0);
    }

    template <>
    inline const Matrix zero(const Matrix &x)
    {
        return Matrix::Zero(x.rows(), x.cols());
    }

    template <>
    inline const Vector zero(const Vector &x)
    {
        return Vector::Zero(x.rows(), x.cols());
    }

    template <typename Type>
    inline const Type identity(const Type &x)
    {
        return Type(1);
    }

    template <>
    inline const Matrix identity(const Matrix &x)
    {
        return Matrix::Identity(x.rows(), x.cols());
    }

    template <>
    inline const Vector identity(const Vector &x)
    {
        return Vector::Identity(x.rows(), x.cols());
    }

    template <typename Type>
    inline const Type pow(const Type &x, const Int &n)
    {
        Int m = n;
        Type a = x;
        Type res = identity<Type>(x);
        while (m)
        {
            if (m & 1)
            {
                res = res * a;
            }
            a *= a;
            m >>= 1;
        }
        return res;
    }

    template <typename Type>
    inline const Type pow(const Type &x, const Int &n, const Type &mod)
    {
        Int m = n;
        Type a = x;
        Type res = identity<Type>(x);
        while (m)
        {
            if (m & 1)
            {
                res = res * a;
                if (mod > 0)
                {
                    res = res % mod;
                }
            }
            a *= a;
            if (mod > 0)
            {
                a = a % mod;
            }
            m >>= 1;
        }
        return res % mod;
    }

    template <typename Type>
    inline const Type gcd(const Type &x, const Type &y)
    {
        if constexpr (std::is_integral_v<Type>)
        {
            MTK_ASSERT(x >= 0 && y >= 0)
        }
        MTK_ASSERT(x != zero<Type>(x) && y != zero<Type>(y))
        Type a = x;
        Type b = y;
        while ((a % b) != 0)
        {
            Type r = a % b;
            a = b;
            b = r;
        }
        return b;
    }
};

#endif