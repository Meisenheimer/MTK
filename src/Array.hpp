#ifndef MTK_ARRAY_HPP
#define MTK_ARRAY_HPP

#include "Array.h"

namespace mtk
{
    template <typename Type>
    inline std::istream &operator>>(std::istream &stream, Array<Type> &array)
    {
        for (size_t i = 0; i < array.size(); i++)
        {
            stream >> array[i];
        }
        return stream;
    }

    template <typename Type>
    inline std::ostream &operator<<(std::ostream &stream, const Array<Type> &array)
    {
        for (size_t i = 0; i < array.size(); i++)
        {
            stream << array[i] << " ";
        }
        return stream;
    }

    template <typename Type>
    inline const Array<Type> operator-(const Array<Type> &array)
    {
        Array res(array);
        for (size_t i = 0; i < array.size(); i++)
        {
            res[i] = -res[i];
        }
        return res;
    }

    template <typename Type>
    inline const Array<Type> operator+(const Array<Type> &array)
    {
        return array;
    }

    template <typename Type>
    inline const Array<Type> operator+(const Array<Type> &array1, const Array<Type> &array2)
    {
        Array res = array1;
        res += array2;
        return res;
    }

    template <typename Type>
    inline const Array<Type> operator+(const Array<Type> &array, const Type &k)
    {
        Array res = array;
        res += k;
        return res;
    }

    template <typename Type>
    inline const Array<Type> operator+(const Type &k, const Array<Type> &array)
    {
        Array res = array;
        res += k;
        return res;
    }

    template <typename Type>
    inline const Array<Type> operator-(const Array<Type> &array1, const Array<Type> &array2)
    {
        Array res = array1;
        res -= array2;
        return res;
    }

    template <typename Type>
    inline const Array<Type> operator-(const Array<Type> &array, const Type &k)
    {
        Array res = array;
        res -= k;
        return res;
    }

    template <typename Type>
    inline const Array<Type> operator-(const Type &k, const Array<Type> &array)
    {
        Array res(array.shape);
        res.fill(k);
        res -= array;
        return res;
    }

    template <typename Type>
    inline const Array<Type> operator*(const Array<Type> &array1, const Array<Type> &array2)
    {
        Array res = array1;
        res *= array2;
        return res;
    }

    template <typename Type>
    inline const Array<Type> operator*(const Type &k, const Array<Type> &array)
    {
        Array res = array;
        res *= k;
        return res;
    }

    template <typename Type>
    inline const Array<Type> operator*(const Array<Type> &array, const Type &k)
    {
        Array res = array;
        res *= k;
        return res;
    }

    template <typename Type>
    inline const Array<Type> operator/(const Array<Type> &array1, const Array<Type> &array2)
    {
        Array res = array1;
        res /= array2;
        return res;
    }

    template <typename Type>
    inline const Array<Type> operator/(const Array<Type> &array, const Type &k)
    {
        Array res = array;
        res /= k;
        return res;
    }

    template <typename Type>
    inline const Array<Type> operator/(const Type &k, const Array<Type> &array)
    {
        Array res(array.shape);
        res.fill(k);
        res /= array;
        return res;
    }

    template <typename Type>
    inline Array<Type>::Array() : shape(_shape)
    {
        _shape = {0};
    }

    template <typename Type>
    inline Array<Type>::Array(const std::vector<size_t> &shape) : Array()
    {
        this->_shape = shape;
        data.resize(size(), 0);
    }

    template <typename Type>
    template <typename... IndexTypes>
    inline Array<Type>::Array(const size_t &firstIndex, const IndexTypes &...otherIndices)
        : Array(std::vector<size_t>({(size_t)firstIndex, (size_t)otherIndices...})) {}

    template <typename Type>
    inline Array<Type>::Array(const Array &array) : Array(array.shape)
    {
        data = array.data;
    }

    template <typename Type>
    inline void Array<Type>::fill(const Type &value)
    {
        for (size_t i = 0; i < data.size; i++)
        {
            data[i] = value;
        }
        return;
    }

    template <typename Type>
    inline const size_t Array<Type>::size() const
    {
        size_t s = 1;
        for (size_t i = 0; i < shape.size(); i++)
        {
            s *= shape.at(i);
        }
        return s;
    }

