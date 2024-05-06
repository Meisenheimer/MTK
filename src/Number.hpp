#ifndef MTK_NUMBER_HPP
#define MTK_NUMBER_HPP

#include "Number.h"
#include "Random.h"

namespace mtk
{
    inline Bool isPrime(const Int &x)
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
        Set<Int> list = {2, 7, 61, 325, 9375, 28178, 450775, 9780504, 1795265022};
        while (list.size() < 20)
        {
            list.insert(random.UniformInt(3, INT_MAX));
        }
        Int u = x - 1, t = 0;
        while (u % 2 == 0)
        {
            u /= 2;
            t++;
        }
        for (auto it = list.begin(); it != list.end(); it++)
        {
            Int a = *it;
            if (a >= x || (a <= x && gcd<Int>(a, x) != 1))
            {
                continue;
            }
            Int v = pow<Int>(a, u, x);
            if (v != 1)
            {
                Int s;
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

    inline Prime::Prime(const Int &m)
    {
        this->max = (std::max<Int>(SHRT_MAX, m));
        List<Bool> flag(max + 1, true);
        num.push_back(2);
        for (Int i = 4; i <= max; i += 2)
        {
            flag[i] = false;
        }
        for (Int i = 3; i <= max; i += 2)
        {
            if (flag[i])
            {
                num.push_back(i);
                for (Int j = i * i; j <= max; j += (2 * i))
                {
                    flag[j] = false;
                }
            }
        }
    }

    inline Int Prime::index(const Int &x) const
    {
        Int l = 0, r = num.size() - 1;
        while (l <= r)
        {
            Int m = l + ((r - l) / 2);
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

    inline Bool Prime::operator()(const Int &n)
    {
        if (n <= max)
        {
            return index(n) != -1;
        }
        else if (n <= std::min<Int>(max * max, INT_MAX))
        {
            for (Int i = 0; i < num.size(); i++)
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

    inline List<Real> Prime::factorization(const Int &x)
    {
        Int u = x;
        List<Real> res;
        for (Int i = 0; i < num.size(); i++)
        {
            while (u % num[i] == 0)
            {
                u /= num[i];
                res.push_back(i);
            }
        }
        for (Int i = max; i <= (sqrt(u) + 1) && isPrime(u) && u > 1; i++)
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