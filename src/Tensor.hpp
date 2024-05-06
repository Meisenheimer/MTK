#ifndef MTK_TENSOR_HPP
#define MTK_TENSOR_HPP

#include "Tensor.h"

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
        if (n != size())
        {
            MTK_ERROR
        }
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
        if (index < 0 || index >= data.size())
        {
            MTK_ERROR
        }
        return data[index];
    }

    inline Real &Tensor::operator[](const Int &index)
    {
        if (index < 0 || index >= data.size())
        {
            MTK_ERROR
        }
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
    const Real &Tensor::operator()(Int firstIndex, IndexTypes... otherIndices) const
    {
        if (sizeof...(otherIndices) + 1 != size.size())
        {
            printf("Error At: %s %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        return this->operator()(std::vector<Int>({firstIndex, otherIndices...}));
    }

    template <typename... IndexTypes>
    inline Real &Tensor::operator()(Int firstIndex, IndexTypes... otherIndices)
    {
        if (sizeof...(otherIndices) + 1 != size.size())
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
        if (t.data.size() != data.size())
        {
            MTK_ERROR
        }
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
        if (t.data.size() != data.size())
        {
            MTK_ERROR
        }
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
};

#endif
