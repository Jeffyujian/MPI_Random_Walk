#include "RandomWalk.h"

//three arguments:
//1. the maximum value of the walking steps
//2. the inital location of the walker
//3. the number of steps the walker will take. 

int main (int argc, char* argv[ ]) {
  //initialize the MPI environment
  MPI_Init(NULL, NULL);
  
  //Get the number of process
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);

  //Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  //set the total boudary of the walking environment
  int Min = 0;
  int Max = std::stoi(argv[1]);

  //set total length
  int L = Max+1;

  //set boundary and length of each processor
  int Low;
  int High;
  int Length;

  //set the internal boundary of the walking environment for each processor
  //length of the environment is evenly divided among every processor except 
  //the last one, which takes the remainder if there are remainder left.
  if (L % world_size == 0 && world_size <= L) {
    Length = L / world_size;
    Low = world_rank * Length;
    High = Low + Length -1;
  } else if (L % world_size != 0 && world_rank != world_size-1 && world_size <= L) {
    Length = (L - L % world_size) / world_size;
    Low = world_rank * Length;
    High = Low + Length -1;
  } else if (L % world_size != 0 && world_rank == world_size-1 && world_size <= L) {
    Length = L % world_size;
    High = Max;
    Low = High - Length + 1;
  } else {
    std::cout << "number of processors can not exeed the length of the boundary";
    MPI_Finalize();
  }

  //set the initial position of the walker
  int W = std::stoi(argv[2]);

  //set sender and receiver that indicate which process will send and which one will recerive the message
  int sender;
  int receiver;

  //set the total number of walking steps
  int n = std::stoi(argv[3]);

  //set a binary variable
  int r;

  //set a holder
  int hold;

  //walking
  for (int i = 0; i < n; ++i) {
      //random selection between left and right
      r = RNG (2);
      //reset sender and receiver
      sender = -1;
      receiver = -1;
      //random walk within each process except the High and Low
      if (Low < W && W < High) {
        Random_Walk (r, W, world_rank);
      } else if (W == Low) {
        //move right from low
        if (r == 1) {
          ++W;
          std::cout << "Move right from " << W-1 << " to " << W << " in process " << world_rank << std::endl;
        //move left from low
        } else {
          if (world_size == 1) {
            W = Max;
            std::cout << "Move left from " << 0 << " to " << W << " in process " << world_rank << std::endl;
          } else {
            sender = world_rank;
            if (world_rank != 0) {
              receiver = sender - 1;
            //if the walker is at Min, loop over to Max
            } else {
              receiver = world_size - 1;
            }
          }
        }
      } else if (W == High) {
        //move left from high
        if (r == 0) {
          --W;
          std::cout << "Move left from " << W+1 << " to " << W << " in process " << world_rank << std::endl;
        //move right from high
        } else {
          if (world_size == 1) {
            W = 0;
            std::cout << "Move right from " << Max << " to " << W << " in process " << world_rank << std::endl;
          } else {
            sender = world_rank;
            if (world_rank != world_size - 1) {
              receiver = sender + 1;
            //If the walker is at Max, loop over to Min
            } else {
              receiver = 0;
            }
          }
        }
      }

      //synchronize receiver and sender
      MPI_Allreduce(&receiver, &hold, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
      receiver = hold;
      MPI_Allreduce(&sender, &hold, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
      sender = hold;

      //moves to the process on its right
      if (r == 1) {
        if (sender == world_rank) {
          //The process sends message to the right if it is not the last one 
          if (world_rank != world_size - 1) {
            ++W;
          } //If the process is the last one, sends it to the first one
          else {
            W = 0;
          }
          MPI_Send (&W, 1, MPI_INT, receiver, 0, MPI_COMM_WORLD); //send new location to the process on its right
        } else if (receiver == world_rank) {
          MPI_Recv(&W, 1, MPI_INT, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //location received
          if (W == 0) {
            std::cout << "Move right from " << Max << " to " << W << " and from " << "process " << sender << " to " << "process " << receiver << std::endl;
          } else {
            std::cout << "Move right from " << W-1 << " to " << W << " and from " << "process " << sender << " to " << "process " << receiver << std::endl;
          }
        }

      } //moves to the process on its left
        else if (r == 0) {
        if (sender == world_rank) {
          //The process on the left receives the message from the right if it is not the last one
          if (world_rank != 0) {
            --W;
          } //The first process sends message to the last one
          else {
            W = Max;
          }
          MPI_Send (&W, 1, MPI_INT, receiver, 0, MPI_COMM_WORLD); //send new location to the process on its left
        //The process on the right receives the message from the right if it is not the last one
        } else if (receiver == world_rank) {
          MPI_Recv(&W, 1, MPI_INT, sender, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); //location received
          if (W == Max) {
            std::cout << "Move left from " << 0 << " to " << W << " and from " << "process " << sender << " to " << "process " << receiver << std::endl;
          } else {
            std::cout << "Move left from " << W+1 << " to " << W << " and from " << "process " << sender << " to " << "process " << receiver << std::endl;
          }
        }
      }
    }
//end
MPI_Finalize();
}

