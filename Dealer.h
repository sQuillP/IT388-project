#include "Player.h"
#ifndef DEAL_H
#define DEAL_H
/*
* Header file for dealer functions.
*/

typedef struct Dealer
{
  Hand deck;
  bool bust;
} dealer_t;



/*Functions for the dealer*/
void createDealer(dealer_t* dealer);
void clearDealer(dealer_t* dealer);
void printDealerHand(dealer_t* dealer);

#endif
