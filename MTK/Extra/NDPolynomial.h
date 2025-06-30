#ifndef MTK_NDPOLYNOMIAL_H
#define MTK_NDPOLYNOMIAL_H

#include <array>
#include <vector>
#include <map>
#include <string>

#include "../Trait.h"

static_assert(__cplusplus >= 201700, "C++17 or higher is required.");

namespace mtk
{
    template <typename Real, size_t DIM>
    class NDPolynomial;

    template <typename Real, size_t DIM>
    class NDPolynomial
    {
    private:
        struct CmpArray
        {
            static bool operator()(const std::array<size_t, DIM> &x, const std::array<size_t, DIM> &y) const;
        };

    private:
        std::map<std::array<size_t, DIM>, Real, CmpArray> _coef;
        std::vector<size_t> _degree;

    public:
        const std::map<std::array<size_t, DIM>, Real, CmpArray> &coef;
        const std::vector<size_t> &degree;

    public:
        NDPolynomial();
        NDPolynomial(const NDPolynomial &poly);

        const std::string print(const Real &precision = Trait<float>::epsilon()) const;

        Real &operator()(const std::array<Real, DIM> &point);
        template <typename... RealTypes>
        Real &operator()(const Real &first, const RealTypes &...other);

        Real &operator[](const std::array<size_t, DIM> &index);
        template <typename... IndexTypes>
        Real &operator[](const size_t &first, const IndexTypes &...other);
        const Real &operator[](const std::array<size_t, DIM> &index) const;
        template <typename... IndexTypes>
        const Real &operator[](const size_t &first, const IndexTypes &...other) const;
    };
}

#include "NDPolynomial.hpp"

#endif
