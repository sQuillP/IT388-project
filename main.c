#include "Game.h"
#include <mpi.h>


/*
* Serial Gang Software
* Copyright 2020
* @ William Pattison, Kevin Gomez,
* and Filip Sofeski
*/


/*Runner program for blackjack. Uses MPI library to partition
simulations among processors.*/


int main(int argc, char** argv)
{
  int i, j, nproc, my_rank, localGames, simulations;
  double start, end;
  game_t game;
  time_t t;
  MPI_Init(&argc, &argv);
  MPI_Comm comm = MPI_COMM_WORLD; MPI_Status status;
  MPI_Comm_size(comm,&nproc);
  MPI_Comm_rank(comm,&my_rank);
  simulations = atoi(argv[1]);

  /*Generate a unique seed for each processor*/
  srand((unsigned)time(&t)+my_rank);

  if(my_rank == 0 && argc != 2)
  {
    printf("\nUse: mpiexec -n <# of processors> ./test <# of simulations>\n\n");
    exit(1);
  }

  localGames = simulations/nproc;
  if(simulations%nproc!=0 && my_rank == 0)
    localGames += simulations%nproc;

  /*Allocate memory for holding stats*/
  int** stats = (int**)malloc(sizeof(int*)*3);
  int** collectedStats = (int**)malloc(sizeof(int*)*3);
  for(i = 0; i<3; i++)
  {
    collectedStats[i] = (int*)malloc(sizeof(int)*10);
    stats[i] = (int*)malloc(sizeof(int)*10);
  }

  initGame(&game);
  Hand* curHand;
  MPI_Barrier(comm);
  start = MPI_Wtime();

  /*Run the simulations*/
  for(j = 0; j<localGames; j++)
  {
    if(game.dealer.deck.cardTotal==21)
    {
      gatherStats(&game);
      newGame(&game);
    }
    else
    {
      for(i = 0; i<3; i++)
      {
        curHand = &(game.players[i].hands[0]);
        playerTurn(&game,curHand,i);
        setTracker(&(game.tracker));
      }
      dealerTurn(&(game.dealer),&(game.deck));
      gatherStats(&game);
      newGame(&game);
    }
  }

  /*Pack game statistics into stats array*/
  for(i = 0; i<3; i++)
    packData(&(game.players[i]),&stats[i][0]);

  MPI_Barrier(comm);
  end = MPI_Wtime() - start;

  /*Get the sum of all stats from each processor*/
  for(i = 0; i<3; i++)
    MPI_Reduce(&stats[i][0],&collectedStats[i][0],10,MPI_INT,MPI_SUM,0,comm);

  /*Manager prints the results of the simulation*/
  if(my_rank == 0)
  {
    for(i = 0; i<3; i++)
      printStats(&collectedStats[i][0],i+1);
    printf("\n# of simulations: %d\nTotal time: %f\n",simulations,end);
  }

  for(i = 0 ;i<3; i++)
    free(stats[i]);
  free(stats);
  MPI_Finalize();

  return 0;
}
