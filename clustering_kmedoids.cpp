#include "clustering.h"
#include <vector>
#include <algorithm>

extern void build_and_assign(std::vector<int>&,
                             read_data&, int);

void run_kmedoids(read_data &data, int p){

    int n = data.num_v;
    std::vector<int> medoids;

    while(medoids.size()<p){
        int r = rand()%n;
        if(std::find(medoids.begin(),
                     medoids.end(),
                     r)==medoids.end())
            medoids.push_back(r);
    }

    for(int iter=0; iter<5; iter++){

        std::vector<int> cluster(n);

        for(int i=0;i<n;i++){
            double best=1e18;
            int bestm=0;

            for(int j=0;j<p;j++)
                if(data.D[i][medoids[j]] < best){
                    best=data.D[i][medoids[j]];
                    bestm=j;
                }

            cluster[i]=bestm;
        }

        for(int j=0;j<p;j++){
            double best=1e18;
            int bestv=medoids[j];

            for(int i=0;i<n;i++){
                if(cluster[i]==j){
                    double cost=0;
                    for(int k=0;k<n;k++)
                        if(cluster[k]==j)
                            cost+=data.D[i][k];

                    if(cost < best){
                        best=cost;
                        bestv=i;
                    }
                }
            }
            medoids[j]=bestv;
        }
    }

    build_and_assign(medoids,data,p);
}

