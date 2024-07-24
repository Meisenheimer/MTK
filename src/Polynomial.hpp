#ifndef MTK_POLYNOMIAL_HPP
#define MTK_POLYNOMIAL_HPP

#include "Polynomial.h"

namespace mtk
{
    inline ConditionList::Condition::Condition() : Condition(true, false) {};

    inline ConditionList::Condition::Condition(const bool &is_knot, const bool &is_smooth)
    {
        this->knot = is_knot;
        this->smooth = is_smooth;
    };

    inline ConditionList::Condition::Condition(const std::map<Int, Real> &y, const bool &is_knot, const bool &is_smooth)
        : Condition(is_knot, is_smooth)
    {
        this->y = y;
    }

    inline ConditionList::ConditionList() : list(_list) {}

    inline ConditionList::ConditionList(const std::map<Real, ConditionList::Condition> &init_list) : ConditionList()
    {
        this->_list = init_list;
    }

    inline void ConditionList::setCondition(const Real &x, const Real &y, const Int &order)
    {
        _list[x].y[order] = y;
        return;
    }

    inline void ConditionList::setKnot(const Real &x, const bool &is_knot)
    {
        _list[x].knot = is_knot;
        return;
    }

    inline void ConditionList::setSmooth(const Real &x, const bool &is_smooth)
    {
        _list[x].smooth = is_smooth;
        return;
    }

    inline const ConditionList::Condition &ConditionList::operator()(const Real &x) const
    {
        auto it = list.find(x);
        if (it == list.end())
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        return it->second;
    }

    inline const Polynomial operator-(const Polynomial &p)
    {
        Polynomial res(p);
        for (Int i = 0; i <= res.degree; i++)
        {
            res[i] = -res[i];
        }
        return res;
    }

    inline const Polynomial operator+(const Polynomial &p)
    {
        return p;
    }

    inline const Polynomial operator+(const Polynomial &p1, const Polynomial &p2)
    {
        Polynomial res = p1;
        res += p2;
        return res;
    }

    inline const Polynomial operator+(const Polynomial &p, const Real &k)
    {
        Polynomial res = p;
        res += k;
        return res;
    }

    inline const Polynomial operator+(const Real &k, const Polynomial &p)
    {
        Polynomial res = p;
        res += k;
        return res;
    }

    inline const Polynomial operator-(const Polynomial &p1, const Polynomial &p2)
    {
        Polynomial res = p1;
        res -= p2;
        return res;
    }

    inline const Polynomial operator-(const Polynomial &p, const Real &k)
    {
        Polynomial res = p;
        res -= k;
        return res;
    }

    inline const Polynomial operator*(const Polynomial &p1, const Polynomial &p2)
    {
        Polynomial res = p1;
        res *= p2;
        return res;
    }

    inline const Polynomial operator*(const Real &k, const Polynomial &p)
    {
        Polynomial res = p;
        res *= k;
        return res;
    }

    inline const Polynomial operator*(const Polynomial &p, const Real &k)
    {
        Polynomial res = p;
        res *= k;
        return res;
    }

    inline const Polynomial operator/(const Polynomial &p, const Real &k)
    {
        Polynomial res = p;
        res /= k;
        return res;
    }

    inline const Polynomial operator/(const Polynomial &p1, const Polynomial &p2)
    {
        Polynomial res = p1;
        res /= p2;
        return res;
    }

    inline const Polynomial operator%(const Polynomial &p1, const Polynomial &p2)
    {
        Polynomial res = p1;
        res %= p2;
        return res;
    }

    inline const bool isEqual(const Polynomial &p1, const Polynomial &p2, const Real &delta)
    {
        return p1.equal(p2, delta);
    }

    inline const bool operator==(const Polynomial &p1, const Polynomial &p2)
    {
        Int n = std::max(p1.degree, p2.degree);
        for (Int i = 0; i <= n; i++)
        {
            Real tmp = std::max(std::abs(p1[i]), std::abs(p2[i]));
            if (std::abs(p1[i] - p2[i]) > tmp * Trait<float>::epsilon())
            {
                return false;
            }
        }
        return true;
    }

    inline const bool operator!=(const Polynomial &p1, const Polynomial &p2)
    {
        return !(p1 == p2);
    }

    inline std::ostream &operator<<(std::ostream &stream, const Polynomial &p)
    {
        stream << p.print(Trait<float>::min());
        return stream;
    }

