#ifndef MTK_NEURALNETWORK_HPP
#define MTK_NEURALNETWORK_HPP

#include "NeuralNetwork.h"

namespace mtk
{
    inline AbstractLayer::~AbstractLayer() {}

    inline Activation::~Activation() {}

    inline void Activation::load(std::istream &stream) { return; }

    inline Layer::~Layer() {}

    inline LeakyReLU::LeakyReLU(const Real &negative_slope)
    {
        this->negative_slope = negative_slope;
    }

    inline Tensor LeakyReLU::operator()(const Tensor &t) const
    {
        Tensor res(t);
        for (Int i = 0; i < res.size(); i++)
        {
            res[i] = res[i] >= zero<Real>() ? res[i] : negative_slope * res[i];
        }
        return res;
    }

    inline Tensor Sigmoid::operator()(const Tensor &t) const
    {
        Tensor res(t);
        for (Int i = 0; i < t.size(); i++)
        {
            res[i] = identity<Real>() / (identity<Real>() + std::exp(-res[i]));
        }
        return res;
    }

    inline Linear::Linear(const Int &input, const Int &output, const bool &bias) : A(output, input), b(output, input)
    {
        this->bias = bias;
    }

    inline void Linear::setWeight(const Tensor &A)
    {
        this->A = A;
        return;
    }

    inline void Linear::setBias(const Tensor &b)
    {
        this->b = b;
        return;
    }

    inline const Tensor Linear::getWeight() const
    {
        return A;
    }

    inline const Tensor Linear::getBias() const
    {
        return b;
    }

    inline void Linear::load(std::istream &stream)
    {
        stream >> A;
        if (bias)
        {
            stream >> b;
        }
        std::cout << A << std::endl;
        std::cout << b << std::endl;
        return;
    }

    inline Tensor Linear::operator()(const Tensor &t) const
    {
        Int h = A.shape[0];
        Int w = A.shape[1];
        Tensor res(h);
        for (Int i = 0; i < h; i++)
        {
            res(i) = bias ? b(i) : zero<Real>();
            for (Int j = 0; j < w; j++)
            {
                res(i) = res(i) + A(i, j) * t(j);
            }
        }
        return res;
    }
};

#endif