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
void gatherStats(game_t* game);

void recordSoft(player_t* player, dealer_t* dealer);
void recordDoubles(player_t* player, dealer_t* dealer,int hand);
void recordHard(player_t* player, dealer_t* dealer);

void printStats(game_t* game);
void printPlayerHand(player_t* player);
void playerTurn(game_t* game, Hand* curHand, int playerNum);
void dealerTurn(dealer_t* dealer, deck_t* deck);
void initGame(game_t* game);
void dealTable(game_t* game);
/*Root function to run a game. Runs a single blackjack
simulation with 3 players*/
void run();



#endif
