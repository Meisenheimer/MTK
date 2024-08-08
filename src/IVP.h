#ifndef MTK_IVP_H
#define MTK_IVP_H

#include "Optimizer.h"

static_assert(__cplusplus >= 201700, "C++17 or higher is required.");

namespace mtk
{
    enum class LMM
    {
        ForwardEuler = 11,
        BackwardEuler = 21,
        Trapezoidal = 31,
        Midpoint = 41,
    };

    enum class RKM
    {
        HeunThirdOrder = 12,
        ClassicalFourthOrder = 22,
    };

    template <typename Real>
    class IVP;

    template <typename Real>
    class IVP
    {
    protected:
        std::function<const Vector<Var<Real>>(const Vector<Var<Real>> &, const Real &)> _f;
        std::vector<std::pair<Vector<Real>, Real>> _res;
        Optimizer<Real> _opt;

    public:
        const std::function<const Vector<Var<Real>>(const Vector<Var<Real>> &, const Real &)> &f;
        const std::vector<std::pair<Vector<Real>, Real>> &res;
        const Optimizer<Real> &opt;

    public:
        IVP();
        IVP(const IVP &ivp);

        void setRHS(const std::function<const Vector<Var<Real>>(const Vector<Var<Real>> &, const Real &)> &f);
        void setInitValue(const std::vector<std::pair<Vector<Real>, Real>> &init_value);

        Optimizer<Real> &getOpt();

        const Vector<Real> operator()(const Real &t) const;

        virtual bool isExplicit() const = 0;
        virtual bool isImplicit() const = 0;
        virtual void solve(const Real &end, const Real &k = 0.0) = 0;

        IVP &operator=(const IVP &ivp);
    };

    template <typename Real>
    class LM : public IVP<Real>
    {
    private:
        std::vector<Real> _alpha;
        std::vector<Real> _beta;

    public:
        const std::vector<Real> &alpha;
        const std::vector<Real> &beta;

    public:
        LM();
        LM(const LM &lmm);

        void setMethod(const LMM &name);
        void setMethod(const std::vector<Real> &alpha, const std::vector<Real> &beta);
        bool isExplicit() const override;
        bool isImplicit() const override;
        void solve(const Real &end, const Real &k) override;

        LM &operator=(const LM &lmm);
    };

    template <typename Real>
    class RK : public IVP<Real>
    {
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

        void setMethod(const RKM &name);
        void setMethod(const Matrix<Real> &a, const Vector<Real> &b, const Vector<Real> &c);
        bool isExplicit() const override;
        bool isImplicit() const override;
        void solve(const Real &end, const Real &k) override;

        RK &operator=(const RK &rk);
    };
};

#include "IVP.hpp"

#endif