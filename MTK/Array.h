#ifndef MTK_ARRAY_H
#define MTK_ARRAY_H

#include <iostream>
#include <vector>

// #define MTK_NO_CHECK

static_assert(__cplusplus >= 201700, "C++17 or higher is required.");

namespace mtk
{
    template <typename Type>
    class Array;

    template <typename Type>
    std::istream &operator>>(std::istream &stream, Array<Type> &array);
    template <typename Type>
    std::ostream &operator<<(std::ostream &stream, const Array<Type> &array);

    template <typename Type>
    const Array<Type> operator-(const Array<Type> &array);
    template <typename Type>
    const Array<Type> operator+(const Array<Type> &array);
    template <typename Type>
    const Array<Type> operator+(const Array<Type> &array1, const Array<Type> &array2);
    template <typename Type>
    const Array<Type> operator+(const Array<Type> &array, const Type &k);
    template <typename Type>
    const Array<Type> operator+(const Type &k, const Array<Type> &array);
    template <typename Type>
    const Array<Type> operator-(const Array<Type> &array1, const Array<Type> &array2);
    template <typename Type>
    const Array<Type> operator-(const Array<Type> &array, const Type &k);
    template <typename Type>
    const Array<Type> operator-(const Type &k, const Array<Type> &array);
    template <typename Type>
    const Array<Type> operator*(const Array<Type> &array1, const Array<Type> &array2);
    template <typename Type>
    const Array<Type> operator*(const Type &k, const Array<Type> &array);
    template <typename Type>
    const Array<Type> operator*(const Array<Type> &array, const Type &k);
    template <typename Type>
    const Array<Type> operator/(const Array<Type> &array1, const Array<Type> &array2);
    template <typename Type>
    const Array<Type> operator/(const Array<Type> &array, const Type &k);
    template <typename Type>
    const Array<Type> operator/(const Type &k, const Array<Type> &array);

    template <typename Type>
    class Array
    {
        static_assert(std::is_floating_point_v<Type> || std::is_integral_v<Type>);

    private:
        std::vector<Type> _data;
        std::vector<size_t> _shape;

    public:
        const std::vector<Type> &data;
        const std::vector<size_t> &shape;

    public:
        Array();
        Array(const std::vector<size_t> &shape);
        template <typename... IndexTypes>
        Array(const size_t &firstIndex, const IndexTypes &...otherIndices);
        Array(const Array &array);

        void fill(const Type &value);

        const size_t size() const;
        void reshape(const std::vector<size_t> &shape);
        template <typename... IndexTypes>
        void reshape(const size_t &firstIndex, const IndexTypes &...otherIndices);

        const Type &operator[](const size_t &index) const;
        Type &operator[](const size_t &index);

        const Type &operator()(const std::vector<size_t> &index) const;
        template <typename... IndexTypes>
        const Type &operator()(const size_t &firstIndex, const IndexTypes &...otherIndices) const;

        Type &operator()(const std::vector<size_t> &index);
        template <typename... IndexTypes>
        Type &operator()(const size_t &firstIndex, const IndexTypes &...otherIndices);

        Array &operator=(const Array &array);
        Array &operator+=(const Array &array);
        Array &operator+=(const Type &k);
        Array &operator-=(const Array &array);
        Array &operator-=(const Type &k);
        Array &operator*=(const Array &array);
        Array &operator*=(const Type &k);
        Array &operator/=(const Array &array);
        Array &operator/=(const Type &k);
    };
};

#include "Array.hpp"

#endif