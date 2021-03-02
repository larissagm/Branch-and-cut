#include "userCallback.h"
#include "maxbackMincut.h"
#include <stdlib.h>
#include <vector>
using namespace std;

userCallback::userCallback(IloEnv Env, const IloArray<IloBoolVarArray> &X,int Dim):IloCplex::UserCutCallbackI(env)
{
    env = Env;
    x = X;
    dim = Dim;
}

IloCplex::CallbackI* userCallback::duplicateCallback() const{
    return new (env) userCallback(env,x,dim);
}

void userCallback::main(){

    if (it > 100) return;
    it++;

    IloArray<IloNumArray> x2(env, dim);
    vector<vector<double>> x3(dim, vector<double>(dim));
    
    for (int i=0; i<dim; i++){

        x2[i] = IloNumArray(env,dim);
        getValues(x2[i], x[i]);
    }

    for (int i=0; i<dim; i++){
        for (int j=i+1; j<dim; j++){
            x3[i][j] = x2[i][j] + x2[j][i];
        }
    }

    vector<vector<int>> subtours = maxback(x3, dim);

    if (subtours.empty() && getCurrentNodeDepth()<=7) { 
        subtours = mincut(x3, dim);
    }

    for (int i=0; i<subtours.size(); i++){
        IloExpr sum(env);
        
        for (int j=0; j<subtours[i].size()-1; j++){
            for (int k=j+1; k<subtours[i].size(); k++){
                sum+= x[subtours[i][j]][subtours[i][k]] + x[subtours[i][k]][subtours[i][j]];
            }
        }
        sum-= subtours[i].size();
        add(sum <= - 1);
    }
}
