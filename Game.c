#include "Game.h"



/*
-potential program flaws/inconsistencies:
 how the P1 code is implemented with keeping track of recursion
 with how the player wants to split their cards. (using the track_t struct
 to track how many times a player can split their cards).
 Overall program structure is not the best but the code will at least work.
the plan is to make it modular for parallelization.


-Redundancies in code
*/


void createDeck(deck_t* deck)
{
  int counter = 0, temp;
  deck->cards = (int*)malloc(sizeof(int)*312);
  deck->current = 0;
  for(int i = 0 ;i<312; i++)
  {
    if(i%24 == 0 && counter < 10)
      counter++;
    deck->cards[i] = counter;
  }
  shuffleN(deck,2);
}



void shuffleN(deck_t* deck,int n)
{
  int temp, index;
  for(int j = 0; j<n; j++)
  {
    for(int i = 0 ;i<312; i++)
    {
      index = rand()%312;
      temp = deck->cards[i];
      deck->cards[i] = deck->cards[index];
      deck->cards[index] = temp;
    }
  }
}



void dealerTurn(dealer_t* dealer, deck_t* deck)
{
  updateHand(&(dealer->deck));
  if(dealer->deck.cardTotal < 17)
  {
    dealer->deck.hand[dealer->deck.numCards++] = dealCard(deck);
    // printf("Dealer drew a %d\n",dealer->deck.hand[dealer->deck.numCards-1]);
    dealerTurn(dealer,deck);
  }
}

void recordSoft(player_t* player, dealer_t* dealer)
{
  Hand* curHand = &(player->hands[0]);
  if(curHand->cardTotal == 21 && dealer->deck.cardTotal == 21)
    player->STATS.push++;
  else if(curHand->cardTotal>21 ||(dealer->deck.cardTotal<=21 &&
    curHand->cardTotal <= dealer->deck.cardTotal))
    {
      if(curHand->doubleDown)
        player->STATS.doubleDown[1]++;
      player->STATS.softScore[1]++;
    }
  else
  {
    if(curHand->doubleDown)
      player->STATS.doubleDown[0]++;
    player->STATS.softScore[0]++;
  }
}

void recordDoubles(player_t* player,dealer_t* dealer, int hand)
{
  Hand* curHand = &(player->hands[hand]);
  if(curHand->numCards != 0)
  {
    if(curHand->cardTotal == 21 && dealer->deck.cardTotal == 21)
      player->STATS.push++;
    else if(curHand->cardTotal>21 ||(dealer->deck.cardTotal<=21 &&
      curHand->cardTotal <= dealer->deck.cardTotal))
      {
        if(curHand->doubleDown)
          player->STATS.doubleDown[1]++;
        player->STATS.splitScore[1]++;
      }
    else
    {
      if(curHand->doubleDown)
        player->STATS.doubleDown[0]++;
      player->STATS.splitScore[0]++;
    }
  }
}

void recordHard(player_t* player, dealer_t* dealer)
{
  Hand* curHand = &(player->hands[0]);
  if(curHand->cardTotal == 21 && dealer->deck.cardTotal == 21)
    player->STATS.push++;
  else if(curHand->cardTotal>21 ||(dealer->deck.cardTotal<=21 &&
    curHand->cardTotal <= dealer->deck.cardTotal))
    {
      if(curHand->doubleDown)
        player->STATS.doubleDown[1]++;
      player->STATS.hardScore[1]++;
    }
  else
  {
    if(curHand->doubleDown)
      player->STATS.doubleDown[0]++;
    player->STATS.hardScore[0]++;
  }
}


void gatherStats(game_t* game)
{
  int i, j;
  dealer_t* dealer = &(game->dealer);
  player_t* players = game->players;
  for(i = 0; i<3; i++)
  {
    if(players[i].didSplit)
    {
      for(j = 0; j<4; j++)
        recordDoubles(&players[i],dealer,j);
    }
    else if(players[i].hands[0].handType == SOFT)
      recordSoft(&players[i],dealer);
    else
      recordHard(&players[i],dealer);
  }
}

/*Fix player statistics, format it so it prints per player*/
void printStats(int* arr)
{
  // printf("Player %d statistics: \n",i);
  printf("Hard: w: %d L: %d\n",arr[4],arr[5]);
  printf("Soft: W: %d L: %d\n",arr[0],arr[1]);
  printf("Split: W: %d L: %d\n",arr[2],arr[3]);
  printf("Doubling down: W: %d L: %d\n",arr[6],arr[7]);
  printf("push: %d\n",arr[8]);
  printf("\n\n");

 }



