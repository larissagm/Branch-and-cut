#include "lazyCallback.h"
#include "maxbackMincut.h"
#include <stdlib.h>
#include <vector>
using namespace std;

lazyCallback::lazyCallback(IloEnv Env, const IloArray<IloBoolVarArray> &X,int Dim):IloCplex::LazyConstraintCallbackI(env)
{
    env = Env;
    x = X;
    dim = Dim;
}

IloCplex::CallbackI* lazyCallback::duplicateCallback() const{
    return new (env) lazyCallback(env,x,dim);
}

void lazyCallback::main(){

    IloArray<IloNumArray> x2(env, dim);
    vector<vector<double>> x3(dim, vector<double>(dim));
    
    for (int i=0; i<dim; i++){

        x2[i] = IloNumArray(env, dim);
        getValues(x2[i], x[i]);
    }

    for (int i=0; i<dim; i++){
        for (int j=i+1; j<dim; j++){
            x3[i][j] = floor(x2[i][j]+0.00001) + floor(x2[j][i]+0.00001);
        }
    }

    vector<vector<int>> subtours = maxback(x3, dim);

    if (subtours.empty()) return;

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
    
    /*for (int i=0;i<subtours.size();i++){
        for (int j=0;j<subtours[i].size();j++){
            printf("%d ", subtours[i][j]);
        }
        printf("\n");
    }
    printf("\n");*/

	/*int t=0;
	vector<int> ch(dim,0);
    vector<vector<int>> subtours;
	for (int i=0;i<dim;i++){
		if (ch[i]==0){
			vector<int> init = {i};
			int k=i;
			while (ch[i]==0){
				for (int j=0;j<dim;j++){
                    if (x3[k][j]==1){
						ch[j]=1;
						k=j;
                        t++;
						init.push_back(j);
						break;
					}
				}
			}
			subtours.push_back(init);
            if (t==dim) break;
		}
	}
    for (int i=0;i<subtours.size();i++){
        for (int j=0;j<subtours[i].size();j++){
            cout<<subtours[i][j]<<" ";
        }
        cout<<"\n";
    }*/

}
