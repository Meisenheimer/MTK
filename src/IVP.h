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
    protected:
        std::function<const Vector<Real>(const Vector<Real> &, const Real &)> _f;
        std::vector<std::pair<Vector<Real>, Real>> _res;
        Optimizer<Real> _opt;

    public:
        const std::function<const Vector<Real>(const Vector<Real> &, const Real &)> &f;
        const std::vector<std::pair<Vector<Real>, Real>> &res;
        const Optimizer<Real> &opt;

    public:
        IVP();
        IVP(const IVP &ivp);

        void setRHS(const std::function<const Vector<Real>(const Vector<Real> &, const Real &)> &f);
        void setInitValue(const std::vector<std::pair<Vector<Real>, Real>> &init_value);

        const Vector<Real> operator()(const Real &t) const;

        virtual void solve(const Real &end, const Real &k = 0.0) = 0;

        IVP &operator=(const IVP &ivp);
    };

    class LMM : public IVP
    {
    public:
        static constexpr size_t ForwardEuler = 11;
        static constexpr size_t BackwardEuler = 21;
        static constexpr size_t Trapezoidal = 31;
        static constexpr size_t Midpoint = 41;

    private:
        std::vector<Real> _alpha;
        std::vector<Real> _beta;

    public:
        const std::vector<Real> &alpha;
        const std::vector<Real> &beta;

    public:
        LMM();
        LMM(const LMM &lmm);

        void setMethod(const size_t &name);
        void setMethod(const std::vector<Real> &alpha, const std::vector<Real> &beta);
        void solve(const Real &end, const Real &k);

        LMM &operator=(const LMM &lmm);
    };

    class RK : public IVP
    {
    public:
        static constexpr size_t HeunThirdOrder = 12;
        static constexpr size_t ClassicalFourthOrder = 22;

    private:
        Matrix<Real> _a;
        Vector<Real> _b;
        Vector<Real> _c;

    public:
        const Matrix<Real> &a;
        const Vector<Real> &b;
        const Vector<Real> &c;

    public:
        RK();
        RK(const RK &rk);

        void setMethod(const size_t &name);
        void setMethod(const Matrix<Real> &a, const Vector<Real> &b, const Vector<Real> &c);
        void solve(const Real &end, const Real &k);

        RK &operator=(const RK &rk);
    };
};

#include "IVP.hpp"

#endif