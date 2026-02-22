/*
 * Indi.h
 *
 *  Created on: 18 Apr 2020
 *      Author: Peng
 */

#ifndef INDIVIDUAL_H_
#define INDIVIDUAL_H_
#include"read_data.h"
struct Node;
struct Route{
	int cour;
	int nbClients;
	int whenLastModified;	// "When" this route has been last modified
	int start;
	double dis;					// Total time on the route
};
struct Node{
	int cour;//city index
	int position;
	int auxPos;// this variable is used to split the giant tour in the crossover operator.
	int whenLastTestedRI;//When the MOVES evaluate this node
	Node *next;
	Node *pre;
	Route *route;
	double cumulatedDis;	// Cumulated distance on this route until the customer (including itself)
};

class Individual{
public:
	//methods
	Individual();
	virtual ~Individual();
	void define(read_data *data);
	void evaluationDis();
	void initilization();
	void updateRouteInfor(int &r);
	void outputSolution();
	void isRight();
	void outputBestSolution();
	//for population management
	double brokenPairsDistance(Individual * indiv2);
	double averageBrokenPairsDistanceClosest(int nbClosest);
	void removeProximity(Individual * indiv);
	//variables
	Node *client;
	int * start;
	Route * route;
	double dis;
	double biasedFit;
	bool isFeasible;
	std::multiset < std::pair < double, Individual* > > indivsPerProximity ;	// The other individuals in the population, ordered by increasing proximity (the set container follows a natural ordering based on the first value of the pair)

private:
	read_data *data;
	int num_v;
	int num_p;
	int *usedTwoVerify;
};
#endif /* INDI_H_ */
