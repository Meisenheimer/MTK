#ifndef MTK_IVP_H
#define MTK_IVP_H

#include "Optimizer.h"

static_assert(__cplusplus >= 201700, "C++17 or higher is required.");

namespace mtk
{
    using Real = long double;

    class IVP;

    class IVP
    {
    public:
        struct Method
        {
        };

    protected:
        std::function<const Vector<Real>(const Vector<Real> &, const Real &)> _f;
        std::vector<std::pair<Vector<Real>, Real>> _res;
        Optimizer<Real> _opt;

    public:
        const std::function<const Vector<Real>(const Vector<Real> &, const Real &)> &f;
        const std::vector<std::pair<Vector<Real>, Real>> &res;

    public:
        IVP();

        Optimizer<Real> &opt();
        const Optimizer<Real> &opt() const;
        void setRHS(const std::function<const Vector<Real>(const Vector<Real> &, const Real &)> &f);
        void setInitValue(const std::vector<std::pair<Vector<Real>, Real>> &init_value);

        const Vector<Real> operator()(const Real &t) const;

        virtual void solve(const Real &end, const Real &k = 0.0) = 0;
    };

    class LMM : public IVP
    {
    public:
        struct Method : IVP::Method
        {
            static constexpr size_t ForwardEuler = 11;
            static constexpr size_t BackwardEuler = 21;
            static constexpr size_t Trapezoidal = 31;
            static constexpr size_t Midpoint = 41;
        };

    private:
        std::vector<Real> alpha;
        std::vector<Real> beta;

    public:
        void setMethod(const size_t &name);
        void setMethod(const std::vector<Real> &alpha, const std::vector<Real> &beta);
        void solve(const Real &end, const Real &k);
    };

    class RK : public IVP
    {
    public:
        struct Method : IVP::Method
        {
            static constexpr size_t HeunThirdOrder = 12;
            static constexpr size_t ClassicalFourthOrder = 22;
        };

    private:
        Matrix<Real> a;
        Vector<Real> b;
        Vector<Real> c;

    public:
        void setMethod(const size_t &name);
        void setMethod(const Matrix<Real> &a, const Vector<Real> &b, const Vector<Real> &c);
        void solve(const Real &end, const Real &k);
    };
};

#include "IVP.hpp"

#endif