#include <ilcplex/ilocplex.h>
#include "lazycallback.h"
#include "data.h"
#include <stdlib.h>
#include <stdio.h>

void solve(Data &data);

int main(int argc, char** argv){

    //Data data(argv[1]);
    Data data(argc, argv[1]);

    solve(data);
    
    return 0;
}

void solve(Data &data){
    IloEnv env;
    IloModel modelo(env);

    data.readData();

    double **cost = data.getMatrixCost();

    // variavel x_ij = 1 se o arco for escolhido
    IloArray<IloBoolVarArray> x(env, data.getDimension());
    
    //adiciona a variavel x ao modelo
    for (int i=0;i<data.getDimension();i++){
        IloBoolVarArray vetor(env,data.getDimension());
        x[i]=vetor;
        for (int j=0;j<data.getDimension();j++){
            char name[100];
            sprintf(name, "X(%d,%d)",i,j);
            x[i][j].setName(name);
            modelo.add(x[i][j]);
        }
    }

    //objetivo
    IloExpr sum(env);
    for (int i=0;i<data.getDimension();i++){
        for(int j=0;j<data.getDimension();j++){
            sum+=cost[i][j]*x[i][j];
        }
    }
    modelo.add(IloMinimize(env,sum));

    //restricoes

    for (int i=0; i<data.getDimension();i++){
        IloExpr sum1(env);
        IloExpr sum2(env);
        for (int j=0; j<data.getDimension();j++){
            //if (i!=j){
                sum1+=x[i][j];
                sum2+=x[j][i];
            //}
            //else modelo.add(x[i][j]==0);
        }
        modelo.add(sum1==1);
        modelo.add(sum2==1);
    }

    //resolve o modelo
    IloCplex tsp(modelo);
    tsp.exportModel("modelo.lp");

    lazyCallback *lazy = new (env) lazyCallback(env,x,data.getDimension());
    tsp.use(lazy);
    //tsp.use(LazyConstraintCallbackI) 

    /*tsp.setParam(IloCplex::TiLim, 60);
    tsp.setParam(IloCplex::Param::MIP::Interval, 1);
    tsp.setParam(IloCplex::Param::MIP::Display, 5);*/

    tsp.setParam(IloCplex::TiLim, 2 * 60 * 60);
    //tsp.setParam(IloCplex::Param::MIP::Display, 0);
	tsp.setParam(IloCplex::Threads, 1);
	//tsp.setParam(IloCplex::CutUp, UB + 1);

    try {
        tsp.solve();
    }
    catch(IloException &e){
        std::cout << e;
    }
    printf("DONE\n");
    std::cout << "status: " << tsp.getStatus() << std::endl;
    std::cout << "valor objetivo: " << tsp.getObjValue() << std::endl;
    /*for (int i=0;i<data.getDimension();i++){
        for (int j=0;j<data.getDimension();j++){
            if (tsp.getValue(x[i][j])>0.9){
                printf("1 ");
            }
            else printf("0 ");
        }
        printf("\n");
    }*/
}
