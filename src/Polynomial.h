#ifndef MTK_POLYNOMIAL_H
#define MTK_POLYNOMIAL_H

#include "Config.hpp"

namespace mtk
{
    class ConditionList;
    class Polynomial;
    class OrthonormalPolynomial;

    const Polynomial operator-(const Polynomial &p);
    const Polynomial operator+(const Polynomial &p);
    const Polynomial operator+(const Polynomial &p1, const Polynomial &p2);
    const Polynomial operator+(const Polynomial &p, const Real &k);
    const Polynomial operator-(const Polynomial &p1, const Polynomial &p2);
    const Polynomial operator-(const Polynomial &p, const Real &k);
    const Polynomial operator*(const Polynomial &p1, const Polynomial &p2);
    const Polynomial operator*(const Real &k, const Polynomial &p);
    const Polynomial operator*(const Polynomial &p, const Real &k);
    const Polynomial operator/(const Polynomial &p, const Real &k);
    const Polynomial operator/(const Polynomial &p1, const Polynomial &p2);
    const Polynomial operator%(const Polynomial &p1, const Polynomial &p2);

    const Bool isEqual(const Polynomial &p1, const Polynomial &p2, const Real &delta = EPS<Real>);

    const Bool operator==(const Polynomial &p1, const Polynomial &p2);
    const Bool operator!=(const Polynomial &p1, const Polynomial &p2);

    std::ostream &operator<<(std::ostream &stream, const Polynomial &p);

    const List<Polynomial> fundamentalPolynomial(const List<Real> &x);
    const Polynomial newtonFormula(const List<Real> &x, const List<Real> &y);
    const Polynomial fitPolynomial(const Int &degree, const ConditionList &cond);
    const Polynomial differential(const Polynomial &p);
    const Polynomial integral(const Polynomial &p, const Real &x);

    class ConditionList
    {
    public:
        struct Condition
        {
            Map<Int, Real> y;
            Bool knot;
            Bool smooth;

            Condition();
            // Condition(const Condition &c);
            Condition(const Bool &is_knot, const Bool &is_smooth);
            Condition(const Map<Int, Real> &y, const Bool &is_knot = true, const Bool &is_smooth = false);
        };

    private:
        Map<Real, Condition> _list;

    public:
        const Map<Real, Condition> &list;

    public:
        ConditionList();
        ConditionList(const Map<Real, Condition> &init_list);

        void setCondition(const Real &x, const Real &y, const Int &order = 0);
        void setKnot(const Real &x, const Bool &is_knot = true);
        void setSmooth(const Real &x, const Bool &is_smooth = true);

        const Condition &operator()(const Real &x) const;
    };

    class Polynomial
    {
    private:
        List<Real> coefs;
        Int _degree;

    public:
        const Int &degree;

    public:
        Polynomial(const Int &n = 0);
        Polynomial(const Polynomial &p);
        Polynomial(const List<Real> &coefs);

        const Polynomial differential() const;
        const Polynomial integral(const Real &x) const;
        const List<Real> root(const Real &delta = EPS<float>) const;
        const Bool isRoot(const Real &x, const Real &delta = EPS<float>) const;

        const Bool equal(const Polynomial &p, const Real &delta = EPS<Real>) const;

        String print(const Real &precision = EPS<float>) const;

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
        const Real operator[](const Int &degree) const;
        Real &operator[](const Int &degree);
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
        Func<Real, Real> _weight;
        List<Polynomial> _poly;
        Func<Polynomial, List<Polynomial>> next;
        std::pair<Real, Real> _range;

    public:
        const Func<Real, Real> &weight;
        const List<Polynomial> &poly;
        const std::pair<Real, Real> &range;

        OrthogonalPolynomial(const Type &type);

        const Polynomial &operator()(const Int &index);
    };
};

#include "Polynomial.hpp"

#endif
