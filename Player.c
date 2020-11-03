#include "Player.h"

/*Implementation file for player*/

/*Update the current card count of the player*/
void updateHand(player_t *player)
{
  int sum = 0;
  for(int i = 0; i<player->numCards; i++)
    if(player->cards[i]!=1)
      sum += player->cards[i];
  if(player->aceCount>0&&(sum+11+player->aceCount)-1<=21)
    player->cardTotal = (sum+11+player->aceCount)-1;
  else
    player->cardTotal = sum + player->aceCount;
}


/*Reset the players hand*/
void clearHand(player_t *player)
{
  free(player->cards);
  player->cards = (int*)malloc(sizeof(int)*15);
}


/*simulate game that is not the same as the other one */
void createPlayer(player_t *player)
{
  player->cards = malloc(sizeof(int)*15);
  player->cash = 1000;
  player->cardTotal = 0;
  player->aceCount = 0;
  player->bust = false;
}
