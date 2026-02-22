#include "clustering.h"
#include <vector>
#include <iostream>
#include <algorithm>

static double best_insertion(std::vector<int> &cycle,
                             int v,
                             read_data &data,
                             int &pos)
{
    double best = 1e18;
    int size = cycle.size();

    for(int i=0;i<size;i++){
        int u = cycle[i];
        int w = cycle[(i+1)%size];

        double inc =
            data.D[u][v] +
            data.D[v][w] -
            data.D[u][w];

        if(inc < best){
            best = inc;
            pos = i+1;
        }
    }
    return best;
}

static void assign_nodes(std::vector<std::vector<int>> &cycles,
                         read_data &data)
{
    int n = data.num_v;
    std::vector<bool> used(n,false);

    for(auto &c : cycles)
        for(int v : c)
            used[v]=true;

    for(int v=0; v<n; v++){
        if(!used[v]){
            double best = 1e18;
            int best_cycle=-1;
            int best_pos=0;

            for(int c=0;c<cycles.size();c++){
                int pos;
                double inc = best_insertion(cycles[c], v, data, pos);

                if(inc < best){
                    best = inc;
                    best_cycle = c;
                    best_pos = pos;
                }
            }

            cycles[best_cycle].insert(
                cycles[best_cycle].begin()+best_pos, v);

            used[v]=true;
        }
    }
}

void build_and_assign(std::vector<int> &seeds,
                      read_data &data,
                      int p)
{
    int n = data.num_v;
    std::vector<std::vector<int>> cycles(p);
    std::vector<bool> used(n,false);

    // ----- build 3-node triangles -----
    for(int i=0;i<p;i++){
        int s = seeds[i];
        used[s]=true;

        int second=-1;
        double mind=1e18;

        for(int j=0;j<n;j++)
            if(!used[j] && data.D[s][j] < mind){
                mind=data.D[s][j];
                second=j;
            }

        used[second]=true;

        int third=-1;
        double best=1e18;

        for(int j=0;j<n;j++)
            if(!used[j] &&
               data.D[s][j]+data.D[second][j] < best){
                best = data.D[s][j]+data.D[second][j];
                third=j;
            }

        used[third]=true;

        cycles[i] = {s,second,third};
    }

    // ----- assign remaining nodes -----
    assign_nodes(cycles,data);

    // ----- print result -----
    for(int i=0;i<p;i++){
        std::cout<<"Cycle "<<i+1<<": ";
        for(int v:cycles[i])
            std::cout<<v+1<<" ";
        std::cout<<"\n";
    }
}

