#include "maxbackMincut.h"
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <limits>
#include <algorithm>
#include <list>
#include <chrono>
using namespace std;

vector<vector<int>> maxback(vector<vector<double>> x, int dim){

    vector<vector<int>> sol;
    list<int> lista;

    for (int i=0; i<dim; i++) {
        lista.push_back(i);
    }

    while (!lista.empty()){

        int v = lista.front();
        vector<int> S, h, s;
        vector<double> b(dim);
        
        double cutval=0.0, cutmin=0.0, max=-1.0;
        int n;
        for (int i=0, k=0; i<dim; i++){
            if (i==v) {
                b[i]=0.0;
                h.push_back(i);
                s.push_back(i);
                continue;
            }

            b[i] = x[v][i];
            if (b[i]>max){
                max = b[i];
                n=k;
            }
            S.push_back(i);
            k++;
            
            cutmin+=b[i];
        }
        
        cutval = cutmin;
        int total = 0, e;
        
        while (h.size() < lista.size()) {
            e=S[n];
            cutval+= 2 - 2*b[e];

            h.push_back(e);
            S.erase(S.begin()+n);
            max = -1;
            for (int i=0; i<S.size(); i++){
                if (e<S[i]) b[S[i]]+= x[e][S[i]];
                else b[S[i]]+= x[S[i]][e];

                if (b[S[i]]>max){
                    max=b[S[i]];
                    n=i;
                }
            }

            if (cutval < cutmin){
                cutmin=cutval;
                s=h;
            }
        }
        if (s.size()!=dim) sol.push_back(s);
        for (int i=0; i<s.size(); i++) lista.remove(s[i]);   
    }
    return sol;
}

vector<vector<int>> mincut(vector<vector<double>> x, int dim) {

    double mincut = numeric_limits<double>::infinity();
    vector<vector<int>> sol;
    vector<vector<int>> aux(dim);
    vector<bool> ch(dim, false);

    for (int i=0; i<dim; i++){
        aux[i].push_back(i);
		for(int j=i+1; j<dim; j++) {
            x[j][i] = x[i][j];
        }
    }
    vector<vector<double>> x2 = x;

    int c = 0;
    while (c < dim-1) {

        vector<int> V(dim-1-c);
        for (int i=1,j=0; i<dim; i++){
            if (!ch[i]){
                V[j]=i;
                j++;
            }
        }

        //minimumCutPhase
        int t = 1;
        vector<int> A(V.size()+1,-1);
        A[0] = 0;
        double phaseCut = -1, maxSum;
        while (t < A.size()) {  // A!=V
            
            maxSum = -1;
            int iMax;
            for (int i=0;i<V.size();i++){
                double weightSum = 0;
                for (int j=0; j<t; j++){
                    weightSum+= x2[A[j]][V[i]];
                }
                if (weightSum > maxSum) {
                    maxSum = weightSum;
                    iMax = i;
                }
            }
            A[t] = V[iMax];
            t++;
            V.erase(V.begin()+iMax); 
        }
        c++;
        phaseCut = maxSum;
        ch[A[A.size()-1]] = true;

        for (int i=0; i < A.size() - 2; i++) {
            x2[A[i]][A[A.size()-2]] += x2[A[i]][A[A.size()-1]];
            x2[A[A.size()-2]][A[i]] += x2[A[i]][A[A.size()-1]];
        }

        if (phaseCut < mincut) mincut = phaseCut;

        double cutval = 0.0;
        int h = A[A.size()-1];
        
        if (aux[h].size()<dim){
            for(int i = 0; i < (aux[h].size() - 1); i++){
                for(int j = i+1; j < aux[h].size(); j++){
                    if(aux[h][i] < aux[h][j]) cutval += x[aux[h][i]][aux[h][j]];
                    else cutval += x[aux[h][j]][aux[h][i]];
                }
            }
            if(cutval > aux[h].size()-1){
                sol.push_back(aux[h]);
            }
        }
        
        for(int i = 0; i < aux[A[A.size()-1]].size(); i++){
			aux[A[A.size()-2]].push_back(aux[A[A.size()-1]][i]);
		}
    }
    return sol;
}