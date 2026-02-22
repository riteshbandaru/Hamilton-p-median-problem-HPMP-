#include "clustering.h"
#include <vector>
#include <algorithm>

extern void build_and_assign(std::vector<int>&,
                             read_data&, int);

void run_kmeans(read_data &data, int p){

    int n = data.num_v;
    std::vector<int> seeds;

    seeds.push_back(rand()%n);

    while(seeds.size()<p){

        int best=-1;
        double bestdist=-1;

        for(int i=0;i<n;i++){

            double mind=1e18;

            for(int s:seeds)
                mind = std::min(mind,
                                data.D[i][s]);

            if(mind > bestdist){
                bestdist = mind;
                best = i;
            }
        }

        seeds.push_back(best);
    }

    build_and_assign(seeds,data,p);
}

