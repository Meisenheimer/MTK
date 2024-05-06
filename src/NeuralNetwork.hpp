#ifndef MTK_NEURALNETWORK_HPP
#define MTK_NEURALNETWORK_HPP

#include <fstream>

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

    inline Linear::Linear(const Int &input, const Int &output, const bool &bias) : A(output, input), b(output)
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

    NeuralNetwork::NeuralNetwork() {}

    NeuralNetwork::~NeuralNetwork()
    {
        for (Int i = 0; i < (Int)layer.size(); i++)
        {
            delete layer[i];
        }
    }

    void NeuralNetwork::load(const std::string &filename)
    {
        std::ifstream fp(filename, std::ios::in);
        if (!fp.is_open())
        {
            MTK_ERROR
        }
        for (Int i = 0; i < (Int)layer.size(); i++)
        {
            layer[i]->load(fp);
        }
        return;
    }

    void NeuralNetwork::push_back(AbstractLayer *p)
    {
        layer.push_back(p);
        return;
    }

    Tensor NeuralNetwork::operator()(const Tensor &x) const
    {
        Tensor res = x;
        for (Int i = 0; i < (Int)layer.size(); i++)
        {
            res = layer[i]->operator()(res);
        }
        return res;
    }
};

#endif