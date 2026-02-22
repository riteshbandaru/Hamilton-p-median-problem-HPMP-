/*
 * readdata.h
 *
 *  Created on: 17 Apr 2020
 *      Author: Peng
 */

#ifndef READ_DATA_H_
#define READ_DATA_H_
#include <time.h>
#include "basic.h"
class read_data {
public:
	//general variables
	int iteration=0;
	int nbIterNonImpro;
	int num_v;
	int num_p;
	clock_t startTime;
	int timeLimit;
	int iterLimit;
	double meetBestTime=Max;
	double timeToTarget=Max;
	int isOptimal;
	bool meetOptimal=true;
	//parameters
	double mutationPer = 0.1;
	int maxIterNonProd=30000;
	int alpha=10;
	int popMin=50;
	int genSize=25;
	int popMax=popMin+genSize;
/***********************************************************************************/
	int nbClost=5;//parameters for measuring distance
	int nbElite=4;//parameters for measuring distance
	std::vector < std::vector < int > > edge_node;
	std::vector < std::vector < int > > nearCity;
	int **correctedEdge;
	double ** D;// distance between all cities
	double *x;
	double *y;
	double bestDis;
/****************************************************************************/
	read_data();
	virtual ~read_data();
	void define();
	void read_fun(std::string pathInstance,std::string pathSolution, int nump, double bestDis, int isOptimal, int timeLimit,int iterLimit,int seed);
	bool stopCondition(double dis);
	std::string pathSolution;     // used to file name of output best solution
	std::string pathInstance;
private:
	std::string typeGraph;
	std::string matrix_row;
	double dtrunc (double x);
};

#endif /* READ_DATA_H_ */
