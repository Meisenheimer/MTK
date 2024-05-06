#ifndef MTK_NUMBER_H
#define MTK_NUMBER_H

#include "Config.hpp"

namespace mtk
{
    class Prime;

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