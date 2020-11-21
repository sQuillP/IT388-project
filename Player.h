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

} stats_t;


typedef struct PlayerTrack
{
  int handIndex;
  int splitNum;
} track_t;


typedef struct PlayerHand
{
  int* hand;
  int cardTotal;
  int numCards;
  int aceCount;
  bool bust;
}Hand;


/*Player Structure template for each player type*/
typedef struct Player
{
  int cash;
  Hand hands[3];
  stats_t STATS;
  bool canSplit;
} player_t;



void updateTracker(track_t* tracker);
void setTracker(track_t* tracker);

/*Generic methods for Player structure*/
void updateHand(Hand* deck);
void clearHand(int* hand);
void createPlayer(player_t *player);


/*Functions for Player 1*/
PlayerDecision player1Decide(player_t* player, Hand* pCards, int upCard);
PlayerDecision P1HardHand(int playerTotal, int upCard);
PlayerDecision P1SoftHand(int pCard, int upCard);
PlayerDecision P1Doubles(int pCard, int upCard);



/*Functions for Player 2*/






/*Function for Player 3*/







#endif