    template <typename Type>
    inline void Array<Type>::reshape(const std::vector<size_t> &shape)
    {
        size_t n = 1;
        for (size_t i = 0; i < shape.size(); i++)
        {
            n *= shape[i];
        }
        if (n != size())
        {
            printf("Error At: %s %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        this->_shape = shape;
        return;
    }

    template <typename Type>
    template <typename... IndexTypes>
    inline void Array<Type>::reshape(const size_t &firstIndex, const IndexTypes &...otherIndices)
    {
        reshape(std::vector<size_t>({(size_t)firstIndex, (size_t)otherIndices...}));
        return;
    }

    template <typename Type>
    inline const Type &Array<Type>::operator[](const size_t &index) const
    {
        if (index >= data.size())
        {
            printf("Error At: %s %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        return data[index];
    }

    template <typename Type>
    inline Type &Array<Type>::operator[](const size_t &index)
    {
        if (index >= data.size())
        {
            printf("Error At: %s %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        return data[index];
    }

    template <typename Type>
    inline const Type &Array<Type>::operator()(const std::vector<size_t> &index) const
    {
        if (index.size() != shape.size())
        {
            printf("Error At: %s %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        size_t n = 0;
        size_t m = 1;
        for (size_t i = index.size(); i > 0; i--)
        {
            n += (m * index[i - 1]);
            m *= shape.at(i - 1);
        }
        return data.at(n);
    }

    template <typename Type>
    template <typename... IndexTypes>
    inline const Type &Array<Type>::operator()(const size_t &firstIndex, const IndexTypes &...otherIndices) const
    {
        if (sizeof...(otherIndices) + 1 != shape.size())
        {
            printf("Error At: %s %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        return this->operator()(std::vector<size_t>({(size_t)firstIndex, (size_t)otherIndices...}));
    }

    template <typename Type>
    inline Type &Array<Type>::operator()(const std::vector<size_t> &index)
    {
        if (index.size() != shape.size())
        {
            printf("Error At: %s %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        size_t n = 0;
        size_t m = 1;
        for (size_t i = index.size(); i > 0; i--)
        {
            n += (m * index[i - 1]);
            m *= shape[i - 1];
        }
        return data.at(n);
    }

    template <typename Type>
    template <typename... IndexTypes>
    inline Type &Array<Type>::operator()(const size_t &firstIndex, const IndexTypes &...otherIndices)
    {
        if (sizeof...(otherIndices) + 1 != shape.size())
        {
            printf("Error At: %s %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        return this->operator()(std::vector<size_t>({(size_t)firstIndex, (size_t)otherIndices...}));
    }

    template <typename Type>
    inline Array<Type> &Array<Type>::operator=(const Array<Type> &array)
    {
        _shape = array.shape;
        data = array.data;
        return (*this);
    }

    template <typename Type>
    inline Array<Type> &Array<Type>::operator+=(const Array<Type> &array)
    {
        if (array.data.size() != data.size())
        {
            printf("Error At: %s %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        for (size_t i = 0; i < data.size(); i++)
        {
            data.at(i) += array.data.at(i);
        }
        return (*this);
    }

    template <typename Type>
    inline Array<Type> &Array<Type>::operator+=(const Type &k)
    {
        for (size_t i = 0; i < data.size(); i++)
        {
            data.at(i) += k;
        }
        return (*this);
    }

    template <typename Type>
    inline Array<Type> &Array<Type>::operator-=(const Array<Type> &array)
    {
        if (array.data.size() != data.size())
        {
            printf("Error At: %s %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        for (size_t i = 0; i < data.size(); i++)
        {
            data[i] -= array.data[i];
        }
        return (*this);
    }

    template <typename Type>
    inline Array<Type> &Array<Type>::operator-=(const Type &k)
    {
        for (size_t i = 0; i < data.size(); i++)
        {
            data[i] -= k;
        }
        return (*this);
    }

    template <typename Type>
    inline Array<Type> &Array<Type>::operator*=(const Array &array)
    {
        if (array.data.size() != data.size())
        {
            printf("Error At: %s %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        for (size_t i = 0; i < data.size(); i++)
        {
            data[i] *= array.data[i];
        }
        return (*this);
    }

    template <typename Type>
    inline Array<Type> &Array<Type>::operator*=(const Type &k)
    {
        for (size_t i = 0; i < data.size(); i++)
        {
            data[i] *= k;
        }
        return (*this);
    }

    template <typename Type>
    inline Array<Type> &Array<Type>::operator/=(const Array &array)
    {
        if (array.data.size() != data.size())
        {
            printf("Error At: %s %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        for (size_t i = 0; i < data.size(); i++)
        {
            data[i] /= array.data[i];
        }
        return (*this);
    }

    template <typename Type>
    inline Array<Type> &Array<Type>::operator/=(const Type &k)
    {
        for (size_t i = 0; i < data.size(); i++)
        {
            data[i] /= k;
        }
        return (*this);
    }
};

#endif