#ifndef MTK_NUMBER_HPP
#define MTK_NUMBER_HPP

#include <set>

#include "Number.h"
#include "Random.h"
#include "Trait.h"

namespace mtk
{
    template <typename Type>
    inline constexpr Type pow(const Type &x, const size_t &n, const Type &mod)
    {
        size_t m = n;
        Type a = x;
        Type res = Trait<Type>::identity(x);
        while (m)
        {
            if (m & 1)
            {
                res = res * a;
                if (mod > 0)
                {
                    res = res % mod;
                }
            }
            a *= a;
            if (mod > 0)
            {
                a = a % mod;
            }
            m >>= 1;
        }
        return res % mod;
    }

    template <typename Type>
    inline constexpr Type gcd(const Type &x, const Type &y)
    {
        if (x == Trait<Type>::zero(x) || Trait<Type>::zero(y))
        {
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        Type a = x;
        Type b = y;
        while ((a % b) != 0)
        {
            Type r = a % b;
            a = b;
            b = r;
        }
        return b;
    }

    inline const bool isPrime(const size_t &x)
    {
        static Random random;
        if (x == 2)
        {
            return true;
        }
        if (x <= 1 || x % 2 == 0)
        {
            return false;
        }
        std::set<size_t> list = {2, 7, 61, 325, 9375, 28178, 450775, 9780504, 1795265022};
        while (list.size() < 20)
        {
            list.insert(random.uniform<size_t>(3, INT_MAX));
        }
        size_t u = x - 1, t = 0;
        while (u % 2 == 0)
        {
            u /= 2;
            t++;
        }
        for (auto it = list.begin(); it != list.end(); it++)
        {
            size_t a = *it;
            if (a >= x || (a <= x && gcd<size_t>(a, x) != 1))
            {
                continue;
            }
            size_t v = pow<size_t>(a, u, x);
            if (v != 1)
            {
                size_t s;
                for (s = 0; s < t; ++s)
                {
                    if (v == (x - 1))
                    {
                        break;
                    }
                    v = ((v * v) % x);
                }
                if (s == t)
                {
                    return false;
                }
            }
        }
        return true;
    }

    inline Prime::Prime(const size_t &m) : max(_max), num(_num)
    {
        this->_max = (std::max<size_t>(SHRT_MAX, m));
        std::vector<bool> flag(_max + 1, true);
        _num.push_back(2);
        for (size_t i = 4; i <= _max; i += 2)
        {
            flag[i] = false;
        }
        for (size_t i = 3; i <= _max; i += 2)
        {
            if (flag[i])
            {
                _num.push_back(i);
                for (size_t j = i * i; j <= _max; j += (2 * i))
                {
                    flag[j] = false;
                }
            }
        }
    }

    inline Prime::Prime(const Prime &p) : max(_max), num(_num), _max(p.max), _num(p.num) {}

    inline const size_t Prime::index(const size_t &x) const
    {
        size_t l = 0, r = _num.size() - 1;
        while (l <= r)
        {
            size_t m = l + ((r - l) / 2);
            if (_num[m] < x)
            {
                l = m + 1;
            }
            else if (_num[m] > x)
            {
                r = m - 1;
            }
            else
            {
                return m;
            }
        }
        return -1;
    }

    inline const bool Prime::operator()(const size_t &n)
    {
        if (n <= _max)
        {
            return index(n) != -1;
        }
        else if (n <= std::min<size_t>(_max * _max, INT_MAX))
        {
            for (size_t i = 0; i < _num.size(); i++)
            {
                if (n % _num[i] == 0)
                {
                    return false;
                }
            }
            return true;
        }
        return isPrime(n);
    }

    inline const std::vector<size_t> Prime::factorization(const size_t &x)
    {
        size_t u = x;
        std::vector<size_t> res;
        for (size_t i = 0; i < _num.size(); i++)
        {
            while (u % _num[i] == 0)
            {
                u /= _num[i];
                res.push_back(i);
            }
        }
        for (size_t i = _max; i <= (sqrt(u) + 1) && isPrime(u) && u > 1; i++)
        {
            if (this->operator()(i))
            {
                while (u % _num[i] == 0)
                {
                    u /= _num[i];
                    res.push_back(i);
                }
            }
        }
        return res;
    }

    inline Prime &Prime::operator=(const Prime &p)
    {
        if (this != &p)
        {
            this->_max = p.max;
            this->_num = p.num;
        }
        return (*this);
    }
};

#endif