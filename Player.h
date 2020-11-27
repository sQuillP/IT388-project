#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#ifndef PLAYER_H
#define PLAYER_H


/*
* Serial Gang Software
* Copyright 2020
* @ William Pattison, Kevin Gomez,
* and Filip Sofeski
*/


/*Header file for player functionality and data structures*/


/*Datatype for player's decisions when they are
being dealt cards.*/
typedef enum PlayerDecision
{
  HIT,
  STAND,
  SPLIT,
  DOUBLEDOWN
} PlayerDecision;


/*HandType will define what kind of hand the player has based
on the first two cards they receive.*/
typedef enum HandType
{
  SOFT,
  HARD,
  DOUBLE
} HandType;


/*Contains information about a player's game. This includes
information about their wins/losses with the hand they are given
at the start of the round.*/
typedef struct PlayerStatistics
{
  int softScore[2];
  int splitScore[2];
  int hardScore[2];
  int doubleDown[2];
  int push;
} stats_t;


/*PlayerTrack will make sure that the player will not split
more than 3 times, and will also make sure that cards will be placed
correctly into the player's decks.*/
typedef struct PlayerTrack
{
  int handIndex;
  int splitNum;
} track_t;


/*Structure for a "hand" in blackjack. This will hold all
the attributes about a player's hand*/
typedef struct PlayerHand
{
  int* hand;
  int cardTotal;
  int numCards;
  int aceCount;
  bool doubleDown;
  HandType handType;
}Hand;


/*Player structure type. This will create an instance of a player
in blackjack. Each player has 4 hands since they are able to split their
cards up to 3 times.*/
typedef struct Player
{
  Hand hands[4];
  stats_t STATS;
  bool canSplit;
  bool didSplit;
} player_t;


/*Functions for keeping track of how many times a player can split*/
void updateTracker(track_t* tracker);
void setTracker(track_t* tracker);

/*Generic methods for handling player structure and their stats*/
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


/*Functions for Player 3*/
PlayerDecision player3Decide(player_t* player, Hand* pCards, int upCard);
PlayerDecision P3HardHand(int playerTotal, int upCard);
PlayerDecision P3SoftHand(int pCard, int upCard);
PlayerDecision P3Doubles(int pCard, int upCard);


#endif
