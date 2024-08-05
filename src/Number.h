#ifndef MTK_NUMBER_H
#define MTK_NUMBER_H

#include <cstddef>
#include <vector>

static_assert(__cplusplus >= 201700, "C++17 or higher is required.");

namespace mtk
{
    class Prime;

    template <typename Type>
    constexpr Type pow(const Type &x, const size_t &n, const Type &mod);
    template <typename Type>
    constexpr Type gcd(const Type &x, const Type &y);

    bool isPrime(const size_t &x);

    class Prime
    {
    private:
        size_t max;
        std::vector<size_t> num;

    public:
        Prime(const size_t &m);

        size_t index(const size_t &x) const;
        bool operator()(const size_t &n);
        std::vector<size_t> factorization(const size_t &x);
    };
};

#include "Number.hpp"

#endif