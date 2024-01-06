# MPI_Random_Walk
A program that can use multiple processors to perform one dimensional random walk.

# instructions
To run the program one can execute ./compile.sh in the terminal, and then use mpirun -np [# of process] ./r_walk [int Max] [int W] [int n] to start walking. Where 
 Max: Maximum of the walking environment
 W: Initial position of the walker
 n: number of steps want to walk

note: the walking environment is integer between 0 and Max inclusive. and 0 <= W <= Max. 
