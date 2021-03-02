#ifndef LAZYCALLBACK_H
#define LAZYCALLBACK_H

#include <ilcplex/ilocplex.h>
#include <limits>
#include "data.h"

class lazyCallback : public IloCplex::LazyConstraintCallbackI {
    public:
        lazyCallback(IloEnv Env, const IloArray<IloBoolVarArray> &X,int Dim);
        IloCplex::CallbackI *duplicateCallback() const;
        void main();

    private:
        IloArray<IloBoolVarArray> x;
        IloEnv env;
        int dim;
};

#endif