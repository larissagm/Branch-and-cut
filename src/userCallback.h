#ifndef USERCALLBACK_H
#define USERCALLBACK_H

#include <ilcplex/ilocplex.h>
#include <limits>
#include "data.h"

class userCallback : public IloCplex::UserCutCallbackI {
    public:
        userCallback(IloEnv Env, const IloArray<IloBoolVarArray> &X,int Dim);
        IloCplex::CallbackI *duplicateCallback() const;
        void main();

    private:
        IloArray<IloBoolVarArray> x;
        IloEnv env;
        int dim, it = 0;
};

#endif