#include "Dealer.h"
#include "Player.h"
#include <time.h>
#ifndef GAME_H
#define GAME_H


/*
* Serial Gang Software
* Copyright 2020
* @ William Pattison, Kevin Gomez,
* and Filip Sofeski
*/


/*Function declarations for game and resource management.*/


/*Structure for representing a deck in blackjack.
Cards are represented as a stack structure with "current"
pointing to the next card to be dealt*/
typedef struct Deck
{
  int* cards;
  int current;
} deck_t;


/*Structure for the entire game. game_t will hold every structure
to keep the game code organized. Contains three players.*/
typedef struct GameResources
{
  deck_t deck;
  dealer_t dealer;
  player_t players[3];
  track_t tracker;
} game_t;


/*Functions for managing the game deck. This would include shuffling,
creating the deck, dealing cards, and clearing the deck.*/
void createDeck(deck_t* deck);
void shuffleN(deck_t* deck,int n);
void clearDeck();
int dealCard(deck_t* deck);


/*Declarations for gathering statistics and data management*/
void gatherStats(game_t* game);
void recordSoft(player_t* player, dealer_t* dealer);
void recordDoubles(player_t* player, dealer_t* dealer,int hand);
void recordHard(player_t* player, dealer_t* dealer);
void printStats(int* arr, int player);
void packData(player_t* players, int* array);


/*Root functions for game mechanics*/
void newGame(game_t* game);
void playerTurn(game_t* game, Hand* curHand, int playerNum);
void dealerTurn(dealer_t* dealer, deck_t* deck);
void initGame(game_t* game);
void dealTable(game_t* game);
void printPlayerHand(player_t* player);


/*Clears the game memory when the game is finished*/
void clearMem(game_t* game);


#endif
