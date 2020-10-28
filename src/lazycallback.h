#ifndef LAZYCALLBACK_H
#define LAZYCALLBACK_H

#include <ilcplex/ilocplex.h>
#include <bits/stdc++.h>


class lazyCallback : public IloCplex::LazyConstraintCallbackI {
    public:
        lazyCallback(IloEnv env_ref, const IloArray<IloBoolVarArray> &x_ref,int dim_ref);
        IloCplex::CallbackI *duplicateCallback() const;
        void main();

    private:
        IloArray<IloBoolVarArray> x;
        IloEnv env;
        int dim;
};

#endif