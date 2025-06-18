#ifndef MTK_NEURALNETWORK_H
#define MTK_NEURALNETWORK_H

#include "Trait.h"
#include "Array.h"

static_assert(__cplusplus >= 201700, "C++17 or higher is required.");

namespace mtk
{
    template <typename Real>
    class AbstractLayer;
    template <typename Real>
    class Activation;
    template <typename Real>
    class Layer;

    template <typename Real>
    class LeakyReLU;
    template <typename Real>
    class Sigmoid;

    template <typename Real>
    class Linear;

    template <typename Real>
    class NeuralNetwork;

    template <typename Real>
    class AbstractLayer
    {
    public:
        virtual ~AbstractLayer();

        virtual void load(std::istream &stream) = 0;
        virtual const Array<Real> operator()(const Array<Real> &x) const = 0;
    };

    template <typename Real>
    class Activation : public AbstractLayer<Real>
    {
    public:
        virtual ~Activation();
        virtual void load(std::istream &stream) final;
    };

    template <typename Real>
    class Layer : public AbstractLayer<Real>
    {
    public:
        virtual ~Layer();
    };

    template <typename Real>
    class LeakyReLU : public Activation<Real>
    {
    protected:
        Real negative_slope;

    public:
        LeakyReLU(const Real &negative_slope = Trait<Real>::zero());

        virtual const Array<Real> operator()(const Array<Real> &t) const override;
    };

    template <typename Real>
    class Sigmoid : public Activation<Real>
    {
    public:
        virtual const Array<Real> operator()(const Array<Real> &t) const override;
    };

    template <typename Real>
    class Linear : public Layer<Real>
    {
    protected:
        bool bias;
        Array<Real> A;
        Array<Real> b;

    public:
        Linear(const size_t &input, const size_t &output, const bool &bias = true);

        void setWeight(const Array<Real> &A);
        void setBias(const Array<Real> &b);

        const Array<Real> getWeight() const;
        const Array<Real> getBias() const;

        virtual void load(std::istream &stream) override;

        virtual const Array<Real> operator()(const Array<Real> &t) const override;
    };

    template <typename Real>
    class NeuralNetwork
    {
    protected:
        std::vector<AbstractLayer<Real> *> layer;

    public:
        NeuralNetwork();
        ~NeuralNetwork();

        void load(const std::string &filename);
        void push_back(AbstractLayer<Real> *p);

        const Array<Real> operator()(const Array<Real> &x) const;
    };
};

#include "NeuralNetwork.hpp"

#endif