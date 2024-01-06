mpic++ -c main.cpp -o main.o
mpic++ -c RandomWalk.cpp -o RandomWalk.o
mpic++ main.o RandomWalk.o -o r_walk

