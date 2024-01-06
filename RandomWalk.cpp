#include "RandomWalk.h"

//a random number generator that generate number from 0 to n-1
int RNG (int range) {
  std::ifstream urandom("/dev/urandom", std::ios::in|std::ios::binary);
  unsigned int seed;
  urandom.read(reinterpret_cast<char*>(&seed), sizeof(seed));
  urandom.close();
  srand(seed);
  int r = rand() % range;
  return r;
}

//random walk when world_size = 1
void Walk (int& r, int& W, int& Max, int& Min, int& world_rank) {
  r = RNG(2);
  //walk right
  if (r == 1) {
    if (W != Max) {
      ++W;
      std::cout << "Move right from " << W-1 << " to " << W << " in process " << world_rank << "\n";
    }else{
      W = Min;
      std::cout << "Move right from " << Max << " to " << W << " in process " << world_rank << "\n";
    }
  //walk left
  }else if (r == 0) {
    if (W != Min) {
      --W;
      std::cout << "Move left from " << W+1 << " to " << W << " in process " << world_rank << "\n";
    }else{
      W = Max;
      std::cout << "Move left from " << Min << " to " << W << " in process " << world_rank << "\n";
    }
  }
}

//random walk in one of the processor
void Random_Walk (int& r, int& W, int& world_rank) {
  //walk right
  if (r == 1) {
    ++W;
    std::cout << "Move right from " << W-1 << " to " << W << " in process " << world_rank << "\n";
  //walk left
  }else if (r == 0) {
    --W;
    std::cout << "Move left from " << W+1 << " to " << W << " in process " << world_rank << "\n";
  }
}
