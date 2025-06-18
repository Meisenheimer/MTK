#ifndef MTK_POLYNOMIAL_H
#define MTK_POLYNOMIAL_H

#include "Trait.h"

static_assert(__cplusplus >= 201700, "C++17 or higher is required.");

namespace mtk
{
    template <typename Real>
    class ConditionList;
    template <typename Real>
    class Polynomial;
    template <typename Real>
    class Trait<Polynomial<Real>>;
    template <typename Real>
    class OrthonormalPolynomial;

    template <typename Real>
    const Polynomial<Real> operator-(const Polynomial<Real> &p);
    template <typename Real>
    const Polynomial<Real> operator+(const Polynomial<Real> &p);
    template <typename Real1, typename Real2>
    const Polynomial<std::common_type_t<Real1, Real2>> operator+(const Polynomial<Real1> &p1, const Polynomial<Real2> &p2);
    template <typename Real, typename Type>
    const Polynomial<Real> operator+(const Polynomial<Real> &p, const Type &k);
    template <typename Real, typename Type>
    const Polynomial<Real> operator+(const Type &k, const Polynomial<Real> &p);
    template <typename Real1, typename Real2>
    const Polynomial<std::common_type_t<Real1, Real2>> operator-(const Polynomial<Real1> &p1, const Polynomial<Real2> &p2);
    template <typename Real, typename Type>
    const Polynomial<Real> operator-(const Polynomial<Real> &p, const Type &k);
    template <typename Real, typename Type>
    const Polynomial<Real> operator-(const Type &k, const Polynomial<Real> &p);
    template <typename Real1, typename Real2>
    const Polynomial<std::common_type_t<Real1, Real2>> operator*(const Polynomial<Real1> &p1, const Polynomial<Real2> &p2);
    template <typename Real, typename Type>
    const Polynomial<Real> operator*(const Type &k, const Polynomial<Real> &p);
    template <typename Real, typename Type>
    const Polynomial<Real> operator*(const Polynomial<Real> &p, const Type &k);
    template <typename Real, typename Type>
    const Polynomial<Real> operator/(const Polynomial<Real> &p, const Type &k);
    template <typename Real1, typename Real2>
    const Polynomial<std::common_type_t<Real1, Real2>> operator/(const Polynomial<Real1> &p1, const Polynomial<Real2> &p2);
    template <typename Real1, typename Real2>
    const Polynomial<std::common_type_t<Real1, Real2>> operator%(const Polynomial<Real1> &p1, const Polynomial<Real2> &p2);

    template <typename Real>
    const bool isEqual(const Polynomial<Real> &p1, const Polynomial<Real> &p2, const Real &delta = Trait<Real>::epsilon());

    template <typename Real>
    const bool operator==(const Polynomial<Real> &p1, const Polynomial<Real> &p2);
    template <typename Real>
    const bool operator!=(const Polynomial<Real> &p1, const Polynomial<Real> &p2);

    template <typename Real>
    std::ostream &operator<<(std::ostream &stream, const Polynomial<Real> &p);

    template <typename Real>
    const std::vector<Polynomial<Real>> fundamentalPolynomial(const std::vector<Real> &x);
    template <typename Real>
    const Polynomial<Real> newtonFormula(const std::vector<Real> &x, const std::vector<Real> &y);
    template <typename Real>
    const Polynomial<Real> fitPolynomial(const size_t &degree, const ConditionList<Real> &cond);
    template <typename Real>
    const Polynomial<Real> differential(const Polynomial<Real> &p);
    template <typename Real>
    const Polynomial<Real> integral(const Polynomial<Real> &p, const Real &x);

    template <typename Real>
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

    template <typename Real>
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

    template <typename Real>
    class Trait<Polynomial<Real>>
    {
    public:
        Trait() = delete;

        static const Polynomial<Real> zero();
        static const Polynomial<Real> identity();
        static const Polynomial<Real> basis(const size_t &n);
    };

    template <typename Real>
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
        std::function<const Polynomial<Real>(const std::vector<Polynomial<Real>> &)> next;

    private:
        std::function<const Real(const Real &)> _weight;
        std::vector<Polynomial<Real>> _poly;
        std::pair<Real, Real> _range;

    public:
        const std::function<const Real(const Real &)> &weight;
        const std::vector<Polynomial<Real>> &poly;
        const std::pair<Real, Real> &range;

    public:
        OrthogonalPolynomial(const Type &type);
        OrthogonalPolynomial(const OrthogonalPolynomial &op);

        const Polynomial<Real> &operator()(const size_t &index);

        OrthogonalPolynomial &operator=(const OrthogonalPolynomial &op);
    };
};

#include "Polynomial.hpp"

#endif
