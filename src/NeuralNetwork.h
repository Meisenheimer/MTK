#ifndef MTK_NEURALNETWORK_H
#define MTK_NEURALNETWORK_H

#include "Config.h"

namespace mtk
{
    class Tensor;
    class AbstractLayer;
    class Activation;
    class Layer;

    class LeakyReLU;
    class Sigmoid;

    class Linear;

    class NeuralNetwork;

    std::istream &operator>>(std::istream &stream, Tensor &t);
    std::ostream &operator<<(std::ostream &stream, const Tensor &t);

    const Tensor operator-(const Tensor &t);
    const Tensor operator+(const Tensor &t);
    const Tensor operator+(const Tensor &t1, const Tensor &t2);
    const Tensor operator+(const Tensor &t, const Real &k);
    const Tensor operator+(const Real &k, const Tensor &t);
    const Tensor operator-(const Tensor &t1, const Tensor &t2);
    const Tensor operator-(const Tensor &t, const Real &k);
    const Tensor operator*(const Real &k, const Tensor &t);
    const Tensor operator*(const Tensor &t, const Real &k);
    const Tensor operator/(const Tensor &t, const Real &k);

    class Tensor
    {
    private:
        List<Real> data;
        List<Int> _shape;

    public:
        const List<Int> &shape;

    public:
        Tensor();
        Tensor(const std::vector<Int> &shape);
        template <typename... IndexTypes>
        Tensor(Int firstIndex, IndexTypes... otherIndices);
        Tensor(const Tensor &t);

        Int size() const;
        void reshape(const std::vector<Int> &shape);
        template <typename... IndexTypes>
        void reshape(Int firstIndex, IndexTypes... otherIndices);

        const Real &operator[](const Int &index) const;
        Real &operator[](const Int &index);

        const Real &operator()(const std::vector<Int> &index) const;
        Real &operator()(const std::vector<Int> &index);

        template <typename... IndexTypes>
        const Real &operator()(Int firstIndex, IndexTypes... otherIndices) const;
        template <typename... IndexTypes>
        Real &operator()(Int firstIndex, IndexTypes... otherIndices);

        Tensor &operator=(const Tensor &t);
        Tensor &operator+=(const Tensor &t);
        Tensor &operator+=(const Real &k);
        Tensor &operator-=(const Tensor &t);
        Tensor &operator-=(const Real &k);
        Tensor &operator*=(const Real &k);
        Tensor &operator/=(const Real &k);

        friend std::istream &operator>>(std::istream &stream, Tensor &t);
        friend std::ostream &operator<<(std::ostream &stream, const Tensor &t);
    };

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