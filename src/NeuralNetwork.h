#ifndef MTK_NEURALNETWORK_H
#define MTK_NEURALNETWORK_H

#include "Trait.h"
#include "Array.h"

static_assert(__cplusplus >= 201700, "C++17 or higher is required.");

namespace mtk
{
    using Int = long long int;
    using Real = long double;

    class AbstractLayer;
    class Activation;
    class Layer;

    class LeakyReLU;
    class Sigmoid;

    class Linear;

    class NeuralNetwork;

    class AbstractLayer
    {
    public:
        virtual ~AbstractLayer();

        virtual void load(std::istream &stream) = 0;
        virtual const Array<Real> operator()(const Array<Real> &x) const = 0;
    };

    class Activation : public AbstractLayer
    {
    public:
        virtual ~Activation();
        virtual void load(std::istream &stream) final;
    };

    class Layer : public AbstractLayer
    {
    public:
        virtual ~Layer();
    };

    class LeakyReLU : public Activation
    {
    protected:
        Real negative_slope;

    public:
        LeakyReLU(const Real &negative_slope = Trait<Real>::zero());

        virtual const Array<Real> operator()(const Array<Real> &t) const override;
    };

    class Sigmoid : public Activation
    {
    public:
        virtual const Array<Real> operator()(const Array<Real> &t) const override;
    };

    class Linear : public Layer
    {
    protected:
        bool bias;
        Array<Real> A;
        Array<Real> b;

    public:
        Linear(const Int &input, const Int &output, const bool &bias = true);

        void setWeight(const Array<Real> &A);
        void setBias(const Array<Real> &b);

        const Array<Real> getWeight() const;
        const Array<Real> getBias() const;

        virtual void load(std::istream &stream) override;

        virtual const Array<Real> operator()(const Array<Real> &t) const override;
    };

    class NeuralNetwork
    {
    protected:
        std::vector<AbstractLayer *> layer;

    public:
        NeuralNetwork();
        ~NeuralNetwork();

        void load(const std::string &filename);
        void push_back(AbstractLayer *p);

        const Array<Real> operator()(const Array<Real> &x) const;
    };
};

#include "NeuralNetwork.hpp"

#endif