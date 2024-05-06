#ifndef MTK_TENSOR_H
#define MTK_TENSOR_H

#include "Config.h"

namespace mtk
{
    class Tensor;

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
};

#include "Tensor.hpp"

#endif
