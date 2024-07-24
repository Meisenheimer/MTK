#ifndef MTK_INTEGRAL_H
#define MTK_INTEGRAL_H

#include "Config.h"
#include "Polynomial.h"

namespace mtk
{
    inline const std::function<const Real(const Real &)> TRIVIAL_WEIGHT = [](const Real &x) -> Real
    { return identity<Real>(); };

    template <typename ResType>
    const ResType trapezoidal(const Real &min, const Real &max, const std::function<const ResType(const Real &)> &f,
                              const Int &step = MAX<Byte>);
    template <typename ResType>
    const ResType midpoint(const Real &min, const Real &max, const std::function<const ResType(const Real &)> &f,
                           const Int &step = MAX<Byte>);
    template <typename ResType>
    const ResType simpson(const Real &min, const Real &max, const std::function<const ResType(const Real &)> &f,
                          const Int &step = MAX<Byte>);

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
        std::function<const Real(const Real &)> _weight;

    public:
        const Real &min;
        const Real &max;
        const Real &delta;
        const Int &step;
        const Int &num_worker;
        const std::function<const Real(const Real &)> &weight;

    private:
        void check();

    public:
        NewtonCotesIntegrator(const Real &min = -identity<Real>(), const Real &max = identity<Real>());

        void setRange(const Real &min, const Real &max);
        void setDelta(const Real &delta);
        void setStep(const Int &step);
        void setNumWorker(const Int &num_worker);
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
