#include "Game.h"




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
  updateDealerHand(dealer);
  if(dealer->deck.cardTotal < 17)
  {
    dealer->deck.hand[dealer->deck.numCards++] = dealCard(deck);
    DealerTurn(dealer,deck);
  }
}


/*
* TODO: Finish P1Turn
*/


/*Play a session with player 1*/
void P1Turn(deck_t* deck, dealer_t* dealer, player_t* player, int handIndex, int upCard, int turn)
{
  PlayerDecision d = player1Decide(player,0,dealer->deck.hand[0]);
  Hand* playerHand = &(player->hands[handIndex]);
  int* numCards = &(playerHand->numCards);
  if(d == HIT)
  {
    playerHand->hand[playerHand->numCards++] = dealCard(deck);
    updatePlayerHand(player, handIndex);
    if(playerHand->cardTotal>21)
      playerHand->bust = true;
    else if(playerHand->cardTotal==21)
      return;
    else
      P1Turn(deck,dealer,player,handIndex,upCard,turn+1);
  }
  else if(d == STAND)
    return;
  else if(d == DOUBLEDOWN)
  {
    playerHand->hand[playerHand->numCards++] = dealCard(deck);
    updatePlayerHand(player,handIndex);
  }
  else if(d == SPLIT)
  {
    if(turn<5)
    {
      player->hands[handIndex+1].hand[0] = dealCard(deck);
      player->hands[handIndex+1].hand[1] = dealCard(deck);
      player->hands[handIndex+1].numCards = 2;
      updatePlayerHand(player,handIndex+1);
      P1Turn(deck, dealer, player, handIndex+1, upCard, turn+1);
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
