//======================================================================================================================================!
// Programmer: Michael Clausen
// Date: 4/27/2021
// File: lga.cpp
//======================================================================================================================================!

#include "lga.h"



//======================================================================================================================================!
// Function: initPop
// Description: Initializes the population of random bit strings of length "length", storing them in "array". 
// Input:
//        -- array  [IN/OUT]     
//        -- size   [IN]
//        -- length [IN]
//
// Output: none
//======================================================================================================================================!
void   initPop(int   array[], int size, int length)
{
	int num = 0;

	int mask = 1;

	int randNum = 0;

	for (int i = 0; i < size; i++)
	{
		// generate random bit strings by randomly choosing a 1 or 0 for each bit
		for (int j = 1; j <= length; j++)
		{
			randNum = rand() % 2;

			if (randNum == 1)
				num ^= mask;

			// left shift the bit mask
			mask <<= 1;
		}

		mask = 1;

		array[i] = num;

		num = 0;
	}
} // end "initPop"



//======================================================================================================================================!
// Function: convertToReal
// Description: Converts the population of bit strings in "array" into real numbers, storing them in "compArray".
// Input:
//        -- array     [IN/OUT]  
//        -- compArray [IN/OUT]
//        -- size      [IN]
//        -- length    [IN]
//        -- problem   [IN]
//
// Output: none
//======================================================================================================================================!
void   convertToReal(int  array[], double  compArray[], int size, int  length, int  problem)
{

	double  rMax = 0.0;

	int     rMin = 0;

	int     value = 0;

	if(problem == 0)
		rMax = 10.0;

	else if(problem == 3)
		rMax = 3.0;

	else
		rMax = 0.0;

	for (int i = 0; i < size; i++)
	{
		value = array[i];

		compArray[i] = ((rMax - rMin) / (pow(2.0, length) - 1)) * value + rMin;
	}
} // end "convertToReal"



//======================================================================================================================================!
// Function: testFitness
// Description: Tests the fitness of each population member in "pop" and stores it in "fitArray". Also calculates the fitness average,
//              max, and sum. Returns the index of the fittest member.
// Input:
//        -- pop       [IN/OUT]  
//        -- fitArray  [IN/OUT]
//        -- size      [IN]
//        -- problem   [IN]
//        -- fAve      [OUT]
//        -- fMax      [OUT]
//        -- fSum      [OUT]
//
// Output: The index of the fittest member
//======================================================================================================================================!
int   testFitness(double  pop[], double  fitArray[], int size, int problem, double& fAve, double& fMax, double& fSum)
{
	double   fitness = 0.0;

	int      fitIndex = 0;

	for (int i = 0; i < size; i++)
	{
		fitness = pop[i];

		// test the fitness of each population member, dependant on the problem number
		if (problem == 0)
			fitness *= fitness;

		else if (problem == 3)
			fitness = pow(sin(fitness), 10);

		else
			fitness = 0.0;

		// update max fitness
		if (fitness > fMax)
		{
			fMax = fitness;

			fitIndex = i;
		}

		fSum += fitness;

		fitArray[i] = fitness;
	}

	fAve = fSum / size;

	return fitIndex;
}



//======================================================================================================================================!
// Function: printGen
// Description: Prints the generation, along with the fitness average, max, and the fittest member, to file.
// Input:
//        -- pop       [IN]
//        -- compArray [IN]
//        -- fitArray  [IN]
//        -- size      [IN]
//        -- len       [IN]
//        -- genNum    [IN]
//        -- fMax      [IN]
//        -- fAve      [IN]
//        -- fitIndex  [IN]
//        -- out       [IN]
//
// Output: None
//======================================================================================================================================!
void   printGen(int  pop[], double  compArray[], double fitArray[], int  size, int len, 
	            int  genNum, double  fMax, double  fAve, int fitIndex, ostream& out)
{
	out << endl << "					Generation " << genNum << endl;

	out << "fMax: " << fMax << endl;

	out << "fAve: " << fAve << endl;

	out << "Fittest member: " << fitIndex + 1 << "    Value: " << compArray[fitIndex] << endl << endl;

	for (int i = 0; i < size; i++)
	{
		boost::dynamic_bitset<> B(len, pop[i]);

		out << "Population member: " << setw(3) << i + 1 << "    String: " 
			<< B << "    X: " << setw(10) << compArray[i] << "    Fitness: " << fitArray[i] << endl;
	}

	out << endl << endl;
}// end "printGen"