    inline const std::vector<Polynomial> fundamentalPolynomial(const std::vector<Real> &x)
    {
        std::vector<Polynomial> res;
        for (Int k = 0; k < x.size(); k++)
        {
            Polynomial p({1});
            for (Int i = 0; i < x.size(); i++)
            {
                Polynomial tmp({-x[i], 1});
                tmp /= (x[k] - x[i]);
                p *= tmp;
            }
            res.push_back(p);
        }
        return res;
    }

    inline const Polynomial newtonFormula(const std::vector<Real> &x, const std::vector<Real> &y)
    {
        const Int n = x.size();
        if (n != y.size())
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        std::vector<std::vector<Real>> table(n);
        for (Int i = 0; i < n; i++)
        {
            table[i].resize(i + 1);
            table[i][0] = y[i];
            for (Int j = 0; j < i; j++)
            {
                table[i][j] = (table[i][j - 1] - table[i - 1][j - 1]) / (x[i] - x[i - j]);
            }
        }
        Polynomial s(0);
        Polynomial tmp({1});
        for (Int i = n - 1; i >= 0; i--)
        {
            s += table[i][i] * tmp;
            tmp *= Polynomial({-x[i], 1});
        }
        return s;
    }

    inline const Polynomial fitPolynomial(const Int &degree, const ConditionList &condition)
    {
        const Int n = condition.list.size();
        Matrix<Real> A = Matrix<Real>::Zero(n, degree + 1);
        Vector<Real> b = Vector<Real>::Zero(n);
        Int i = 0;
        for (auto it = condition.list.begin(); it != condition.list.end(); it++)
        {
            for (auto cond = it->second.y.begin(); cond != it->second.y.end(); cond++, i++)
            {
                Int order = cond->first;
                Real x = 1.0;
                Real c = 1.0;
                for (Int j = 1; j <= order; j++)
                {
                    c = c * j;
                }
                for (Int j = 0; j <= degree - order; j++)
                {
                    A(i, j) = c * x;
                    x = x * it->first;
                    c = c * Real(order + j + 1) / Real(j + 1);
                }
                b(i) = cond->second;
            }
        }
        Vector<Real> x = A.fullPivHouseholderQr().solve(b);
        Polynomial p(degree);
        for (Int i = 0; i <= degree; i++)
        {
            p[i] = x(i);
        }
        return p;
    }

    inline const Polynomial differential(const Polynomial &p)
    {
        return p.differential();
    }

    inline const Polynomial integral(const Polynomial &p, const Real &x)
    {
        return p.integral(x);
    }

    inline Polynomial::Polynomial(const Int &n) : degree(_degree)
    {
        if (n < 0)
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        _degree = n;
        this->coefs.resize(n + 1);
        for (Int i = 0; i <= n; i++)
        {
            this->coefs[i] = 0;
        }
    }

    inline Polynomial::Polynomial(const Polynomial &p) : degree(_degree)
    {
        _degree = p.degree;
        this->coefs.assign(p.coefs.begin(), p.coefs.end());
    }

    inline Polynomial::Polynomial(const std::vector<Real> &coefs) : degree(_degree)
    {
        _degree = coefs.size() - 1;
        this->coefs.assign(coefs.begin(), coefs.end());
    }

    inline const Polynomial Polynomial::differential() const
    {
        Polynomial res(degree - 1);
        for (Int i = 1; i <= degree; i++)
        {
            res[i - 1] = (i * coefs[i]);
        }
        return res;
    }

    inline const Polynomial Polynomial::integral(const Real &x) const
    {
        Polynomial res(degree + 1);
        for (Int i = 1; i <= degree; i++)
        {
            res[i + 1] = (coefs[i] / (i + 1.0));
        }
        res[0] = res[0] - res(x);
        return res;
    }

    inline const std::vector<Real> Polynomial::root(const Real &delta) const
    {
        Int n = degree;
        Matrix<Real> A = Matrix<Real>::Zero(n, n);
        for (Int i = 1; i < n; i++)
        {
            A(i, i - 1) = 1.0;
        }
        for (Int i = 0; i < n; i++)
        {
            A(0, i) = -coefs[n - i - 1] / coefs[n];
        }
        std::vector<Real> res;
        Eigen::EigenSolver<Matrix<Real>> re(A);
        Matrix<Real> D = re.pseudoEigenvalueMatrix();
        for (Int i = 0; i < n; i++)
        {
            if (std::abs(operator()(D(i, i))) < delta)
            {
                res.push_back(D(i, i));
            }
        }
        std::sort(res.begin(), res.end());
        return res;
    }

