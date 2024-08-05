#ifndef MTK_POLYNOMIAL_H
#define MTK_POLYNOMIAL_H

#include "Trait.h"

static_assert(__cplusplus >= 201700, "C++17 or higher is required.");

namespace mtk
{
    using Real = long double;

    class ConditionList;
    class Polynomial;
    template <>
    class Trait<Polynomial>;
    class OrthonormalPolynomial;

    const Polynomial operator-(const Polynomial &p);
    const Polynomial operator+(const Polynomial &p);
    const Polynomial operator+(const Polynomial &p1, const Polynomial &p2);
    const Polynomial operator+(const Polynomial &p, const Real &k);
    const Polynomial operator+(const Real &k, const Polynomial &p);
    const Polynomial operator-(const Polynomial &p1, const Polynomial &p2);
    const Polynomial operator-(const Polynomial &p, const Real &k);
    const Polynomial operator*(const Polynomial &p1, const Polynomial &p2);
    const Polynomial operator*(const Real &k, const Polynomial &p);
    const Polynomial operator*(const Polynomial &p, const Real &k);
    const Polynomial operator/(const Polynomial &p, const Real &k);
    const Polynomial operator/(const Polynomial &p1, const Polynomial &p2);
    const Polynomial operator%(const Polynomial &p1, const Polynomial &p2);

    const bool isEqual(const Polynomial &p1, const Polynomial &p2, const Real &delta = Trait<Real>::epsilon());

    const bool operator==(const Polynomial &p1, const Polynomial &p2);
    const bool operator!=(const Polynomial &p1, const Polynomial &p2);

    std::ostream &operator<<(std::ostream &stream, const Polynomial &p);

    const std::vector<Polynomial> fundamentalPolynomial(const std::vector<Real> &x);
    const Polynomial newtonFormula(const std::vector<Real> &x, const std::vector<Real> &y);
    const Polynomial fitPolynomial(const size_t &degree, const ConditionList &cond);
    const Polynomial differential(const Polynomial &p);
    const Polynomial integral(const Polynomial &p, const Real &x);

    class ConditionList
    {
    public:
        struct Condition
        {
            std::map<size_t, Real> y;
            bool knot;
            bool smooth;

            Condition();
            // Condition(const Condition &c);
            Condition(const bool &is_knot, const bool &is_smooth);
            Condition(const std::map<size_t, Real> &y, const bool &is_knot = true, const bool &is_smooth = false);
        };

    private:
        std::map<Real, Condition> _list;

    public:
        const std::map<Real, Condition> &list;

    public:
        ConditionList();
        ConditionList(const std::map<Real, Condition> &init_list);

        void setCondition(const Real &x, const Real &y, const size_t &order = 0);
        void setKnot(const Real &x, const bool &is_knot = true);
        void setSmooth(const Real &x, const bool &is_smooth = true);

        const Condition &operator()(const Real &x) const;
    };

    class Polynomial
    {
    private:
        std::vector<Real> _coefs;
        size_t _degree;

    public:
        const std::vector<Real> &coefs;
        const size_t &degree;

    public:
        Polynomial(const size_t &n = 0);
        Polynomial(const Polynomial &p);
        Polynomial(const std::vector<Real> &coefs);

        const Polynomial differential() const;
        const Polynomial integral(const Real &x) const;
        const std::vector<Real> root(const Real &delta = Trait<float>::epsilon()) const;
        const bool isRoot(const Real &x, const Real &delta = Trait<float>::epsilon()) const;

        const bool equal(const Polynomial &p, const Real &delta = Trait<Real>::epsilon()) const;

        std::string print(const Real &precision = Trait<float>::epsilon()) const;

        Polynomial &operator=(const Polynomial &p);
        Polynomial &operator=(const Real &p);
        Polynomial &operator+=(const Polynomial &p);
        Polynomial &operator+=(const Real &k);
        Polynomial &operator-=(const Polynomial &p);
        Polynomial &operator-=(const Real &k);
        Polynomial &operator*=(const Polynomial &p);
        Polynomial &operator*=(const Real &k);
        Polynomial &operator/=(const Real &k);
        Polynomial &operator/=(const Polynomial &p);
        Polynomial &operator%=(const Polynomial &p);

        const Real operator()(const Real &x) const;
        const Real operator[](const size_t &degree) const;
        Real &operator[](const size_t &degree);
    };

    template <>
    class Trait<Polynomial>
    {
    public:
        Trait() = delete;

        static const Polynomial zero();
        static const Polynomial identity();
        static const Polynomial basis(const size_t &n);
    };

    class OrthogonalPolynomial
    {
    public:
        enum class Type
        {
            Legendre,
            ChebyshevOfFirstClass,
            ChebyshevOfSecondClass,
            Laguerre,
            Hermite,
        };

    private:
        std::function<const Polynomial(const std::vector<Polynomial> &)> next;

    private:
        std::function<const Real(const Real &)> _weight;
        std::vector<Polynomial> _poly;
        std::pair<Real, Real> _range;

    public:
        const std::function<const Real(const Real &)> &weight;
        const std::vector<Polynomial> &poly;
        const std::pair<Real, Real> &range;

    public:
        OrthogonalPolynomial(const Type &type);
        OrthogonalPolynomial(const OrthogonalPolynomial &op);

        const Polynomial &operator()(const size_t &index);

        OrthogonalPolynomial &operator=(const OrthogonalPolynomial &op);
    };
};

#include "Polynomial.hpp"

#endif
