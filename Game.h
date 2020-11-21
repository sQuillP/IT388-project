#include "Dealer.h"
#include "Player.h"
#include <time.h>
#ifndef GAME_H
#define GAME_H

typedef struct Deck
{
  int* cards;
  int current;
} deck_t;



void createDeck(deck_t* deck);
void shuffle(deck_t* deck);
void clearDeck();


int dealCard(deck_t* deck);

//have a starting function that will begin the game



void P1Turn(deck_t* deck, dealer_t* dealer, player_t* player, int handNum, int upCard, int turn);
void P2Turn(player_t* player, int handNum, int upCard);
void P3Turn(player_t* player, int handNum, int upCard);

void DealerTurn(dealer_t* dealer, deck_t* deck);
#endif
