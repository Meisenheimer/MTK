#ifndef MTK_POLYNOMIAL_HPP
#define MTK_POLYNOMIAL_HPP

#include "Polynomial.h"

namespace mtk
{
    template <typename Real>
    inline ConditionList<Real>::Condition::Condition() : Condition(true, false){};

    template <typename Real>
    inline ConditionList<Real>::Condition::Condition(const bool &is_knot, const bool &is_smooth)
    {
        this->knot = is_knot;
        this->smooth = is_smooth;
    };

    template <typename Real>
    inline ConditionList<Real>::Condition::Condition(const std::map<size_t, Real> &y, const bool &is_knot, const bool &is_smooth)
        : Condition(is_knot, is_smooth)
    {
        this->y = y;
    }

    template <typename Real>
    inline ConditionList<Real>::ConditionList() : list(_list) {}

    template <typename Real>
    inline ConditionList<Real>::ConditionList(const std::map<Real, ConditionList<Real>::Condition> &init_list) : ConditionList()
    {
        this->_list = init_list;
    }

    template <typename Real>
    inline void ConditionList<Real>::setCondition(const Real &x, const Real &y, const size_t &order)
    {
        _list[x].y[order] = y;
        return;
    }

    template <typename Real>
    inline void ConditionList<Real>::setKnot(const Real &x, const bool &is_knot)
    {
        _list[x].knot = is_knot;
        return;
    }

    template <typename Real>
    inline void ConditionList<Real>::setSmooth(const Real &x, const bool &is_smooth)
    {
        _list[x].smooth = is_smooth;
        return;
    }

