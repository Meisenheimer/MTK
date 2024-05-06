#ifndef MTK_NEURALNETWORK_H
#define MTK_NEURALNETWORK_H

#include "Tensor.h"

namespace mtk
{
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
        virtual Tensor operator()(const Tensor &x) const = 0;
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
        LeakyReLU(const Real &negative_slope = zero<Real>());

        virtual Tensor operator()(const Tensor &t) const override;
    };

    class Sigmoid : public Activation
    {
    public:
        virtual Tensor operator()(const Tensor &t) const override;
    };

    class Linear : public Layer
    {
    protected:
        bool bias;
        Tensor A;
        Tensor b;

    public:
        Linear(const Int &input, const Int &output, const bool &bias = true);

        void setWeight(const Tensor &A);
        void setBias(const Tensor &b);

        const Tensor getWeight() const;
        const Tensor getBias() const;

        virtual void load(std::istream &stream) override;

        virtual Tensor operator()(const Tensor &t) const override;
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

        Tensor operator()(const Tensor &x) const;
    };
};

#include "NeuralNetwork.hpp"

#endif