//======================================================================================================================================!
// Function: selection
// Description: Selects by using fitness proportional selection pairs of population members and applies crossover with the probability
//              "crossProb" given by the user by calling the function "crossover". Returns a new temporary population.
// Input:
//        -- pop       [IN]
//        -- fitArray  [IN]
//        -- size      [IN]
//        -- len       [IN]
//        -- fSum      [IN]
//        -- crossProb [IN]
//
// Output: An array of the new temporary population of bit strings.
//======================================================================================================================================!
int*   selection(int  pop[], double fitArray[], int size, int len, double fSum, double crossProb)
{
	// round fSum to an integer, so that it can be used in rand()
	fSum = fSum + 0.5 - (fSum < 0);

	int FSum = (int)fSum;

	// choose a random number between 0 and the fSum
	int  randNum = rand() % (FSum + 1);

	double sum = 0.0;

	int    ind1 = -1;

	int    ind2 = -2;

	int    count = 0;

	int    num = 1;

	int    val1 = 0;

	int    val2 = 0;

	crossProb *= 100;

	int* newPop = new int[size];

	// loop until "size" individuals are selected
	for (int i = 0; count < size; i++)
	{
		if (i == size)
			i = 0;

		// add the members fitness to the sum
		sum += fitArray[i];
		
			// if the sum is greater than the random number
			if (sum > randNum)
			{
				// check that the two selected individuals aren't the same
				if (ind1 != ind2)
				{
					// select the first individual
					if (num == 1)
					{
						ind1 = i;

						val1 = pop[i];

						num = 2;
					}

					// select second individual
					else
					{
						ind2 = i;

						val2 = pop[i];

						// apply single point crossover with probability "crossProb"
						if ((rand() % 100) < crossProb)
							crossover(size, len, val1, val2);

						// store the chose individuals and reset the variables
						newPop[count - 1] = val1;

						newPop[count] = val2;

						num = 1;

						ind1 = -1;

						ind2 = -2;
					}

					sum = 0.0;

					// increase count and choose another random number
					count++;

					randNum = rand() % (FSum + 1);
				}
			}
	}

	return newPop;
} // end "selection"



//======================================================================================================================================!
// Function: crossover
// Description: Applies single point crossover to pairs of bit strings, "val1" and "val2".
// Input:
//        -- size      [IN]
//        -- len       [IN]
//        -- val1      [IN/OUT]
//        -- val2      [IN/OUT]
//
// Output: None
//======================================================================================================================================!
void   crossover(int size, int len, int& val1, int& val2)
{
	// choose crossover point
	int   pivot = (rand() % (len-1)) + 1;

	int   lhsMask = 0;

	// set the right hand side bit mask
	int   rhsMask = pow(2.0, pivot) - 1;

	int   lhs1 = 0;

	int   rhs1 = 0;

	int   lhs2 = 0;

	int   rhs2 = 0;

	// set all the bits for the left hand side bit mask
	for (int i = pivot; i < size; i++)
	{
		int   power = pow(2.0, i);

		lhsMask += power;
	}

	//get lhs of val1
	lhs1 = lhsMask & val1;

	// get rhs of val1
	rhs1 = rhsMask & val1;

	// repeat for val2
	lhs2 = lhsMask & val2;

	rhs2 = rhsMask & val2;

	// apply the crossover
	val1 = lhs1 | rhs2;

	val2 = lhs2 | rhs1;
} // end "crossover"



//======================================================================================================================================!
// Function: mutation
// Description: Goes through each bit of every population member, flipping the bit with the probability "mutProb" given by the user.
//              Returns an array containing the mutated population.
// Input:
//        -- pop       [IN/OUT]
//        -- size      [IN]
//        -- len       [IN]
//        -- mutProb   [IN]
//
// Output: Returns an array containing the mutated population.
//======================================================================================================================================!
int*    mutation(int  pop[], int size, int len, double mutProb)
{
	mutProb *= 1000;

	// bit mask for flipping the desired bit
	int  mask = 1;

	int* array = new int[size];

	int  num = 0;

	int  randNum = 0;

	for (int i = 0; i < size; i++)
	{
		num = pop[i];

		// go through each bit, and flip it with probability "mutProb"
		for (int j = 1; j <= len; j++)
		{
			randNum = rand() % 1000;

			if (randNum < mutProb)
				num ^= mask;

			// left shift the bit mask
			mask <<= 1;
		}

		mask = 1;

		array[i] = num;
	}

	return array;
} // end "mutation"



//======================================================================================================================================!
// Function: elitismFunc
// Description: If the user wants elitism, this function checks if the temporary population contains the original populations elite
//              member. If no, it replaces a random member in the temporary population with the elite member.
// Input:
//        -- pop       [IN/OUT]
//        -- compArray [IN/OUT]
//        -- fitArray  [IN/OUT]
//        -- size      [IN]
//        -- elite     [IN]
//        -- eliteFit  [IN]
//        -- eliteStr  [IN]
//
// Output: Returns "true" if the elite member was inserted into the new population, "false" otherwise
//======================================================================================================================================!
bool    elitismFunc(int  pop[], double  compArray[], double  fitArray[], int size, 
	double elite, double eliteFit, int eliteStr)
{
	int  newFitIndex = 0;

	bool replaced = false;

	bool eliteMemberInPop = false;

	for (int i = 0; i < size; i++)
	{
		if (eliteFit <= fitArray[i])
		{
			eliteMemberInPop = true;
			break;
		}
	}

	if (!eliteMemberInPop)
	{
		replaced = true;

		newFitIndex = rand() % size;

		pop[newFitIndex] = eliteStr;

		compArray[newFitIndex] = elite;

		fitArray[newFitIndex] = eliteFit;
	}

	return replaced;

} // end "elitismFunc"