    template <typename Real>
    inline const ConditionList<Real>::Condition &ConditionList<Real>::operator()(const Real &x) const
    {
        auto it = list.find(x);
        if (it == list.end())
        {
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        return it->second;
    }

    template <typename Real>
    inline const Polynomial<Real> operator-(const Polynomial<Real> &p)
    {
        Polynomial<Real> res(p);
        for (size_t i = 0; i <= res.degree; i++)
        {
            res[i] = -res[i];
        }
        return res;
    }

    template <typename Real>
    inline const Polynomial<Real> operator+(const Polynomial<Real> &p)
    {
        return p;
    }

    template <typename Real1, typename Real2>
    inline const Polynomial<std::common_type_t<Real1, Real2>> operator+(const Polynomial<Real1> &p1,
                                                                        const Polynomial<Real2> &p2)
    {
        Polynomial<std::common_type_t<Real1, Real2>> res = p1;
        res += p2;
        return res;
    }

    template <typename Real, typename Type>
    inline const Polynomial<Real> operator+(const Polynomial<Real> &p, const Type &k)
    {
        Polynomial<Real> res = p;
        res += k;
        return res;
    }

    template <typename Real, typename Type>
    inline const Polynomial<Real> operator+(const Type &k, const Polynomial<Real> &p)
    {
        Polynomial<Real> res = p;
        res += k;
        return res;
    }

    template <typename Real1, typename Real2>
    inline const Polynomial<std::common_type_t<Real1, Real2>> operator-(const Polynomial<Real1> &p1,
                                                                        const Polynomial<Real2> &p2)
    {
        Polynomial<std::common_type_t<Real1, Real2>> res = p1;
        res -= p2;
        return res;
    }

    template <typename Real, typename Type>
    inline const Polynomial<Real> operator-(const Polynomial<Real> &p, const Type &k)
    {
        Polynomial<Real> res = p;
        res -= k;
        return res;
    }

    template <typename Real, typename Type>
    inline const Polynomial<Real> operator-(const Type &k, const Polynomial<Real> &p)
    {
        Polynomial<Real> res = p;
        res -= k;
        return res;
    }

    template <typename Real1, typename Real2>
    inline const Polynomial<std::common_type_t<Real1, Real2>> operator*(const Polynomial<Real1> &p1,
                                                                        const Polynomial<Real2> &p2)
    {
        Polynomial<std::common_type_t<Real1, Real2>> res = p1;
        res *= p2;
        return res;
    }

    template <typename Real, typename Type>
    inline const Polynomial<Real> operator*(const Type &k, const Polynomial<Real> &p)
    {
        Polynomial<Real> res = p;
        res *= k;
        return res;
    }

    template <typename Real, typename Type>
    inline const Polynomial<Real> operator*(const Polynomial<Real> &p, const Type &k)
    {
        Polynomial<Real> res = p;
        res *= k;
        return res;
    }

    template <typename Real, typename Type>
    inline const Polynomial<Real> operator/(const Polynomial<Real> &p, const Type &k)
    {
        Polynomial<Real> res = p;
        res /= k;
        return res;
    }

    template <typename Real1, typename Real2>
    inline const Polynomial<std::common_type_t<Real1, Real2>> operator/(const Polynomial<Real1> &p1,
                                                                        const Polynomial<Real2> &p2)
    {
        Polynomial<std::common_type_t<Real1, Real2>> res = p1;
        res /= p2;
        return res;
    }

    template <typename Real1, typename Real2>
    inline const Polynomial<std::common_type_t<Real1, Real2>> operator%(const Polynomial<Real1> &p1,
                                                                        const Polynomial<Real2> &p2)
    {
        Polynomial<std::common_type_t<Real1, Real2>> res = p1;
        res %= p2;
        return res;
    }

    template <typename Real>
    inline const bool isEqual(const Polynomial<Real> &p1, const Polynomial<Real> &p2, const Real &delta)
    {
        return p1.equal(p2, delta);
    }

    template <typename Real>
    inline const bool operator==(const Polynomial<Real> &p1, const Polynomial<Real> &p2)
    {
        size_t n = std::max(p1.degree, p2.degree);
        for (size_t i = 0; i <= n; i++)
        {
            Real tmp = std::max(std::abs(p1[i]), std::abs(p2[i]));
            if (std::abs(p1[i] - p2[i]) > tmp * Trait<float>::epsilon())
            {
                return false;
            }
        }
        return true;
    }

    template <typename Real>
    inline const bool operator!=(const Polynomial<Real> &p1, const Polynomial<Real> &p2)
    {
        return !(p1 == p2);
    }

    template <typename Real>
    inline std::ostream &operator<<(std::ostream &stream, const Polynomial<Real> &p)
    {
        stream << p.print(Trait<float>::min());
        return stream;
    }

    template <typename Real>
    inline const std::vector<Polynomial<Real>> fundamentalPolynomial(const std::vector<Real> &x)
    {
        std::vector<Polynomial<Real>> res;
        for (size_t k = 0; k < x.size(); k++)
        {
            Polynomial<Real> p({1});
            for (size_t i = 0; i < x.size(); i++)
            {
                Polynomial<Real> tmp({-x[i], 1});
                tmp /= (x[k] - x[i]);
                p *= tmp;
            }
            res.push_back(p);
        }
        return res;
    }

    template <typename Real>
    inline const Polynomial<Real> newtonFormula(const std::vector<Real> &x, const std::vector<Real> &y)
    {
        const size_t n = x.size();
        if (n != y.size())
        {
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        std::vector<std::vector<Real>> table(n);
        for (size_t i = 0; i < n; i++)
        {
            table[i].resize(i + 1);
            table[i][0] = y[i];
            for (size_t j = 0; j < i; j++)
            {
                table[i][j] = (table[i][j - 1] - table[i - 1][j - 1]) / (x[i] - x[i - j]);
            }
        }
        Polynomial<Real> s(0);
        Polynomial<Real> tmp({1});
        for (size_t i = n; i > 0; i--)
        {
            s += table[i - 1][i] * tmp;
            tmp *= Polynomial<Real>({-x[i - 1], 1});
        }
        return s;
    }

    template <typename Real>
    inline const Polynomial<Real> fitPolynomial(const size_t &degree, const ConditionList<Real> &condition)
    {
        const size_t n = condition.list.size();
        Matrix<Real> A = Matrix<Real>::Zero(n, degree + 1);
        Vector<Real> b = Vector<Real>::Zero(n);
        size_t i = 0;
        for (auto it = condition.list.begin(); it != condition.list.end(); it++)
        {
            for (auto cond = it->second.y.begin(); cond != it->second.y.end(); cond++, i++)
            {
                size_t order = cond->first;
                Real x = 1.0;
                Real c = 1.0;
                for (size_t j = 1; j <= order; j++)
                {
                    c = c * j;
                }
                if (degree < order)
                {
                    printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
                    exit(0);
                }
                for (size_t j = 0; j <= degree - order; j++)
                {
                    A(i, j) = c * x;
                    x = x * it->first;
                    c = c * Real(order + j + 1) / Real(j + 1);
                }
                b(i) = cond->second;
            }
        }
        Vector<Real> x = A.fullPivHouseholderQr().solve(b);
        Polynomial<Real> p(degree);
        for (size_t i = 0; i <= degree; i++)
        {
            p[i] = x(i);
        }
        return p;
    }

    template <typename Real>
    inline const Polynomial<Real> differential(const Polynomial<Real> &p)
    {
        return p.differential();
    }

    template <typename Real>
    inline const Polynomial<Real> integral(const Polynomial<Real> &p, const Real &x)
    {
        return p.integral(x);
    }

    template <typename Real>
    inline Polynomial<Real>::Polynomial(const size_t &n) : coefs(_coefs), degree(_degree)
    {
        if (n < 0)
        {
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        _degree = n;
        this->_coefs.resize(n + 1);
        for (size_t i = 0; i <= n; i++)
        {
            this->_coefs[i] = 0;
        }
    }

    template <typename Real>
    inline Polynomial<Real>::Polynomial(const Polynomial &p) : coefs(_coefs), degree(_degree)
    {
        _degree = p.degree;
        this->_coefs.assign(p.coefs.begin(), p.coefs.end());
    }

    template <typename Real>
    inline Polynomial<Real>::Polynomial(const std::vector<Real> &coefs) : coefs(_coefs), degree(_degree)
    {
        _degree = coefs.size() - 1;
        this->_coefs.assign(coefs.begin(), coefs.end());
    }

    template <typename Real>
    inline const Polynomial<Real> Polynomial<Real>::differential() const
    {
        Polynomial res(degree - 1);
        for (size_t i = 1; i <= degree; i++)
        {
            res[i - 1] = (i * _coefs[i]);
        }
        return res;
    }

    template <typename Real>
    inline const Polynomial<Real> Polynomial<Real>::integral(const Real &x) const
    {
        Polynomial res(degree + 1);
        for (size_t i = 1; i <= degree; i++)
        {
            res[i + 1] = (_coefs[i] / (i + 1.0));
        }
        res[0] = res[0] - res(x);
        return res;
    }

    template <typename Real>
    inline const std::vector<Real> Polynomial<Real>::root(const Real &delta) const
    {
        const size_t n = degree;
        Matrix<Real> A = Matrix<Real>::Zero(n, n);
        for (size_t i = 1; i < n; i++)
        {
            A(i, i - 1) = 1.0;
        }
        for (size_t i = 0; i < n; i++)
        {
            A(0, i) = -_coefs[n - i - 1] / _coefs[n];
        }
        std::vector<Real> res;
        Eigen::EigenSolver<Matrix<Real>> re(A);
        Matrix<Real> D = re.pseudoEigenvalueMatrix();
        for (size_t i = 0; i < n; i++)
        {
            if (std::abs(operator()(D(i, i))) < delta)
            {
                res.push_back(D(i, i));
            }
        }
        std::sort(res.begin(), res.end());
        return res;
    }

    template <typename Real>
    inline const bool Polynomial<Real>::isRoot(const Real &x, const Real &delta) const
    {
        return std::abs(operator()(x)) < delta;
    }

    template <typename Real>
    inline const bool Polynomial<Real>::equal(const Polynomial &p, const Real &delta) const
    {
        int n = std::max(p.degree, degree);
        for (size_t i = 0; i <= n; i++)
        {
            if (std::abs(p[i] - this->_coefs[i]) > delta)
            {
                return false;
            }
        }
        return true;
    }

    template <typename Real>
    inline std::string Polynomial<Real>::print(const Real &precision) const
    {
        std::string s = "";
        bool flag = true;
        for (size_t i = degree; i > 0; i--)
        {
            if (std::abs(_coefs[i]) > precision)
            {
                s += ((_coefs[i] > 0 ? (i == degree ? "" : "+ ") : "- ") + std::to_string(std::abs(_coefs[i])));
                s += (" x^" + std::to_string(i) + " ");
                flag = false;
            }
        }
        if (flag)
        {
            s += std::to_string(_coefs[0]);
        }
        else if (std::abs(_coefs[0]) > precision)
        {
            s += ((_coefs[0] > 0 ? "+ " : "- ") + std::to_string(std::abs(_coefs[0])));
        }
        return s;
    }

    template <typename Real>
    inline Polynomial<Real> &Polynomial<Real>::operator=(const Polynomial &p)
    {
        if (&p != this)
        {
            this->_degree = p.degree;
            this->_coefs = p.coefs;
        }
        return (*this);
    }

    template <typename Real>
    inline Polynomial<Real> &Polynomial<Real>::operator=(const Real &k)
    {
        this->_degree = 0;
        this->_coefs.resize(1);
        this->_coefs[0] = k;
        return (*this);
    }

    template <typename Real>
    inline Polynomial<Real> &Polynomial<Real>::operator+=(const Polynomial &p)
    {
        _degree = (std::max(degree, p.degree));
        _coefs.resize(_degree + 1, Trait<Real>::zero());
        for (size_t i = 0; i <= p.degree; i++)
        {
            _coefs[i] += p[i];
        }
        return (*this);
    }

    template <typename Real>
    inline Polynomial<Real> &Polynomial<Real>::operator+=(const Real &k)
    {
        _coefs[0] += k;
        return (*this);
    }

    template <typename Real>
    inline Polynomial<Real> &Polynomial<Real>::operator-=(const Polynomial &p)
    {
        _degree = (std::max(degree, p.degree));
        _coefs.resize(_degree + 1, Trait<Real>::zero());
        for (size_t i = 0; i <= p.degree; i++)
        {
            _coefs[i] -= p[i];
        }
        return (*this);
    }

    template <typename Real>
    inline Polynomial<Real> &Polynomial<Real>::operator-=(const Real &k)
    {
        _coefs[0] -= k;
        return (*this);
    }

    template <typename Real>
    inline Polynomial<Real> &Polynomial<Real>::operator*=(const Polynomial &p)
    {
        Polynomial tmp(degree + p.degree);
        for (size_t i = 0; i <= degree; i++)
        {
            for (size_t j = 0; j <= p.degree; j++)
            {
                tmp[i + j] += (_coefs[i] * p[j]);
            }
        }
        (*this) = tmp;
        return (*this);
    }

    template <typename Real>
    inline Polynomial<Real> &Polynomial<Real>::operator*=(const Real &k)
    {
        for (size_t i = 0; i <= degree; i++)
        {
            _coefs[i] *= k;
        }
        return (*this);
    }

    template <typename Real>
    inline Polynomial<Real> &Polynomial<Real>::operator/=(const Real &k)
    {
        for (size_t i = 0; i <= degree; i++)
        {
            _coefs[i] /= k;
        }
        return (*this);
    }

    template <typename Real>
    inline Polynomial<Real> &Polynomial<Real>::operator/=(const Polynomial<Real> &p)
    {
        Polynomial res = Trait<Polynomial>::identity();
        while (_degree >= p.degree)
        {
            size_t i = _degree - p.degree;
            Polynomial x = (_coefs[_degree] / p[p.degree]) * Trait<Polynomial>::basis(i);
            res += x;
            (*this) -= (x * p);
            _coefs.resize(_degree);
            _degree--;
        }
        (*this) = res;
        return (*this);
    }

    template <typename Real>
    inline Polynomial<Real> &Polynomial<Real>::operator%=(const Polynomial<Real> &p)
    {
        Polynomial res = Trait<Polynomial>::identity();
        while (_degree >= p.degree)
        {
            size_t i = _degree - p.degree;
            Polynomial x = (_coefs[_degree] / p[p.degree]) * Trait<Polynomial>::basis(i);
            res += x;
            (*this) -= (x * p);
            _coefs.resize(_degree);
            _degree--;
        }
        return (*this);
    }

    template <typename Real>
    inline const Real Polynomial<Real>::operator()(const Real &x) const
    {
        Real res = 0.0;
        Real k = 1.0;
        for (size_t i = 0; i <= degree; i++, k *= x)
        {
            res += (k * this->_coefs[i]);
        }
        return res;
    }

    template <typename Real>
    inline const Real Polynomial<Real>::operator[](const size_t &n) const
    {
        if (n > degree)
        {
            return 0.0;
        }
        return this->_coefs[n];
    }

    template <typename Real>
    inline Real &Polynomial<Real>::operator[](const size_t &n)
    {
        if (n > degree)
        {
            printf("Error at: file %s line %d.\n", __FILE__, __LINE__);
            exit(0);
        }
        return this->_coefs[n];
    }

    template <typename Real>
    inline const Polynomial<Real> Trait<Polynomial<Real>>::zero()
    {
        return Polynomial<Real>(Trait<Real>::zero());
    }

    template <typename Real>
    inline const Polynomial<Real> Trait<Polynomial<Real>>::identity()
    {
        return Polynomial<Real>(std::vector<Real>({Trait<Real>::identity()}));
    }

    template <typename Real>
    inline const Polynomial<Real> Trait<Polynomial<Real>>::basis(const size_t &n)
    {
        Polynomial<Real> p(n);
        p[n] = Trait<Real>::identity();
        return p;
    }

    template <typename Real>
    inline OrthogonalPolynomial<Real>::OrthogonalPolynomial(const Type &type) : weight(_weight), poly(_poly), range(_range)
    {
        const Polynomial<Real> X = Polynomial<Real>(std::vector<Real>({0.0, 1.0}));
        switch (type)
        {
        case Type::Legendre:
            _weight = [](const Real &x) -> Real
            { return 1.0; };
            _poly.push_back(Polynomial<Real>(std::vector<Real>({1.0})));
            _poly.push_back(Polynomial<Real>(std::vector<Real>({0.0, 1.0})));
            next = [&X = std::as_const(X)](const std::vector<Polynomial<Real>> &list) -> Polynomial<Real>
            {
                const size_t n = list.size() - 1;
                return ((2 * n + 1) * X * list[n] - n * list[n - 1]) / (n + 1);
            };
            _range = std::make_pair(-1.0, 1.0);
            break;
        case Type::ChebyshevOfFirstClass:
            _weight = [](const Real &x) -> Real
            { return 1.0 / std::sqrt(1 - x * x); };
            _poly.push_back(Polynomial<Real>(std::vector<Real>({1.0})));
            _poly.push_back(Polynomial<Real>(std::vector<Real>({0.0, 1.0})));
            next = [&X = std::as_const(X)](const std::vector<Polynomial<Real>> &list) -> Polynomial<Real>
            {
                const size_t n = list.size() - 1;
                return 2 * X * list[n] - list[n - 1];
            };
            _range = std::make_pair(-1.0, 1.0);
            break;
        case Type::ChebyshevOfSecondClass:
            _weight = [](const Real &x) -> Real
            { return std::sqrt(1 - x * x); };
            _poly.push_back(Polynomial<Real>(std::vector<Real>({1.0})));
            _poly.push_back(Polynomial<Real>(std::vector<Real>({0.0, 2.0})));
            next = [&X = std::as_const(X)](const std::vector<Polynomial<Real>> &list) -> Polynomial<Real>
            {
                const size_t n = list.size() - 1;
                return 2 * X * list[n] - list[n - 1];
            };
            _range = std::make_pair(-1.0, 1.0);
            break;
        case Type::Laguerre:
            _weight = [](const Real &x) -> Real
            { return std::exp(-x); };
            _poly.push_back(Polynomial<Real>(std::vector<Real>({1.0})));
            _poly.push_back(Polynomial<Real>(std::vector<Real>({1.0, -1.0})));
            next = [&X = std::as_const(X)](const std::vector<Polynomial<Real>> &list) -> Polynomial<Real>
            {
                const size_t n = list.size() - 1;
                return ((2 * n + 1 - X) * list[n] - n * list[n - 1]) / (n + 1);
            };
            _range = std::make_pair(0.0, INFINITY);
            break;
        case Type::Hermite:
            _weight = [](const Real &x) -> Real
            { return std::exp(-x * x / 2) / std::sqrt(2 * std::numbers::pi); };
            _poly.push_back(Polynomial<Real>(std::vector<Real>({1.0})));
            _poly.push_back(Polynomial<Real>(std::vector<Real>({0.0, 1.0})));
            next = [&X = std::as_const(X)](const std::vector<Polynomial<Real>> &list) -> Polynomial<Real>
            {
                const size_t n = list.size() - 1;
                return X * list[n] - n * list[n - 1];
            };
            _range = std::make_pair(-INFINITY, INFINITY);
            break;
        default:
            break;
        }
    }

    template <typename Real>
    inline OrthogonalPolynomial<Real>::OrthogonalPolynomial(const OrthogonalPolynomial &op)
        : weight(_weight), poly(_poly), range(_range), _weight(op.weight), _poly(op.poly), _range(op.range), next(op.next) {}

    template <typename Real>
    inline const Polynomial<Real> &OrthogonalPolynomial<Real>::operator()(const size_t &index)
    {
        for (size_t i = poly.size(); i <= index; i++)
        {
            _poly.push_back(next(poly));
        }
        return poly[index];
    }

    template <typename Real>
    inline OrthogonalPolynomial<Real> &OrthogonalPolynomial<Real>::operator=(const OrthogonalPolynomial<Real> &op)
    {
        if (this != &op)
        {
            this->next = op.next;
            this->_weight = op.weight;
            this->_poly = op.poly;
            this->_range = op.range;
        }
        return (*this);
    }
};

#endif
