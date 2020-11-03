#include <stdlib.h>
#include <stdio.h>


typedef struct Player
{
  int* cards;
  int* secondDeck;
  int cash;
  int hand;
  int numCards;
  bool fold;
} player_t;


int calculateHand(player_t *p);
int clearHand(player_t *p);
void createPlayer(player_t *player);
void player2Bet(player_t *player);
void hit(player_t *player);
void stand(player_t *player);
void doubleDown(player_t *player);


//
