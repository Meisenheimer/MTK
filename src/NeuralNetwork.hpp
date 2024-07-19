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

    inline Array<Real> LeakyReLU::operator()(const Array<Real> &t) const
    {
        Array<Real> res(t);
        for (Int i = 0; i < res.size(); i++)
        {
            res[i] = res[i] >= zero<Real>() ? res[i] : negative_slope * res[i];
        }
        return res;
    }

    inline Array<Real> Sigmoid::operator()(const Array<Real> &t) const
    {
        Array<Real> res(t);
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

    inline void Linear::setWeight(const Array<Real> &A)
    {
        this->A = A;
        return;
    }

    inline void Linear::setBias(const Array<Real> &b)
    {
        this->b = b;
        return;
    }

    inline const Array<Real> Linear::getWeight() const
    {
        return A;
    }

    inline const Array<Real> Linear::getBias() const
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

    inline Array<Real> Linear::operator()(const Array<Real> &t) const
    {
        Int h = A.shape[0];
        Int w = A.shape[1];
        Array<Real> res(h);
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

    inline NeuralNetwork::NeuralNetwork() {}

    inline NeuralNetwork::~NeuralNetwork()
    {
        for (Int i = 0; i < (Int)layer.size(); i++)
        {
            delete layer[i];
        }
    }

    inline void NeuralNetwork::load(const std::string &filename)
    {
        std::ifstream fp(filename, std::ios::in);
        MTK_ASSERT(fp.is_open())
        for (Int i = 0; i < (Int)layer.size(); i++)
        {
            layer[i]->load(fp);
        }
        return;
    }

    inline void NeuralNetwork::push_back(AbstractLayer *p)
    {
        layer.push_back(p);
        return;
    }

    inline Array<Real> NeuralNetwork::operator()(const Array<Real> &x) const
    {
        Array<Real> res = x;
        for (Int i = 0; i < (Int)layer.size(); i++)
        {
            res = layer[i]->operator()(res);
        }
        return res;
    }
};

#endif