/*
 * readdata.cpp
 *
 *  Created on: 17 Apr 2020
 *      Author: Peng
 */

#include "read_data.h"
#include "basic.h"
#include "Individual.h"
#include <sstream>
read_data::read_data() {

}
read_data::~read_data() {
	for (int i=0;i<num_v;i++)
		delete [] D[i];
	delete [] D;
	delete [] x;
	delete [] y;
	for (int i=0;i<num_v;i++)
		delete [] correctedEdge[i];
	delete [] correctedEdge;
}
void read_data::define(){
	D=new double *[num_v];
	for (int i=0;i<num_v;i++)
		D[i]=new double [num_v];
	x=new double [num_v];
	y=new double [num_v];
	correctedEdge=new int *[num_v];
	for (int i=0;i<num_v;i++)
		correctedEdge[i]=new int [num_v];
	for (int i=0;i<num_v;i++)
		for (int j=0;j<num_v;j++)
			correctedEdge[i][j]=0;
	edge_node=std::vector<std::vector<int>>(num_v);
	nearCity = std::vector< std::vector < int > >(num_v);
}
void read_data::read_fun(std::string pathInstance, std::string pathSolution, int nump, double bestDis, int isOptimal, int timeLimit,int iterLimit,int seed){
	/****imput information*******/
	this->timeLimit=timeLimit;
	this->iterLimit=iterLimit;
	this->num_p=nump;
	this->bestDis = bestDis;
	this->isOptimal=isOptimal;
	/*********************************************************/
	this->pathSolution=pathSolution;
	this->pathInstance=pathInstance;
	/*********************************************************/
	// read the input file
	std::string content, content2, content3;
	int temVar;
//	std::cout<<pathInstance<<std::endl;
	std::ifstream inputFile(pathInstance);
	if (inputFile.is_open()){
		for (inputFile >> content ; ((content != "NODE_COORD_SECTION") && (content !="EDGE_WEIGHT_SECTION")) ; inputFile >> content){
			if (content == "DIMENSION"){inputFile >> content2 >> num_v;} // Need to substract the depot from the number of nodes
			else if (content == "DIMENSION:"){inputFile >> num_v;}
			else if (content == "EDGE_WEIGHT_TYPE"){inputFile >> content2 >> typeGraph;}
			else if (content == "EDGE_WEIGHT_TYPE:"){inputFile >> typeGraph;}
			else if (content == "EDGE_WEIGHT_FORMAT:"){inputFile >> matrix_row;}
			else if (content == "EDGE_WEIGHT_FORMAT"){inputFile >> content2 >> matrix_row;}
//			else if (content == "OPTIMALSOLUTION:"){inputFile >> OptimalLB;}
//			else if (content == "isOPTIMAL:"){inputFile >> wheatherOptimal;}
		}
		define();
		if (typeGraph == "EXPLICIT"){
			if (matrix_row == "UPPER_ROW"){
				for (int i=0;i<num_v;i++){
					D[i][i]=0;
					for (int j=i+1;j<num_v;j++){
						inputFile>>D[i][j];
						D[j][i]=D[i][j];
					}
				}
			}
			else if (matrix_row == "LOWER_DIAG_ROW"){
				for (int i=0;i<num_v;i++){
					for (int j=0;j<= i;j++){
						inputFile>>D[i][j];
						D[j][i]=D[i][j];
					}
				}
			}
			else if (matrix_row == "FULL_MATRIX"){
				for (int i=0;i<num_v;i++)
					for (int j=0;j<num_v;j++)
						inputFile>>D[i][j];
			}
		}
		else{
			for (int i=0;i<num_v;i++){
				inputFile>>temVar;
				inputFile>>x[i];
				inputFile>>y[i];
			}
		}
	}
	else{
		std::cout<<"The input file was wrong"<<std::endl;
		exit(0);
	}
	inputFile.close();
	if (typeGraph == "EUC_2D"){
		for (int i=0;i<num_v;i++){
			for (int j=0;j<num_v;j++){
				D[i][j]=(sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j])));
			//	D[i][j]=int((sqrt((x[i]-x[j])*(x[i]-x[j])+(y[i]-y[j])*(y[i]-y[j])))+0.5);
			}
		}
	}
	if (typeGraph == "GEO"){
	    double deg, min;
	    double lati, latj, longi, longj;
	    double q1, q2, q3;
	    int dd;
	    double x1,x2,yy1,yy2;
		for (int i=0;i<num_v;i++){
			for (int j=0;j<num_v;j++){
				x1=x[i];x2=x[j];
				yy1=y[i];yy2=y[j];
				deg=dtrunc(x1);
				min=x1-deg;
				lati=PI * (deg + 5.0 * min / 3.0) / 180.0;
				deg=dtrunc(x2);
				min=x2-deg;
				latj=PI * (deg + 5.0 * min / 3.0) / 180.0;
				//
				deg=dtrunc(yy1);
				min=yy1-deg;
				longi=PI * (deg + 5.0 * min / 3.0) / 180.0;
				deg=dtrunc(yy2);
				min=yy2-deg;
				longj=PI * (deg + 5.0 * min / 3.0) / 180.0;
				//
				q1=cos (longi-longj);
				q2=cos (lati - latj);
				q3=cos (lati + latj);
				dd=(int)(6378.388 * acos (0.5 * ((1.0 + q1) * q2 - (1.0 - q1) * q3)) + 1.0);
				D[i][j]=dd;
			}
		}
	}
	if (typeGraph == "ATT"){
		double xd,yd,rij,tij,dij;
		for (int i=0;i<num_v;i++){
			for (int j=0;j<num_v;j++){
				xd=x[i]-x[j];
				yd=y[i]-y[j];
				rij=sqrt((xd * xd + yd * yd) / 10.0);
				tij=dtrunc(rij);
				if (tij < rij)
					dij = (int)tij+1;
				else
					dij=(int)tij;
				D[i][j]=dij;
			}
		}
	}
	if (typeGraph == "CEIL_2D"){
		double t1,t2;
		for (int i=0;i<num_v;i++){
			for (int j=0;j<num_v;j++){
				t1=x[i]-x[j];
				t2=y[i]-y[j];
				D[i][j]=(int) (ceil(sqrt(t1*t1 + t2*t2)));
			}
		}
	}
	/* construct sparse graph****************/
	std::vector < std::set < int > > setCorrelatedVertices = std::vector < std::set <int> >(num_v);
	std::vector < std::pair <double, int> > orderProximity;
	for (int i = 0; i <num_v; i++){
		orderProximity.clear();
		for (int j = 0; j <num_v; j++)
			if (i != j) orderProximity.push_back(std::pair <double, int>(D[i][j], j));
		sort(orderProximity.begin(), orderProximity.end());
		for (int j = 0; j < std::min<int>(alpha, num_v - 2); j++){
			setCorrelatedVertices[i].insert(orderProximity[j].second);
			setCorrelatedVertices[orderProximity[j].second].insert(i);
		}
		for (int j=0;j < (int)orderProximity.size();j++)
			nearCity[i].push_back(orderProximity[j].second);
	}
	// Filling the vector of correlated vertices
	for (int i = 0; i <num_v; i++)
		for (int x : setCorrelatedVertices[i])
			edge_node[i].push_back(x);
	for (int i=0;i<num_v;i++)
		for (int j=0;j<(int)edge_node[i].size();j++)
			correctedEdge[i][edge_node[i][j]]=1;
}
double read_data::dtrunc (double x){
    int k;
    k = (int) x;
    x = (double) k;
    return x;
}
bool read_data::stopCondition(double dis){//stopping condition function
	if ((double)clock()/(double)CLOCKS_PER_SEC > timeLimit)
		return true;
	if (iteration > iterLimit)
		return true;
	if ( meetOptimal && bestDis > dis + Min){
		timeToTarget = (double)clock()/(double)CLOCKS_PER_SEC;
		meetOptimal = false;
	}
	if (std::abs(bestDis - dis) < 0.01){
		if (isOptimal)return true;
	}

	return false;
}

