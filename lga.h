//======================================================================================================================================!
// Programmer: Michael Clausen
// Date: 4/27/2021
// File: lga.h
//======================================================================================================================================!

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <math.h> 
#include <iomanip>
#include <boost/dynamic_bitset.hpp>

using namespace std;



// function declarations
void   initPop(int   array[], int size, int length);


void   convertToReal(int  array[], double  compArray[], int size, int  length, int  problem);


int    testFitness(double  pop[], double  fitArray[], int size, int problem, double& fAve, double& fMax, double& fSum);


void   printGen(int  pop[], double  compArray[], double fitArray[], int  size, int len, 
	            int  genNum, double  fMax, double  fAve, int fitIndex, ostream& out);


int*   selection(int  pop[], double fitArray[], int size, int len, double fSum, double  crossProb);


void   crossover(int size, int len, int& val1, int& val2);


int*   mutation(int  pop[], int size, int len, double mutProb);


bool   elitismFunc(int  pop[], double  compArray[], double  fitArray[], 
	int size, double elite, double eliteFit, int eliteStr);
