#ifndef MTK_INTEGRAL_H
#define MTK_INTEGRAL_H

#include "Trait.h"
#include "Polynomial.h"

static_assert(__cplusplus >= 201700, "C++17 or higher is required.");

namespace mtk
{
    template <typename Real>
    inline const std::function<const Real(const Real &)> TRIVIAL_WEIGHT = [](const Real &x) -> Real
    { return Trait<Real>::identity(); };

    template <typename ResType, typename Real>
    const ResType trapezoidal(const Real &min, const Real &max, const std::function<const ResType(const Real &)> &f,
                              const Int &step = Trait<char>::max());
    template <typename ResType, typename Real>
    const ResType midpoint(const Real &min, const Real &max, const std::function<const ResType(const Real &)> &f,
                           const Int &step = Trait<char>::max());
    template <typename ResType, typename Real>
    const ResType simpson(const Real &min, const Real &max, const std::function<const ResType(const Real &)> &f,
                          const Int &step = Trait<char>::max());

    class NewtonCotesIntegrator;
    class GaussianIntegrator;

    class NewtonCotesIntegrator
    {
    private:
        Real _min;
        Real _max;
        Real _delta;
        Int _step;
        std::function<const Real(const Real &)> _weight;

    public:
        const Real &min;
        const Real &max;
        const Real &delta;
        const Int &step;
        const std::function<const Real(const Real &)> &weight;

    private:
        void check();

    public:
        NewtonCotesIntegrator(const Real &min = -Trait<Real>::identity(), const Real &max = Trait<Real>::identity());

        void setRange(const Real &min, const Real &max);
        void setDelta(const Real &delta);
        void setStep(const Int &step);
        void setWeight(const std::function<const Real(const Real &)> &weight);

        template <typename ResType>
        const ResType trapezoidal(const std::function<const ResType(const Real &)> &f) const;
        template <typename ResType>
        const ResType midpoint(const std::function<const ResType(const Real &)> &f) const;
        template <typename ResType>
        const ResType simpson(const std::function<const ResType(const Real &)> &f) const;
    };

    class GaussianIntegrator
    {
    private:
        std::pair<Real, Real> range;
        std::vector<std::pair<Real, Real>> coefs;

    public:
        GaussianIntegrator(const OrthogonalPolynomial &op);

        template <typename ResType>
        const ResType operator()(const std::function<const ResType(const Real &)> &f) const;
    };
};

#include "Integrator.hpp"

#endif
