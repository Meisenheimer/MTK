#ifndef MTK_INTEGRAL_H
#define MTK_INTEGRAL_H

#include "Trait.h"
#include "Polynomial.h"

static_assert(__cplusplus >= 201700, "C++17 or higher is required.");

namespace mtk
{
    template <typename Real>
    class NewtonCotesIntegrator;
    class GaussianIntegrator;

    template <typename Real>
    class NewtonCotesIntegrator
    {
    private:
        Real _min;
        Real _max;
        Real _delta;
        size_t _step;

    public:
        const Real &min;
        const Real &max;
        const Real &delta;
        const size_t &step;

    private:
        void check();

    public:
        NewtonCotesIntegrator(const Real &min = -Trait<Real>::identity(), const Real &max = Trait<Real>::identity());
        NewtonCotesIntegrator(const NewtonCotesIntegrator &integrator);

        void setRange(const Real &min, const Real &max);
        void setDelta(const Real &delta);
        void setStep(const size_t &step);

        template <typename ResType>
        const ResType trapezoidal(const std::function<const ResType(const Real &)> &f) const;
        template <typename ResType>
        const ResType midpoint(const std::function<const ResType(const Real &)> &f) const;
        template <typename ResType>
        const ResType simpson(const std::function<const ResType(const Real &)> &f) const;

        NewtonCotesIntegrator &operator=(const NewtonCotesIntegrator &integrator);
    };

    class GaussianIntegrator
    {
    private:
        std::pair<Real, Real> _range;
        std::vector<std::pair<Real, Real>> _coefs;

    public:
        const std::pair<Real, Real> &range;
        const std::vector<std::pair<Real, Real>> &coefs;

    public:
        GaussianIntegrator(const OrthogonalPolynomial &op);
        GaussianIntegrator(const GaussianIntegrator &integrator);

        template <typename ResType>
        const ResType operator()(const std::function<const ResType(const Real &)> &f) const;

        GaussianIntegrator &operator=(const GaussianIntegrator &integrator);
    };
};

#include "Integrator.hpp"

#endif
