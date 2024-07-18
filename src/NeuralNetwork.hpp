#ifndef MTK_NEURALNETWORK_HPP
#define MTK_NEURALNETWORK_HPP

#include <fstream>

#include "NeuralNetwork.h"

namespace mtk
{
    inline std::istream &operator>>(std::istream &stream, Tensor &t)
    {
        for (Int i = 0; i < (Int)t.data.size(); i++)
        {
            stream >> t.data[i];
        }
        return stream;
    }

    inline std::ostream &operator<<(std::ostream &stream, const Tensor &t)
    {
        for (Int i = 0; i < (Int)t.data.size(); i++)
        {
            stream << t.data[i] << " ";
        }
        return stream;
    }

    inline const Tensor operator-(const Tensor &t)
    {
        Tensor res(t);
        for (Int i = 0; i < t.size(); i++)
        {
            res[i] = -res[i];
        }
        return res;
    }

    inline const Tensor operator+(const Tensor &t)
    {
        return t;
    }

    inline const Tensor operator+(const Tensor &t1, const Tensor &t2)
    {
        Tensor res = t1;
        res += t2;
        return res;
    }

    inline const Tensor operator+(const Tensor &t, const Real &k)
    {
        Tensor res = t;
        res += k;
        return res;
    }

    inline const Tensor operator+(const Real &k, const Tensor &t)
    {
        Tensor res = t;
        res += k;
        return res;
    }

    inline const Tensor operator-(const Tensor &t1, const Tensor &t2)
    {
        Tensor res = t1;
        res -= t2;
        return res;
    }

    inline const Tensor operator-(const Tensor &t, const Real &k)
    {
        Tensor res = t;
        res -= k;
        return res;
    }

    inline const Tensor operator*(const Real &k, const Tensor &t)
    {
        Tensor res = t;
        res *= k;
        return res;
    }

    inline const Tensor operator*(const Tensor &t, const Real &k)
    {
        Tensor res = t;
        res *= k;
        return res;
    }

    inline const Tensor operator/(const Tensor &t, const Real &k)
    {
        Tensor res = t;
        res /= k;
        return res;
    }

    inline Tensor::Tensor() : shape(_shape)
    {
    }

    inline Tensor::Tensor(const std::vector<Int> &shape) : Tensor()
    {
        this->_shape = shape;
        data.resize(size(), zero<Real>());
    }

    template <typename... IndexTypes>
    inline Tensor::Tensor(Int firstIndex, IndexTypes... otherIndices) : Tensor(std::vector<Int>({firstIndex, otherIndices...}))
    {
    }

    inline Tensor::Tensor(const Tensor &t) : Tensor(t.shape)
    {
        data = t.data;
    }

    inline Int Tensor::size() const
    {
        Int n = 1;
        for (Int i = 0; i < (Int)shape.size(); i++)
        {
            n *= shape[i];
        }
        return n;
    }

    inline void Tensor::reshape(const std::vector<Int> &shape)
    {
        Int n = 1;
        for (Int i = 0; i < (Int)shape.size(); i++)
        {
            n *= shape[i];
        }
        MTK_ASSERT(n == size())
        this->_shape = shape;
        return;
    }

    template <typename... IndexTypes>
    inline void Tensor::reshape(Int firstIndex, IndexTypes... otherIndices)
    {
        reshape(std::vector<Int>({firstIndex, otherIndices...}));
    }

    inline const Real &Tensor::operator[](const Int &index) const
    {
        MTK_ASSERT(index >= 0 && index < data.size())
        return data[index];
    }

    inline Real &Tensor::operator[](const Int &index)
    {
        MTK_ASSERT(index >= 0 && index < data.size())
        return data[index];
    }

    inline const Real &Tensor::operator()(const std::vector<Int> &index) const
    {
        if (index.size() != shape.size())
        {
            printf("Error At: %s %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        Int n = 0;
        Int m = 1;
        for (Int i = (Int)index.size() - 1; i >= 0; i--)
        {
            n += (m * index[i]);
            m *= shape[i];
        }
        return data.at(n);
    }

    inline Real &Tensor::operator()(const std::vector<Int> &index)
    {
        if (index.size() != shape.size())
        {
            printf("Error At: %s %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        Int n = 0;
        Int m = 1;
        for (Int i = (Int)index.size() - 1; i >= 0; i--)
        {
            n += (m * index[i]);
            m *= shape[i];
        }
        return data.at(n);
    }

    template <typename... IndexTypes>
    inline const Real &Tensor::operator()(Int firstIndex, IndexTypes... otherIndices) const
    {
        if (sizeof...(otherIndices) + 1 != shape.size())
        {
            printf("Error At: %s %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        return this->operator()(std::vector<Int>({firstIndex, otherIndices...}));
    }

    template <typename... IndexTypes>
    inline Real &Tensor::operator()(Int firstIndex, IndexTypes... otherIndices)
    {
        if (sizeof...(otherIndices) + 1 != shape.size())
        {
            printf("Error At: %s %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        return this->operator()(std::vector<Int>({firstIndex, otherIndices...}));
    }

    inline Tensor &Tensor::operator=(const Tensor &t)
    {
        _shape = t.shape;
        data = t.data;
        return (*this);
    }

    inline Tensor &Tensor::operator+=(const Tensor &t)
    {
        MTK_ASSERT(t.data.size() == data.size())
        for (Int i = 0; i < data.size(); i++)
        {
            data[i] += t.data[i];
        }
        return (*this);
    }

    inline Tensor &Tensor::operator+=(const Real &k)
    {
        for (Int i = 0; i < data.size(); i++)
        {
            data[i] += k;
        }
        return (*this);
    }

    inline Tensor &Tensor::operator-=(const Tensor &t)
    {
        MTK_ASSERT(t.data.size() == data.size())
        for (Int i = 0; i < data.size(); i++)
        {
            data[i] -= t.data[i];
        }
        return (*this);
    }

    inline Tensor &Tensor::operator-=(const Real &k)
    {
        for (Int i = 0; i < data.size(); i++)
        {
            data[i] -= k;
        }
        return (*this);
    }

    inline Tensor &Tensor::operator*=(const Real &k)
    {
        for (Int i = 0; i < data.size(); i++)
        {
            data[i] *= k;
        }
        return (*this);
    }

    inline Tensor &Tensor::operator/=(const Real &k)
    {
        for (Int i = 0; i < data.size(); i++)
        {
            data[i] /= k;
        }
        return (*this);
    }

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

    inline Tensor NeuralNetwork::operator()(const Tensor &x) const
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