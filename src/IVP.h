#ifndef MTK_IVP_H
#define MTK_IVP_H

#include "Config.h"
#include "Optimizer.h"

namespace mtk
{
    class IVP;

    class IVP
    {
    public:
        struct Method
        {
        };

    protected:
        Func<const Vector, const Vector &, const Real &> _f;
        List<Pair<Vector, Real>> _res;
        Optimizer _opt;

    public:
        const Func<const Vector, const Vector &, const Real &> &f;
        const List<Pair<Vector, Real>> &res;

    public:
        IVP();

        Optimizer &opt();
        const Optimizer &opt() const;
        void setRHS(const Func<const Vector, const Vector &, const Real &> &f);
        void setInitValue(const List<Pair<Vector, Real>> &init_value);

        const Vector operator()(const Real &t) const;

        virtual void solve(const Real &end, const Real &k = 0.0) = 0;
    };

    class LMM : public IVP
    {
    public:
        struct Method : IVP::Method
        {
            static constexpr Int ForwardEuler = 11;
            static constexpr Int BackwardEuler = 21;
            static constexpr Int Trapezoidal = 31;
            static constexpr Int Midpoint = 41;
        };

    private:
        List<Real> alpha;
        List<Real> beta;

    public:
        void setMethod(const Int &name);
        void setMethod(const List<Real> &alpha, const List<Real> &beta);
        void solve(const Real &end, const Real &k);
    };

    class RK : public IVP
    {
    public:
        struct Method : IVP::Method
        {
            static constexpr Int HeunThirdOrder = 12;
            static constexpr Int ClassicalFourthOrder = 22;
        };

    private:
        Matrix a;
        Vector b;
        Vector c;

    public:
        void setMethod(const Int &name);
        void setMethod(const Matrix &a, const Vector &b, const Vector &c);
        void solve(const Real &end, const Real &k);
    };
};

#include "IVP.hpp"

#endif