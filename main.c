#include "Game.h"
#include <mpi.h>



int main(int argc, char** argv)
{
  int i, j, nproc, my_rank, localGames, simulations;
  game_t game;
  time_t t;
  srand((unsigned)time(&t));
  MPI_Init(&argc, &argv);
  MPI_Comm comm = MPI_COMM_WORLD; MPI_Status status;
  MPI_Comm_size(comm,&nproc);
  MPI_Comm_rank(comm,&my_rank);
  simulations = atoi(argv[1]);
  localGames = simulations/nproc;
  if(simulations%nproc!=0 && my_rank == 0)
    localGames += simulations% nproc;

  int** stats = (int**)malloc(sizeof(int*)*3);
  int** collectedStats = (int**)malloc(sizeof(int*)*3);
  for(i = 0; i<3; i++)
  {
    collectedStats[i] = (int*)malloc(sizeof(int)*9);
    stats[i] = (int*)malloc(sizeof(int)*9);
  }
  
  initGame(&game);
  Hand* curHand;
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
  for(i = 0; i<3; i++)
    packData(&(game.players[i]),&stats[i][0]);
  for(i = 0; i<3; i++)
    MPI_Reduce(&stats[i][0],&collectedStats[i][0],9,MPI_INT,MPI_SUM,0,comm);
  if(my_rank == 0)
  {
    for(i = 0; i<3; i++)
      printStats(&collectedStats[i][0]);
  }

  for(i = 0 ;i<3; i++)
    free(stats[i]);
  free(stats);
  MPI_Finalize();
  // printStats(&game);
  return 0;
}
