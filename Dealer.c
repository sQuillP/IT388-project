#include "Dealer.h"




void updateDealerHand(dealer_t *dealer)
{
  int sum = 0, aceCount = 0;
  Hand* deck = &(dealer->deck);
  for(int i = 0; i<deck->numCards; i++)
  {
    if(deck->hand[i]!=1)
      sum += deck->hand[i];
    else
      aceCount++;
  }
  deck->aceCount = aceCount;
  if(deck->aceCount>0&&(sum+11+deck->aceCount)-1<=21)
    deck->cardTotal = (sum+11+deck->aceCount)-1;
  else
    deck->cardTotal = sum + deck->aceCount;
}



void createDealer(dealer_t* dealer)
{
  dealer->deck.hand = (int*) malloc(sizeof(int)*15);
  dealer->deck.cardTotal = 0;
  dealer->deck.numCards = 0;
  dealer->deck.aceCount = 0;
  dealer->bust = false;
}



void clearDealer(dealer_t* dealer)
{
  free(dealer->deck.hand);
  dealer->deck.cardTotal = 0;
  dealer->deck.numCards = 0;
  dealer->deck.aceCount = 0;
  dealer->bust = false;
}

void printDealerHand(dealer_t* dealer)
{
  for(int i = 0; i<dealer->deck.numCards; i++)
    printf("%d ",dealer->deck.hand[i]);
  printf("\n");
}
