#ifndef MTK_PRIME_H
#define MTK_PRIME_H

#include "Config.hpp"

namespace mtk
{
    class Prime;

    Int gcd(const Int &x, const Int &y);
    Bool isPrime(const Int &x);

    class Prime
    {
    private:
        Int max;
        List<Int> num;

    public:
        Prime(const Int &m);

        Int index(const Int &x) const;
        Bool operator()(const Int &n);
        List<Real> factorization(const Int &x);
    };
};

#include "Number.hpp"

#endif