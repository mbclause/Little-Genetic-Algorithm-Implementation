//========================================================================================================================================!
// Programmer: Michael Clausen
// File: main.cpp
// Description: An implimentation of the Little Genetic Algorithm as outlined in "An Introduction to Genetic Algorithms for Scientists
//              and Engineers" by David A. Coley
//========================================================================================================================================!

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>
#include <boost/dynamic_bitset.hpp>

#include "lga.h"

using namespace std;



//======================================================================================================================================!
// Function: "main"
//======================================================================================================================================!
int main(void)
{
	int		  popSize = 20;

	int       strLen = 20;

	int       numGens = 20;

	double    crossProb = 0.65;

	double    mutProb = 0.01;

	bool      elitism = false;

	int       problem = 0;

	int*      strArray;

	double*   realNumArray;

	double*   fitArray;

	ofstream  out;

	double    fMax = 0.0;

	double    fAve = 0.0;

	double    fSum = 0.0;

	int       fitIndex = 0;

	double    eliteFit = 0.0;

	double    eliteVal = 0.0;

	int       eliteStr = 0;

	// seed random number generator
	srand(time(NULL));

	// open output file
	out.open("LGAOutput.txt");

	if (out.fail())
	{
		cerr << "File opening failed\n";

		exit(1);
	}

	// get input from user
	while (true)
	{
		cout << "Enter the population size(must be even): ";

		cin >> popSize;

		if (popSize % 2 == 0)
			break;
	}

	cout << "Enter the substring length: ";

	cin >> strLen;

	cout << "Enter the maximum number of generations: ";

	cin >> numGens;

	while (true)
	{
		cout << "Enter the crossover probability (0 <= x <= 1): ";

		cin >> crossProb;

		if (crossProb >= 0 && crossProb <= 1)
			break;
	}

	while (true)
	{
		cout << "Enter the probability of mutation (0 <= x < 1): ";

		cin >> mutProb;

		if (mutProb >= 0 && mutProb < 1)
			break;
	}

	cout << "Elitism (1 for yes, 0 for no)? ";

	cin >> elitism;

	while(true)
	{
		cout << "Enter problem number('0' for MAX[x^2], '3' for MAX[sin^10(x)]): ";

		cin >> problem;

		if (problem == 0 || problem == 3)
			break;
	}

	// initialize dynamic arrays
	strArray = new int[popSize];

	realNumArray = new double[popSize];

	fitArray = new double[popSize];

	// initialize the population of bit strings
	initPop(strArray, popSize, strLen);

	// loop until the max number of generations is reached
	for (int i = 1; i <= numGens; i++)
	{
		// convert bit strings to real numbers
		convertToReal(strArray, realNumArray, popSize, strLen, problem);

		// test the fitness of the population
		fitIndex = testFitness(realNumArray, fitArray, popSize, problem, fAve, fMax, fSum);

		// if elitism is needed, check if the new population has the elite member and insert it if not
		if (elitism && i > 1)
		{
			// if the old elite member was inserted, test the fitness of the population again
			if(elitismFunc(strArray, realNumArray, fitArray, popSize, eliteVal, eliteFit, eliteStr))
			{
				fAve = 0.0;

				fMax = 0.0;

				fSum = 0.0;

				fitIndex = testFitness(realNumArray, fitArray, popSize, problem, fAve, fMax, fSum);
			}
		}

		// ouput the generation
		printGen(strArray, realNumArray, fitArray, popSize, strLen, i, fMax, fAve, fitIndex, out);

		// if elitism is needed, get the values of the elite member
		if (elitism)
		{
			eliteVal = realNumArray[fitIndex];

			eliteFit = fitArray[fitIndex];

			eliteStr = strArray[fitIndex];
		}

		// select the members for the new temporary population
		strArray = selection(strArray, fitArray, popSize, strLen, fSum, crossProb);

		// mutate the temp population
		strArray = mutation(strArray, popSize, strLen, mutProb);

		// zero out the fitness values passed by reference
		fAve = 0.0;

		fMax = 0.0;

		fSum = 0.0;
	}

	// close output file
	out.close();

	return 0;
} // end "main"