    inline const bool Polynomial::isRoot(const Real &x, const Real &delta) const
    {
        return std::abs(operator()(x)) < delta;
    }

    inline const bool Polynomial::equal(const Polynomial &p, const Real &delta) const
    {
        int n = std::max(p.degree, degree);
        for (Int i = 0; i <= n; i++)
        {
            if (std::abs(p[i] - this->coefs[i]) > delta)
            {
                return false;
            }
        }
        return true;
    }

    inline std::string Polynomial::print(const Real &precision) const
    {
        std::string s = "";
        bool flag = true;
        for (Int i = degree; i > 0; i--)
        {
            if (std::abs(coefs[i]) > precision)
            {
                s += ((coefs[i] > 0 ? (i == degree ? "" : "+ ") : "- ") + std::to_string(std::abs(coefs[i])));
                s += (" x^" + std::to_string(i) + " ");
                flag = false;
            }
        }
        if (flag)
        {
            s += std::to_string(coefs[0]);
        }
        else if (std::abs(coefs[0]) > precision)
        {
            s += ((coefs[0] > 0 ? "+ " : "- ") + std::to_string(std::abs(coefs[0])));
        }
        return s;
    }

    inline Polynomial &Polynomial::operator=(const Polynomial &p)
    {
        if (&p != this)
        {
            this->_degree = p.degree;
            this->coefs.assign(p.coefs.begin(), p.coefs.end());
        }
        return (*this);
    }

    inline Polynomial &Polynomial::operator=(const Real &k)
    {
        this->_degree = 0;
        this->coefs.resize(1);
        this->coefs[0] = k;
        return (*this);
    }

    inline Polynomial &Polynomial::operator+=(const Polynomial &p)
    {
        _degree = (std::max(degree, p.degree));
        coefs.resize(degree, Trait<Real>::zero());
        for (Int i = 0; i <= p.degree; i++)
        {
            coefs[i] += p[i];
        }
        return (*this);
    }

    inline Polynomial &Polynomial::operator+=(const Real &k)
    {
        coefs[0] += k;
        return (*this);
    }

    inline Polynomial &Polynomial::operator-=(const Polynomial &p)
    {
        _degree = (std::max(degree, p.degree));
        coefs.resize(degree, Trait<Real>::zero());
        for (Int i = 0; i <= p.degree; i++)
        {
            coefs[i] -= p[i];
        }
        return (*this);
    }

    inline Polynomial &Polynomial::operator-=(const Real &k)
    {
        coefs[0] -= k;
        return (*this);
    }

    inline Polynomial &Polynomial::operator*=(const Polynomial &p)
    {
        Polynomial tmp(degree + p.degree);
        for (Int i = 0; i <= degree; i++)
        {
            for (Int j = 0; j <= p.degree; j++)
            {
                tmp[i + j] += (coefs[i] * p[j]);
            }
        }
        (*this) = tmp;
        return (*this);
    }

    inline Polynomial &Polynomial::operator*=(const Real &k)
    {
        for (Int i = 0; i <= degree; i++)
        {
            coefs[i] *= k;
        }
        return (*this);
    }

    inline Polynomial &Polynomial::operator/=(const Real &k)
    {
        for (Int i = 0; i <= degree; i++)
        {
            coefs[i] /= k;
        }
        return (*this);
    }

    inline Polynomial &Polynomial::operator/=(const Polynomial &p)
    {
        Polynomial res = Trait<Polynomial>::identity();
        while (_degree >= p.degree)
        {
            Int i = _degree - p.degree;
            Polynomial x = (coefs[_degree] / p[p.degree]) * Trait<Polynomial>::basis(i);
            res += x;
            (*this) -= (x * p);
            coefs.resize(_degree);
            _degree--;
        }
        (*this) = res;
        return (*this);
    }

    inline Polynomial &Polynomial::operator%=(const Polynomial &p)
    {
        Polynomial res = Trait<Polynomial>::identity();
        while (_degree >= p.degree)
        {
            Int i = _degree - p.degree;
            Polynomial x = (coefs[_degree] / p[p.degree]) * Trait<Polynomial>::basis(i);
            res += x;
            (*this) -= (x * p);
            coefs.resize(_degree);
            _degree--;
        }
        return (*this);
    }

    inline const Real Polynomial::operator()(const Real &x) const
    {
        Real res = 0.0;
        Real k = 1.0;
        for (Int i = 0; i <= degree; i++, k *= x)
        {
            res += (k * this->coefs[i]);
        }
        return res;
    }

