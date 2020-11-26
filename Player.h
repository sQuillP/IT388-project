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


typedef enum HandType
{
  SOFT,
  HARD,
  DOUBLE
} HandType;

typedef struct PlayerStatistics
{
  int softScore[2];
  int splitScore[2];
  int hardScore[2];
  int doubleDown[2];
  int push;
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
  bool doubleDown;
  HandType handType;

}Hand;


/*Player Structure template for each player type*/
typedef struct Player
{
  Hand hands[4];
  stats_t STATS;
  bool canSplit;
  bool didSplit;
} player_t;



void updateTracker(track_t* tracker);
void setTracker(track_t* tracker);

/*Generic methods for Player structure*/
void updateHand(Hand* deck);
void createPlayer(player_t *player);
void createStats(stats_t* STATS);

/*Functions for Player 1*/
PlayerDecision player1Decide(player_t* player, Hand* pCards, int upCard);
PlayerDecision P1HardHand(int playerTotal, int upCard);
PlayerDecision P1SoftHand(int pCard, int upCard);
PlayerDecision P1Doubles(int pCard, int upCard);



/*Functions for Player 2*/
PlayerDecision player2Decide(player_t* player, Hand* pCards, int upCard);
PlayerDecision P2HardHand(int playerTotal, int upCard);
PlayerDecision P2SoftHand(int pCard, int upCard);
PlayerDecision P2Doubles(int pCard, int upCard);




/*Function for Player 3*/
PlayerDecision player3Decide(player_t* player, Hand* pCards, int upCard);
PlayerDecision P3HardHand(int playerTotal, int upCard);
PlayerDecision P3SoftHand(int pCard, int upCard);
PlayerDecision P3Doubles(int pCard, int upCard);






#endif
