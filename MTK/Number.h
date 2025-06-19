#ifndef MTK_NUMBER_H
#define MTK_NUMBER_H

#include <cstddef>
#include <vector>

static_assert(__cplusplus >= 201700, "C++17 or higher is required.");

namespace mtk
{
    class Prime;

    template <typename Type>
    constexpr Type pow(const Type &x, const size_t &n);
    template <typename Type>
    constexpr Type pow(const Type &x, const size_t &n, const Type &mod);
    template <typename Type>
    constexpr Type gcd(const Type &x, const Type &y);

    const bool isPrime(const size_t &x);

    class Prime
    {
    private:
        size_t _max;
        std::vector<size_t> _num;

    public:
        const size_t &max;
        const std::vector<size_t> &num;

    public:
        Prime(const size_t &m);
        Prime(const Prime &p);

        const size_t index(const size_t &x) const;
        const bool operator()(const size_t &n);
        const std::vector<size_t> factorization(const size_t &x);

        Prime &operator=(const Prime &p);
    };
};

#include "Number.hpp"

#endif