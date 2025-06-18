#ifndef MTK_NEURALNETWORK_HPP
#define MTK_NEURALNETWORK_HPP

#include <fstream>

#include "NeuralNetwork.h"

namespace mtk
{
    template <typename Real>
    inline AbstractLayer<Real>::~AbstractLayer() {}

    template <typename Real>
    inline Activation<Real>::~Activation() {}

    template <typename Real>
    inline void Activation<Real>::load(std::istream &stream) { return; }

    template <typename Real>
    inline Layer<Real>::~Layer() {}

    template <typename Real>
    inline LeakyReLU<Real>::LeakyReLU(const Real &negative_slope)
    {
        this->negative_slope = negative_slope;
    }

    template <typename Real>
    inline const Array<Real> LeakyReLU<Real>::operator()(const Array<Real> &t) const
    {
        Array<Real> res(t);
        for (size_t i = 0; i < res.size(); i++)
        {
            res[i] = res[i] >= Trait<Real>::zero() ? res[i] : negative_slope * res[i];
        }
        return res;
    }

    template <typename Real>
    inline const Array<Real> Sigmoid<Real>::operator()(const Array<Real> &t) const
    {
        Array<Real> res(t);
        for (size_t i = 0; i < t.size(); i++)
        {
            res[i] = Trait<Real>::identity() / (Trait<Real>::identity() + std::exp(-res[i]));
        }
        return res;
    }

    template <typename Real>
    inline Linear<Real>::Linear(const size_t &input, const size_t &output, const bool &bias) : A(output, input), b(output)
    {
        this->bias = bias;
    }

    template <typename Real>
    inline void Linear<Real>::setWeight(const Array<Real> &A)
    {
        this->A = A;
        return;
    }

    template <typename Real>
    inline void Linear<Real>::setBias(const Array<Real> &b)
    {
        this->b = b;
        return;
    }

    template <typename Real>
    inline const Array<Real> Linear<Real>::getWeight() const
    {
        return A;
    }

    template <typename Real>
    inline const Array<Real> Linear<Real>::getBias() const
    {
        return b;
    }

    template <typename Real>
    inline void Linear<Real>::load(std::istream &stream)
    {
        stream >> A;
        if (bias)
        {
            stream >> b;
        }
        return;
    }

    template <typename Real>
    inline const Array<Real> Linear<Real>::operator()(const Array<Real> &t) const
    {
        size_t h = A.shape[0];
        size_t w = A.shape[1];
        Array<Real> res(h);
        for (size_t i = 0; i < h; i++)
        {
            res(i) = bias ? b(i) : Trait<Real>::zero();
            for (size_t j = 0; j < w; j++)
            {
                res(i) = res(i) + A(i, j) * t(j);
            }
        }
        return res;
    }

    template <typename Real>
    inline NeuralNetwork<Real>::NeuralNetwork() {}

    template <typename Real>
    inline NeuralNetwork<Real>::~NeuralNetwork()
    {
        for (size_t i = 0; i < layer.size(); i++)
        {
            delete layer[i];
        }
    }

    template <typename Real>
    inline void NeuralNetwork<Real>::load(const std::string &filename)
    {
        std::ifstream fp(filename, std::ios::in);
        if (!fp.is_open())
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        for (size_t i = 0; i < layer.size(); i++)
        {
            layer[i]->load(fp);
        }
        return;
    }

    template <typename Real>
    inline void NeuralNetwork<Real>::push_back(AbstractLayer<Real> *p)
    {
        layer.push_back(p);
        return;
    }

    template <typename Real>
    inline const Array<Real> NeuralNetwork<Real>::operator()(const Array<Real> &x) const
    {
        Array<Real> res = x;
        for (size_t i = 0; i < layer.size(); i++)
        {
            res = layer[i]->operator()(res);
        }
        return res;
    }
};

#endif