#include "Game.h"



/*
-potential program flaws/inconsistencies:
 how the P1 code is implemented with keeping track of recursion
 with how the player wants to split their cards. (using the track_t struct
 to track how many times a player can split their cards).

- overall program structure, not the best but it will hopefully make the code work.
the plan is to make it modular for parallelization.


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
  shuffle(deck);
  shuffle(deck);
}



void shuffle(deck_t* deck)
{
  int temp, index;
  for(int i = 0 ;i<312; i++)
  {
    index = rand()%312;
    temp = deck->cards[i];
    deck->cards[i] = deck->cards[index];
    deck->cards[index] = temp;
  }
}



void DealerTurn(dealer_t* dealer, deck_t* deck)
{
  updateHand(&(dealer->deck));
  if(dealer->deck.cardTotal < 17)
  {
    dealer->deck.hand[dealer->deck.numCards++] = dealCard(deck);
    DealerTurn(dealer,deck);
  }
}



void playerTurn(game_t* game, Hand* curHand,  int playerNum)
{
  player_t* player = &(game->players[playerNum]);
  track_t* tracker = &(game->tracker);
  dealer_t* dealer = &(game->dealer);
  int cardValue;
  Hand* nextHand;
  PlayerDecision d = player1Decide(player, curHand, dealer->deck.hand[0]);
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





//
