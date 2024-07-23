#ifndef MTK_NUMBER_HPP
#define MTK_NUMBER_HPP

#include <set>

#include "Number.h"
#include "Random.h"
#include "Trait.h"

namespace mtk
{
    template <typename Type>
    inline const Type pow(const Type &x, const size_t &n, const Type &mod)
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
    inline const Type gcd(const Type &x, const Type &y)
    {
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

    inline bool isPrime(const size_t &x)
    {
        if (x == 2)
        {
            return true;
        }
        if (x <= 1 || x % 2 == 0)
        {
            return false;
        }
        static Random random;
        std::set<size_t> list = {2, 7, 61, 325, 9375, 28178, 450775, 9780504, 1795265022};
        while (list.size() < 20)
        {
            list.insert(Random::Uniform<size_t>(3, INT_MAX));
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

    inline Prime::Prime(const size_t &m)
    {
        this->max = (std::max<size_t>(SHRT_MAX, m));
        std::vector<bool> flag(max + 1, true);
        num.push_back(2);
        for (size_t i = 4; i <= max; i += 2)
        {
            flag[i] = false;
        }
        for (size_t i = 3; i <= max; i += 2)
        {
            if (flag[i])
            {
                num.push_back(i);
                for (size_t j = i * i; j <= max; j += (2 * i))
                {
                    flag[j] = false;
                }
            }
        }
    }

    inline size_t Prime::index(const size_t &x) const
    {
        size_t l = 0, r = num.size() - 1;
        while (l <= r)
        {
            size_t m = l + ((r - l) / 2);
            if (num[m] < x)
            {
                l = m + 1;
            }
            else if (num[m] > x)
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

    inline bool Prime::operator()(const size_t &n)
    {
        if (n <= max)
        {
            return index(n) != -1;
        }
        else if (n <= std::min<size_t>(max * max, INT_MAX))
        {
            for (size_t i = 0; i < num.size(); i++)
            {
                if (n % num[i] == 0)
                {
                    return false;
                }
            }
            return true;
        }
        return isPrime(n);
    }

    inline std::vector<size_t> Prime::factorization(const size_t &x)
    {
        size_t u = x;
        std::vector<size_t> res;
        for (size_t i = 0; i < num.size(); i++)
        {
            while (u % num[i] == 0)
            {
                u /= num[i];
                res.push_back(i);
            }
        }
        for (size_t i = max; i <= (sqrt(u) + 1) && isPrime(u) && u > 1; i++)
        {
            if (this->operator()(i))
            {
                while (u % num[i] == 0)
                {
                    u /= num[i];
                    res.push_back(i);
                }
            }
        }
        return res;
    }
};

#endif