//6 and 6
void playerTurn(game_t* game, Hand* curHand,  int playerNum)
{
  player_t* player = &(game->players[playerNum]);
  track_t* tracker = &(game->tracker);
  dealer_t* dealer = &(game->dealer);
  Hand* nextHand;
  PlayerDecision d;
  if(playerNum == 0)
    d = player1Decide(player, curHand, dealer->deck.hand[0]);
  else if(playerNum == 1)
    d = player2Decide(player,curHand,dealer->deck.hand[0]);
  else
    d = player3Decide(player,curHand,dealer->deck.hand[0]);
  if(d == HIT)
  {
    int num = dealCard(&(game->deck));
    // printf("Player %d hit and got a %d\n",playerNum,num);
    curHand->hand[curHand->numCards++] = num;
    updateHand(curHand);
    playerTurn(game,curHand,playerNum);
  }
  else if(d == STAND)
    return;
  else if(d == DOUBLEDOWN)
  {
    curHand->doubleDown = true;
    curHand->hand[curHand->numCards++] = dealCard(&(game->deck));
    updateHand(curHand);
  }
  else if(d == SPLIT)
  {
    updateTracker(tracker);
    if(tracker->splitNum<=3)
    {
      player->didSplit = true;
      curHand->handType = DOUBLE;
      nextHand = &(game->players[playerNum].hands[tracker->handIndex]);
      nextHand->hand[0] = curHand->hand[1];
      nextHand->hand[1] = dealCard(&(game->deck));
      curHand->hand[1] = dealCard(&(game->deck));
      curHand->numCards = 2;
      nextHand->numCards = 2;
      updateHand(curHand);
      updateHand(nextHand);
      playerTurn(game, nextHand, playerNum);
      playerTurn(game, curHand, playerNum);
    }
    else
    {
      player->canSplit = false;
      playerTurn(game,curHand, playerNum);
    }
  }
}



void clearDeck(deck_t* deck)
{
  free(deck->cards);
  deck->current = 0;
}


int dealCard(deck_t* deck) {return deck->cards[deck->current++];}

/*Potential bug in this code*/
void dealTable(game_t* game)
{
  player_t* players = (game->players);
  int i, j;
  for(i = 0; i<3; i++)
  {
    for(j = 0; j<2; j++)
      players[i].hands[0].hand[j] = dealCard(&(game->deck));
    players[i].hands[0].numCards = 2;
    if(players[i].hands[0].hand[0] == 1 || players[i].hands[0].hand[1] == 1)
      players[i].hands[0].handType = SOFT;
    else
      players[i].hands[0].handType = HARD;
    updateHand(&(players[i].hands[0]));
  }
  for(j = 0; j<2; j++)
    game->dealer.deck.hand[j] = dealCard(&(game->deck));
  game->dealer.deck.numCards = 2;
  updateHand(&(game->dealer.deck));
}


void initGame(game_t* game)
{
  createDeck(&(game->deck));
  createDealer(&(game->dealer));
  for(int i = 0; i<3; i++)
  {
    createPlayer(&game->players[i]);
    createStats(&(game->players[i].STATS));
  }
  setTracker(&(game->tracker));
  dealTable(game);
}


void newGame(game_t* game)
{
  int i, j;
  for( i = 0; i< 3; i++)
  {
    for(j = 0; j<4; j++)
      free(game->players[i].hands[j].hand);
    createPlayer(&(game->players[i]));
  }
  free(game->deck.cards);
  createDeck(&(game->deck));
  clearDealer(&(game->dealer));
  createDealer(&(game->dealer));
  dealTable(game);
}

/*Take all the data and pack it into an array to send
to the manager processor*/
void packData(player_t* player, int* array)
{
  int i;
  stats_t* stats = &(player->STATS);
    array[0] = stats->softScore[0];
    array[1] = stats->softScore[1];
    array[2] = stats->splitScore[0];
    array[3] = stats->splitScore[1];
    array[4] = stats->hardScore[0];
    array[5] = stats->hardScore[1];
    array[6] = stats->doubleDown[0];
    array[7] = stats->doubleDown[1];
    array[8] = stats->push;
}




//
