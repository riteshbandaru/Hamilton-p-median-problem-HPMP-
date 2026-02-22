#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <dirent.h>
#include <time.h>

#define MAXN 1000
#define MAXP 100

double coords[MAXN][2];
int D[MAXN][MAXN];

int n;
int p = 5;   // number of clusters

// ------------------ ATT DISTANCE ------------------
int att_distance(int i, int j) {
    double xd = coords[i][0] - coords[j][0];
    double yd = coords[i][1] - coords[j][1];
    double rij = sqrt((xd*xd + yd*yd) / 10.0);
    return (int)(rij + 0.5);
}

void compute_distance_matrix() {
    for(int i=0;i<n;i++){
        for(int j=i;j<n;j++){
            if(i==j) D[i][j]=0;
            else {
                D[i][j]=D[j][i]=att_distance(i,j);
            }
        }
    }
}

// ------------------ READ TSP ------------------
void read_tsp(char *filename){
    FILE *f = fopen(filename,"r");
    if(!f){
        printf("Cannot open %s\n",filename);
        exit(1);
    }

    char line[256];
    int read=0;
    n=0;

    while(fgets(line,256,f)){
        if(strstr(line,"NODE_COORD_SECTION")){
            read=1;
            continue;
        }
        if(strstr(line,"EOF")) break;

        if(read){
            int id;
            double x,y;
            sscanf(line,"%d %lf %lf",&id,&x,&y);
            coords[n][0]=x;
            coords[n][1]=y;
            n++;
        }
    }

    fclose(f);
}

// ------------------ CHEAPEST INSERTION ------------------
int best_insertion(int cycle[], int size, int v, int *best_pos){
    int best_inc = 1e9;

    for(int i=0;i<size;i++){
        int u = cycle[i];
        int w = cycle[(i+1)%size];

        int inc = D[u][v] + D[v][w] - D[u][w];

        if(inc < best_inc){
            best_inc = inc;
            *best_pos = i+1;
        }
    }
    return best_inc;
}

// ------------------ RANDOM SEEDS ------------------
void random_seeds(int seeds[]){
    for(int i=0;i<p;i++){
        seeds[i] = rand() % n;
        for(int j=0;j<i;j++)
            if(seeds[i]==seeds[j]) i--;
    }
}

// ------------------ KMEANS++ SEEDS ------------------
void kmeanspp_seeds(int seeds[]){
    seeds[0] = rand()%n;

    for(int k=1;k<p;k++){
        int best=-1;
        int bestdist=-1;

        for(int i=0;i<n;i++){
            int mind=1e9;
            for(int j=0;j<k;j++){
                if(D[i][seeds[j]] < mind)
                    mind = D[i][seeds[j]];
            }
            if(mind > bestdist){
                bestdist = mind;
                best = i;
            }
        }
        seeds[k] = best;
    }
}

// ------------------ KMEDOIDS SEEDS ------------------
void kmedoids_seeds(int seeds[]){
    for(int i=0;i<p;i++)
        seeds[i] = rand()%n;

    for(int iter=0; iter<5; iter++){

        int cluster[MAXN];
        for(int i=0;i<n;i++){
            int best=0;
            int bestd=1e9;
            for(int j=0;j<p;j++){
                if(D[i][seeds[j]] < bestd){
                    bestd = D[i][seeds[j]];
                    best = j;
                }
            }
            cluster[i] = best;
        }

        for(int j=0;j<p;j++){
            int best=-1;
            int bestcost=1e9;

            for(int i=0;i<n;i++){
                if(cluster[i]==j){
                    int cost=0;
                    for(int k=0;k<n;k++)
                        if(cluster[k]==j)
                            cost += D[i][k];

                    if(cost < bestcost){
                        bestcost = cost;
                        best = i;
                    }
                }
            }
            seeds[j] = best;
        }
    }
}

// ------------------ BUILD TRIANGLES ------------------
void build_initial_cycles(int seeds[], int cycles[MAXP][MAXN], int sizes[]){
    int used[MAXN]={0};

    for(int i=0;i<p;i++){
        int s = seeds[i];
        used[s]=1;

        int second=-1, mind=1e9;
        for(int j=0;j<n;j++)
            if(!used[j] && D[s][j]<mind){
                mind=D[s][j];
                second=j;
            }
        used[second]=1;

        int third=-1, best=1e9;
        for(int j=0;j<n;j++)
            if(!used[j] && D[s][j]+D[second][j] < best){
                best = D[s][j]+D[second][j];
                third=j;
            }
        used[third]=1;

        cycles[i][0]=s;
        cycles[i][1]=second;
        cycles[i][2]=third;
        sizes[i]=3;
    }
}

// ------------------ ASSIGN REMAINING ------------------
void assign_remaining(int cycles[MAXP][MAXN], int sizes[]){
    int used[MAXN]={0};

    for(int i=0;i<p;i++)
        for(int j=0;j<sizes[i];j++)
            used[cycles[i][j]] = 1;

    for(int v=0; v<n; v++){
        if(!used[v]){
            int best_cycle=-1;
            int best_pos=0;
            int best_cost=1e9;

            for(int c=0;c<p;c++){
                int pos;
                int inc = best_insertion(cycles[c], sizes[c], v, &pos);

                if(inc < best_cost){
                    best_cost=inc;
                    best_cycle=c;
                    best_pos=pos;
                }
            }

            for(int k=sizes[best_cycle]; k>best_pos; k--)
                cycles[best_cycle][k] = cycles[best_cycle][k-1];

            cycles[best_cycle][best_pos] = v;
            sizes[best_cycle]++;
            used[v]=1;
        }
    }
}

// ------------------ MAIN ------------------
int main(){

    srand(time(NULL));

    DIR *dir;
    struct dirent *ent;

    dir = opendir("S");
    if(!dir){
        printf("Cannot open folder S\n");
        return 1;
    }

    while((ent = readdir(dir)) != NULL){
        if(strstr(ent->d_name,".tsp")){

            char path[256];
            snprintf(path, sizeof(path), "S/%s", ent->d_name);

            printf("\nProcessing %s\n", ent->d_name);

            read_tsp(path);
            compute_distance_matrix();

            int seeds[MAXP];
            int cycles[MAXP][MAXN];
            int sizes[MAXP];

            // ----- Choose ONE method -----

            // random_seeds(seeds);
            // kmeanspp_seeds(seeds);
            kmedoids_seeds(seeds);

            build_initial_cycles(seeds, cycles, sizes);
            assign_remaining(cycles, sizes);

            for(int i=0;i<p;i++){
                printf("Cycle %d: ",i+1);
                for(int j=0;j<sizes[i];j++)
                    printf("%d ", cycles[i][j]+1);
                printf("\n");
            }
        }
    }

    closedir(dir);
    return 0;
}

