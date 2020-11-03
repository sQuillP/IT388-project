#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#ifndef PLAYER_H
#define PLAYER_H

/*Header file for Player functions and data structures*/

typedef struct PlayerStatistics
{
  int score[2];
  //more data about player
} PStats;


/*Player Structure template for each player type*/
typedef struct Player
{

  int* cards;
  int* secondDeck;

  int cash;
  int hand;
  int numCards;

  PStats STATS;
  bool fold;

} player_t;



/*Data structure for player decisions. A blackjack player can either hit,
stand, split, or double down given the current game circumstances.*/
typedef enum PlayerDecision
{
  HIT,
  STAND,
  SPLIT,
  DOUBLEDOWN
} Decision;

/*Generic methods for Player structure*/
void updateHand(player_t *p);
void clearHand(player_t *p);
void createPlayer(player_t *player);


/*Functions for Player 1*/



/*Functions for Player 2*/
Decision player2Bid(player_t *player, int dealerCount);





/*Function for Player 3*/

#endif
