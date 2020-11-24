#include "Dealer.h"




void createDealer(dealer_t* dealer)
{
  dealer->deck.hand = (int*) malloc(sizeof(int)*15);
  dealer->deck.cardTotal = 0;
  dealer->deck.numCards = 0;
  dealer->deck.aceCount = 0;
}



void clearDealer(dealer_t* dealer)
{
  free(dealer->deck.hand);
  dealer->deck.cardTotal = 0;
  dealer->deck.numCards = 0;
  dealer->deck.aceCount = 0;
}

void printDealerHand(dealer_t* dealer)
{
  for(int i = 0; i<dealer->deck.numCards; i++)
    printf("%d ",dealer->deck.hand[i]);
  printf("\n");
}






//
