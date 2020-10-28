#include <bits/stdc++.h>
using namespace std;
int main()
{
    double tgt[67]={2579,202339,10628,27686,1610,2020,7542,118282,25395,1950,3323,6110,6528,15780,699,629,426,538,11861,937,2378,6942,69853,2085,40160,2707,134602,1272,171414,5046,55209,11461,21282,26524,29368,22141,26130,29437,20749,21294,22068,14379,42029,41345,50778,44303,59030,96772,58537,73682,80369,49135,48191,108159,2323,1211,7910,15281,21407,48450,675,1273,126643,3916,42080,6859,7013};
    string s;
    ifstream arq("bm.txt");
    ofstream arq2("bm_avg.txt");
    for (int i=0;i<67;i++){
        getline(arq,s);
        arq2<<s;
        double custo=0,tempo=0;
        for (int j=0;j<10;j++){
            getline(arq,s);
            stringstream r;
            double t;
            r<<s.substr(7,s.size()-7);
            r>>t;
            tempo+=t;
            getline(arq,s);
            stringstream p;
            double c;
            p<<s.substr(7,s.size()-7);
            p>>c;
            custo+=c;
            getline(arq,s);
        }
       double pct = ( (custo/10 - tgt[i]) / tgt[i] ) * 100;
       arq2<<"\nTempo: "<<tempo/10<<"\nCusto: "<<custo/10<<endl<<pct<<"%"<<endl<<endl;

    }
    arq.close(); 
    arq2.close();
    return 0;
}
