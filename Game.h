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


typedef struct GameResources
{
  deck_t deck;
  dealer_t dealer;
  player_t players[3];
  track_t tracker;
} game_t;



void createDeck(deck_t* deck);
void shuffleN(deck_t* deck,int n);
void clearDeck();


int dealCard(deck_t* deck);

//have a starting function that will begin the game

void printPlayerHand(player_t* player);

void playerTurn(game_t* game, Hand* curHand, int playerNum);
void DealerTurn(dealer_t* dealer, deck_t* deck);
#endif
