#include <mpi.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

//a random number generator that generate number from 0 to n-1
int RNG (int range);

//random walk when world_size = 1
void Walk (int& r, int& W, int& Max, int& Min, int& world_rank);

//random walk in one of the processor
void Random_Walk (int& r, int& W, int& world_rank);