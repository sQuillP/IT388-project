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
  time_t t;
  srand((unsigned)time(&t));
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
    printf("Dealer drew a %d\n",dealer->deck.hand[dealer->deck.numCards-1]);
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
        recordDoubles(&players[i],dealer,i);
    }
    else if(players[i].hands[0].handType == SOFT)
    {
      printf("Player %d had soft hand\n",i);
      recordSoft(&players[i],dealer);
    }
    else
    {
      printf("player %d had a hard hand\n",i);
      recordHard(&players[i],dealer);
    }
  }

}

/*TODO: finish the stats for the player and then parallelize the code*/


void printStats(game_t* game)
{
  int i;
  player_t* player;
  printf("\n");
  for(i = 0; i<3; i++)
  {
    player = &(game->players[i]);
    printf("Player %d statistics: \n",i);
    printf("Hard: w: %d L %d\n",player->STATS.hardScore[0],player->STATS.hardScore[1]);
    printf("Soft: W: %d L %d\n",player->STATS.softScore[0],player->STATS.softScore[1]);
    printf("Double: W: %d L %d\n",player->STATS.splitScore[0],player->STATS.splitScore[1]);
    printf("Doubling down: W: %d L: %d\n",player->STATS.doubleDown[0],player->STATS.doubleDown[1]);
    printf("push: W: %d\n",player->STATS.push);
    printf("\n\n");
  }
}



//6 and 6
void playerTurn(game_t* game, Hand* curHand,  int playerNum)
{
  player_t* player = &(game->players[playerNum]);
  track_t* tracker = &(game->tracker);
  dealer_t* dealer = &(game->dealer);
  int cardValue;
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
    cardValue = dealCard(&(game->deck));
    printf("HIT Player received card: %d\n",cardValue);
    curHand->hand[curHand->numCards++] = cardValue;
    updateHand(curHand);
    playerTurn(game,curHand,playerNum);
  }
  else if(d == STAND)
  {
    printf("STAND\n");
    return;
  }
  else if(d == DOUBLEDOWN)
  {
    //remove double the cash for the player
    curHand->doubleDown = true;
    cardValue = dealCard(&(game->deck));
    curHand->hand[curHand->numCards++] = cardValue;
    updateHand(curHand);
    printf("DOUBLEDOWN Player received card: %d\n",cardValue);
  }
  else if(d == SPLIT)
  {
    updateTracker(tracker);
    if(tracker->splitNum<=3)
    {
      printf("SPLIT\n");
      player->didSplit = true;
      curHand->handType = DOUBLE;
      nextHand = &(game->players[playerNum].hands[tracker->handIndex]);
      nextHand->hand[0] = curHand->hand[1];
      cardValue = dealCard(&(game->deck));
      printf("nextHand was dealt: %d\n",cardValue);
      nextHand->hand[1] = cardValue;
      cardValue = dealCard(&(game->deck));
      printf("curHand was dealt: %d\n",cardValue);
      curHand->hand[1] = cardValue;
      printf("curHand: [%d, %d] nextHand: [%d, %d]\n",curHand->hand[0],curHand->hand[1],nextHand->hand[0],nextHand->hand[1]);
      curHand->numCards = 2;
      nextHand->numCards = 2;
      updateHand(curHand);
      updateHand(nextHand);
      playerTurn(game, nextHand, playerNum);
      playerTurn(game, curHand, playerNum);
    }
    else
    {
      printf("Unable to split\n");
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
  game->dealer.deck.hand[0] = dealCard(&(game->deck));
  game->dealer.deck.hand[1] = dealCard(&(game->deck));
  game->dealer.deck.numCards = 2;
}


void initGame(game_t* game)
{
  createDeck(&(game->deck));
  createDealer(&(game->dealer));
  for(int i = 0; i<3; i++)
    createPlayer(&game->players[i]);
  setTracker(&(game->tracker));
  dealTable(game);
}



void run()
{
  //all processors will run this game
  int i;
  game_t game;
  initGame(&game);
  Hand* curHand;
  for(i = 0; i<3; i++)
  {
    curHand = &(game.players[i].hands[0]);
    playerTurn(&game,curHand,i);
  }
  dealerTurn(&(game.dealer),&(game.deck));
  printf("-----Player results-----\n");
  for(int i = 0; i<3; i++)
  {
    printf("Player %d:\n",i);
    printPlayerHand(&(game.players[i]));
  }
  printf("-----\nDealer's hand: [ ");
  for(int i = 0; i<game.dealer.deck.numCards; i++)
    printf("%d ",game.dealer.deck.hand[i]);
  printf("]\n");

  gatherStats(&game);
  printStats(&game);
}









//
