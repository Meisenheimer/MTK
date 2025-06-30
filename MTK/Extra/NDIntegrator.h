#ifndef MTK_NDINTEGRAL_H
#define MTK_NDINTEGRAL_H

#include <functional>

static_assert(__cplusplus >= 201700, "C++17 or higher is required.");

namespace mtk
{
    template <typename Real, typename Point>
    class Integrator;

    template <typename Real, typename Point>
    class Integrator
    {
    private:
        std::vector<Point> _node;
        std::vector<Real> _weight;

    public:
        const std::vector<Point> &node;
        const std::vector<Real> &weight;

    public:
        Integrator(const std::vector<Point> &nodes, const std::vector<Real> &weights);
        Integrator(const Integrator &integrator);

        template <typename ResType, typename Function>
        const ResType operator()(const Function &function) const;

        Integrator &operator=(const Integrator &integrator);
    };
};

#include "NDIntegrator.hpp"

#endif
