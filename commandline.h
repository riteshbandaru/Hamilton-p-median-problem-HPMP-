/*
 * commandline.h
 *
 *  Created on: 22 Jun 2021
 *      Author: Peng
 */

#ifndef COMMANDLINE_H_
#define COMMANDLINE_H_
#include "basic.h"
class CommandLine{
public:
	std::string pathInstance;
	std::string pathSolution;
	int nump;
	int timeLimit=Max;
	int iterLimit;
	int seed=0;
	double bestDis;
	int isOptimal;
	CommandLine(int argc, char* argv[]){
		if (argc!=9){
			std::cout << "the input parameters in configuration are wrong" <<std::endl;
			exit(0);
		}
		else{
			pathInstance=std::string(argv[1]);
			sscanf(argv[2],"%d",&nump);
			pathSolution=std::string(argv[3]);
			sscanf(argv[4],"%lf",&bestDis);
			sscanf(argv[5],"%d",&isOptimal);
			sscanf(argv[6],"%d",&timeLimit);
			sscanf(argv[7],"%d",&iterLimit);
			sscanf(argv[8],"%d",&seed);
			display_infor();
		}
	}
	void display_infor(){
		std::cout<<"Starting The Algorithm"<<std::endl;
		std::cout<<"The Instance is "<<pathInstance<<" and The number of p is "<<nump<<std::endl;
	}
};
#endif /* COMMANDLINE_H_ */
