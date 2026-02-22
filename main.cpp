#include "read_data.h"
#include "clustering.h"
#include <iostream>

int main(int argc, char* argv[]) {

    if(argc < 4){
        std::cout<<"Usage: ./hpmp instance.tsp method p\n";
        std::cout<<"method = random | kmeans | kmedoids\n";
        return 0;
    }

    std::string instance = argv[1];
    std::string method = argv[2];
    int p = atoi(argv[3]);

    read_data data;

    data.read_fun(instance,"",p,0,0,100000,100000,1);

    if(3*p > data.num_v){
        std::cout<<"Error: 3p > n\n";
        return 0;
    }

    if(method=="random")
        run_random(data,p);
    else if(method=="kmeans")
        run_kmeans(data,p);
    else if(method=="kmedoids")
        run_kmedoids(data,p);
    else
        std::cout<<"Unknown method\n";

    return 0;
}

