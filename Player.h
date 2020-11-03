#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#ifndef PLAYER_H
#define PLAYER_H

/*Header file for Player functions and data structures*/

typedef struct PlayerStatistics
{
  /*Player data*/

} PStats;


/*Player Structure template for each player type*/
typedef struct Player
{

  int* cards;
  int* splitDeck;

  int cash;
  int cardTotal;
  int aceCount;
  int numCards;

  PStats STATS;
  bool bust;

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
int pair(player_t *player);

/*Functions for Player 1*/


/*Functions for Player 2*/
Decision player2Decide(player_t *player, int dealerCount);





/*Function for Player 3*/

#endif
