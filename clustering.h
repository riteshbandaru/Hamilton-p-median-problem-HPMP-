#ifndef CLUSTERING_H
#define CLUSTERING_H

#include "read_data.h"
#include <vector>

void run_random(read_data &data, int p);
void run_kmeans(read_data &data, int p);
void run_kmedoids(read_data &data, int p);

void build_and_assign(read_data &data, std::vector<int> &seeds, int p);

#endif

