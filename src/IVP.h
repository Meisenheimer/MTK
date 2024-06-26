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
        Func<Vector, Vector, Real> _f;
        List<Pair<Vector, Real>> _res;
        Optimizer _opt;

    public:
        const Func<Vector, Vector, Real> &f;
        const List<Pair<Vector, Real>> &res;

    public:
        IVP();

        Optimizer &opt();
        const Optimizer &opt() const;
        void setRHS(const Func<Vector, Vector, Real> &f);
        void setInitValue(const List<Pair<Vector, Real>> &init_value);

        const Vector operator()(const Real &t) const;

        virtual void solve(const Real &end, const Real &k = 0.0) = 0;
    };

    class LMM : public IVP
    {
    public:
        struct Method : IVP::Method
        {
            static const Int ForwardEuler;
            static const Int BackwardEuler;
            static const Int Trapezoidal;
            static const Int Midpoint;
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
            static const Int HeunThirdOrder;
            static const Int ClassicalFourthOrder;
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