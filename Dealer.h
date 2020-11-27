#include "Player.h"
#ifndef DEAL_H
#define DEAL_H


/*
* Serial Gang Software
* Copyright 2020
* @ William Pattison, Kevin Gomez,
* and Filip Sofeski
*/


/*Structure for the dealer, only contains a single hand*/
typedef struct Dealer
{
  Hand deck;
} dealer_t;


/*Functions for the dealer*/
void createDealer(dealer_t* dealer);
void clearDealer(dealer_t* dealer);
void printDealerHand(dealer_t* dealer);


#endif
