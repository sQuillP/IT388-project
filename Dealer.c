#include "Dealer.h"


/*
* Serial Gang Software
* Copyright 2020
* @ William Pattison, Kevin Gomez,
* and Filip Sofeski
*/


/*Implementation file for managing dealer resources*/


/*Creates and allocates data and memory for the dealer*/
void createDealer(dealer_t* dealer)
{
  dealer->deck.hand = (int*) malloc(sizeof(int)*15);
  dealer->deck.cardTotal = 0;
  dealer->deck.numCards = 0;
  dealer->deck.aceCount = 0;
}


/*Frees allocated memory and resets dealer's
data.*/
void clearDealer(dealer_t* dealer)
{
  free(dealer->deck.hand);
  dealer->deck.cardTotal = 0;
  dealer->deck.numCards = 0;
  dealer->deck.aceCount = 0;
}


/*Debugging function, prints the cards for the dealer.*/
void printDealerHand(dealer_t* dealer)
{
  for(int i = 0; i<dealer->deck.numCards; i++)
    printf("%d ",dealer->deck.hand[i]);
  printf("\n");
}