    inline const Real Polynomial::operator[](const Int &n) const
    {
        if (n > degree || n < 0)
        {
            return 0.0;
        }
        return this->coefs[n];
    }

    inline Real &Polynomial::operator[](const Int &n)
    {
        if (n < 0 || n > degree)
        {
            printf("Error at: file %s line %d.", __FILE__, __LINE__);
            exit(0);
        }
        return this->coefs[n];
    }

    inline const Polynomial Trait<Polynomial>::zero()
    {
        return Polynomial(Trait<Real>::zero());
    }

    inline const Polynomial Trait<Polynomial>::identity()
    {
        return Polynomial(std::vector<Real>({Trait<Real>::identity()}));
    }

    inline const Polynomial Trait<Polynomial>::basis(const size_t &n)
    {
        Polynomial p(n);
        p[n] = Trait<Real>::identity();
        return p;
    }

    inline OrthogonalPolynomial::OrthogonalPolynomial(const Type &type) : weight(_weight), poly(_poly), range(_range)
    {
        const Polynomial X = Polynomial(std::vector<Real>({0.0, 1.0}));
        switch (type)
        {
        case Type::Legendre:
            _weight = [](const Real &x) -> Real
            { return 1.0; };
            _poly.push_back(Polynomial(std::vector<Real>({1.0})));
            _poly.push_back(Polynomial(std::vector<Real>({0.0, 1.0})));
            next = [&X = std::as_const(X)](const std::vector<Polynomial> &list) -> Polynomial
            {
                const Int n = list.size() - 1;
                return ((2 * n + 1) * X * list[n] - n * list[n - 1]) / (n + 1);
            };
            _range = std::make_pair(-1.0, 1.0);
            break;
        case Type::ChebyshevOfFirstClass:
            _weight = [](const Real &x) -> Real
            { return 1.0 / std::sqrt(1 - x * x); };
            _poly.push_back(Polynomial(std::vector<Real>({1.0})));
            _poly.push_back(Polynomial(std::vector<Real>({0.0, 1.0})));
            next = [&X = std::as_const(X)](const std::vector<Polynomial> &list) -> Polynomial
            {
                const Int n = list.size() - 1;
                return 2 * X * list[n] - list[n - 1];
            };
            _range = std::make_pair(-1.0, 1.0);
            break;
        case Type::ChebyshevOfSecondClass:
            _weight = [](const Real &x) -> Real
            { return std::sqrt(1 - x * x); };
            _poly.push_back(Polynomial(std::vector<Real>({1.0})));
            _poly.push_back(Polynomial(std::vector<Real>({0.0, 2.0})));
            next = [&X = std::as_const(X)](const std::vector<Polynomial> &list) -> Polynomial
            {
                const Int n = list.size() - 1;
                return 2 * X * list[n] - list[n - 1];
            };
            _range = std::make_pair(-1.0, 1.0);
            break;
        case Type::Laguerre:
            _weight = [](const Real &x) -> Real
            { return std::exp(-x); };
            _poly.push_back(Polynomial(std::vector<Real>({1.0})));
            _poly.push_back(Polynomial(std::vector<Real>({1.0, -1.0})));
            next = [&X = std::as_const(X)](const std::vector<Polynomial> &list) -> Polynomial
            {
                const Int n = list.size() - 1;
                return ((2 * n + 1 - X) * list[n] - n * list[n - 1]) / (n + 1);
            };
            _range = std::make_pair(0.0, INFINITY);
            break;
        case Type::Hermite:
            _weight = [](const Real &x) -> Real
            { return std::exp(-x * x / 2) / std::sqrt(2 * std::numbers::pi); };
            _poly.push_back(Polynomial(std::vector<Real>({1.0})));
            _poly.push_back(Polynomial(std::vector<Real>({0.0, 1.0})));
            next = [&X = std::as_const(X)](const std::vector<Polynomial> &list) -> Polynomial
            {
                const Int n = list.size() - 1;
                return X * list[n] - n * list[n - 1];
            };
            _range = std::make_pair(-INFINITY, INFINITY);
            break;
        default:
            break;
        }
    }

    inline const Polynomial &OrthogonalPolynomial::operator()(const Int &index)
    {
        for (Int i = poly.size(); i <= index; i++)
        {
            _poly.push_back(next(poly));
        }
        return poly[index];
    }
};

#endif
