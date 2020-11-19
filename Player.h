#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#ifndef PLAYER_H
#define PLAYER_H

/*Header file for Player functions and data structures*/


/*Data structure for player decisions. A blackjack player can either hit,
stand, split, or double down given the current game circumstances.*/
typedef enum PlayerDecision
{
  HIT,
  STAND,
  SPLIT,
  DOUBLEDOWN
} PlayerDecision;



typedef struct PlayerStatistics
{
  /*Player data*/

} PStats;


typedef struct PlayerHand
{
  int* hand;
  int cardTotal;
  int numCards;
  int aceCount;
}Hand;


/*Player Structure template for each player type*/
typedef struct Player
{
  int cash;
  Hand hands[4];
  bool bust;
  PStats STATS;
} player_t;


/*Generic methods for Player structure*/
void updateHand(player_t *p, int inxex);
void clearHand(int* hand);
void createPlayer(player_t *player);


/*Functions for Player 1*/




/*Functions for Player 2*/
PlayerDecision player1Decide(player_t* player, int handIndex, int upCard);
PlayerDecision P1HardHand(int playerTotal, int upCard);
PlayerDecision P1SoftHand(int pCard, int upCard);
PlayerDecision P1Doubles(int pCard, int upCard);



/*Function for Player 3*/

#endif
