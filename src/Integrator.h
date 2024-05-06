#ifndef MTK_INTEGRAL_H
#define MTK_INTEGRAL_H

#include "Config.h"
#include "Polynomial.h"

namespace mtk
{
    inline const Func<Real, Real> TRIVIAL_WEIGHT = [](const Real &x) -> Real
    { return identity<Real>(); };

    class NewtonCotesIntegrator;
    class GaussianIntegrator;

    class NewtonCotesIntegrator
    {
    private:
        Real _min;
        Real _max;
        Real _delta;
        Int _step;
        Int _num_worker;
        Func<Real, Real> _weight;

    public:
        const Real &min;
        const Real &max;
        const Real &delta;
        const Int &step;
        const Int &num_worker;
        const Func<Real, Real> &weight;

    private:
        void check();

    public:
        NewtonCotesIntegrator(const Real &min = -identity<Real>(), const Real &max = identity<Real>());

        void setRange(const Real &min, const Real &max);
        void setDelta(const Real &delta);
        void setStep(const Int &step);
        void setNumWorker(const Int &num_worker);
        void setWeight(const Func<Real, Real> &weight);

        template <typename ResType>
        const ResType Trapezoidal(const Func<ResType, Real> &f) const;
        template <typename ResType>
        const ResType Midpoint(const Func<ResType, Real> &f) const;
        template <typename ResType>
        const ResType Simpson(const Func<ResType, Real> &f) const;
    };

    class GaussianIntegrator
    {
    private:
        Pair<Real, Real> range;
        List<Pair<Real, Real>> coefs;

    public:
        GaussianIntegrator(const OrthogonalPolynomial &op);

        template <typename ResType>
        const ResType operator()(const Func<ResType, Real> &f) const;
    };
};

#include "Integrator.hpp"

#endif
