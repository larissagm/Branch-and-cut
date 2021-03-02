#include <ilcplex/ilocplex.h>
#include "lazyCallback.h"
#include "userCallback.h"
#include "data.h"
#include <stdlib.h>
#include <stdio.h>
#include <chrono>

void solve(Data &data);

int main(int argc, char** argv){

    Data data(argc, argv[1]);
    solve(data);
    
    return 0;
}

void solve(Data &data){

    auto start = std::chrono::system_clock::now();

    IloEnv env;
    IloModel modelo(env);

    data.readData();

    int dim = data.getDimension();
    double **cost = data.getMatrixCost();
    //data.printMatrixDist();

    // variavel x_ij = 1 se o arco for escolhido
    IloArray<IloBoolVarArray> x(env, dim);
    
    // adiciona a variavel x ao modelo
    for (int i=0;i<dim;i++){
        IloBoolVarArray vetor(env,dim);
        x[i]=vetor;
        for (int j=0;j<dim;j++){
            char name[100];
            sprintf(name, "X(%d,%d)",i,j);
            x[i][j].setName(name);
            modelo.add(x[i][j]);
        }
    }

    // objetivo
    IloExpr sum(env);
    for (int i=0;i<dim;i++){
        for(int j=0;j<dim;j++){
            sum+=cost[i][j]*x[i][j];
        }
    }
    modelo.add(IloMinimize(env,sum));

    // restricoes
    for (int i=0; i<dim;i++){
        IloExpr sum1(env);
        IloExpr sum2(env);
        for (int j=0; j<dim;j++){
            sum1+=x[i][j];
            sum2+=x[j][i];
        }
        modelo.add(sum1==1);
        modelo.add(sum2==1);
    }

    // resolve o modelo
    IloCplex tsp(modelo);
    tsp.exportModel("modelo.lp");

    lazyCallback *lazy = new (env) lazyCallback(env,x,dim);
    userCallback *user = new (env) userCallback(env,x,dim);
    tsp.use(lazy);
    tsp.use(user);

    tsp.setParam(IloCplex::TiLim, 7200);
	tsp.setParam(IloCplex::Threads, 1);
    tsp.setParam(IloCplex::Param::MIP::Display, 0);
    //tsp.setParam(IloCplex::Param::MIP::Interval, 1);

    try {
        tsp.solve();
    }
    catch(IloException &e){
        std::cout << e;
    }
    
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> time = end - start;

    std::cout << "Status: " << tsp.getStatus() << std::endl;
    std::cout << "Valor objetivo: " << tsp.getObjValue() << std::endl;
    printf("Tempo: %lf s\n", time);
}
