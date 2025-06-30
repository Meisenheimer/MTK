#ifndef MTK_NDINTEGRAL_HPP
#define MTK_NDINTEGRAL_HPP

#include "../Trait.h"

#include "NDIntegrator.h"

namespace mtk
{
    template <typename Real, typename Point>
    inline Integrator<Real, Point>::Integrator(const std::vector<Point> &nodes, const std::vector<Real> &weights) : node(_node), weight(_weight)
    {
        this->_node = nodes;
        this->_weight = weights;
    }

    template <typename Real, typename Point>
    inline Integrator<Real, Point>::Integrator(const Integrator &integrator) : node(_node), weight(_weight)
    {
        this->_node = integrator.node;
        this->_weight = integrator.weight;
    }

    template <typename Real, typename Point>
    template <typename ResType, typename Function>
    inline const ResType Integrator<Real, Point>::operator()(const Function &function) const
    {
        ResType res = Trait<ResType>::zero();
        for (size_t i = 0; i < node.size(); i++)
        {
            res += weight[i] * function(node[i]);
        }
        return res;
    }

    template <typename Real, typename Point>
    inline Integrator<Real, Point> &Integrator<Real, Point>::operator=(const Integrator<Real, Point> &integrator)
    {
        if (this != (&Integrator))
        {
            this->_node = integrator.node;
            this->_weight = integrator.weight;
        }
        return (*this);
    }
};

#endif
