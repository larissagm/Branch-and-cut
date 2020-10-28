#include "lazycallback.h"
#include <bits/stdc++.h>
using namespace std;

lazyCallback::lazyCallback(IloEnv env_ref, const IloArray<IloBoolVarArray> &x_ref,int dim_ref):IloCplex::LazyConstraintCallbackI(env)
{
    env = env_ref;
    x = x_ref;
    dim = dim_ref;
}

IloCplex::CallbackI* lazyCallback::duplicateCallback() const{
    return new (env) lazyCallback(env,x,dim);
}

void lazyCallback::main(){
    
    /*printf("main1\n");
    for (int i=0;i<dim;i++){
        for (int j=0;j<dim;j++){
            if (getValue(x[i][j])>0.9){
                printf("1 ");
            }
            else printf("0 ");
        }
        printf("\n");
    }
    printf("\n");*/
	int a=-1;
	vector<int> ch(dim,0);
    vector<vector<int>> subtours;
	for (int i=0;i<dim;i++){
		if (ch[i]==0){
			vector<int> init = {i};
			int k=i;
			a++;
            //printf("in %d\n",i);
			while (ch[i]==0){
				for (int j=0;j<dim;j++){
                    //printf("%d ",getValue(x[k][j]));
					if (getValue(x[k][j])>0.9){
                        //printf("%d ",j);
						ch[j]=1;
						k=j;
						init.push_back(j);
						break;
					}
				}
                //printf("\n");
			}
            /*printf("out\n");
            for (int w=0;w<init.size();w++){
                printf("%d ",init[w]);
            }
            printf("\n");*/
			subtours.push_back(init);
		}
	}
    //printf("main2\n");
    if (subtours.size()==1) return;
    for (int i=0;i<subtours.size();i++){
        IloExpr sum(env);
        sum-=subtours[i].size();
        for (int j=0;j<subtours[i].size()-1;j++){
            sum+=x[subtours[i][j]][subtours[i][j+1]];
        }
        add(sum<= -2);
    }
    /*for (int i=0;i<subtours.size();i++){
        for (int j=0;j<subtours[i].size();j++){
            printf("%d ", subtours[i][j]);
        }
        printf("\n");
    }
    printf("\n");*/
}