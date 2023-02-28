# LGA Implimentation

This program was a solo project written in C++. It is an implimentation of the Little Genetic Algorithm as outlined in "An Introduction to Genetic Algorithms for Scientists and Engineers" by David A. Coley displayed here below. Output is written to "LGAOutput.txt."

"1. Generate an initial (g = 1) population of random binary
strings.

2. Decode each individual, i, within the population to
integers z sub i, k and then to real numbers r sub i,k to obtain the
unknown parameters.

3. Test each individual in turn on the problem at hand and
convert the objective function or performance, omega, of each
individual to a fitness where a better solution implies a
higher fitness.

4. Select, by using fitness proportional selection, pairs of
individuals and apply with probability Pc single point
crossover. Repeat until a new temporary population of N
individuals is formed.

5. Apply the mutation operator to every individual in the
temporary population, by stepping bit-wise through each
string, occasionally flipping a 0 to a 1 or vice versa. The
probability of any bit mutating is given by Pm and is typically
very small (for example, 0.001).

6. If elitism is required, and the temporary population does
not contain a copy of an individual with at least the fitness of
the elite member, replace (at random) one member of the
temporary population with the elite member.

7. Replace the old population by the new temporary
generation.

8. Increment, by 1, the generational counter (i.e. g = g + 1)
and repeat from Step 2 until G generations have elapsed."
