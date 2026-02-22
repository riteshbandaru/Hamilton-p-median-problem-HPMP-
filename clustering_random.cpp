#include "clustering.h"
#include <cstdlib>
#include <vector>
#include <algorithm>

extern void build_and_assign(std::vector<int>&,
                             read_data&, int);

void run_random(read_data &data, int p){

    int n = data.num_v;
    std::vector<int> seeds;

    while(seeds.size() < p){
        int r = rand()%n;
        if(std::find(seeds.begin(),
                     seeds.end(),
                     r) == seeds.end())
            seeds.push_back(r);
    }

    build_and_assign(seeds,data,p);
}

