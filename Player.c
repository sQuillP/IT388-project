#include "Player.h"


/*
* Serial Gang Software
* Copyright 2020
* @ William Pattison, Kevin Gomez,
* and Filip Sofeski
*/


/*Implementation file for player*/


/*Updates the players hand after they are dealt a card. Players hand will always
have the best score possible given their current hand.*/
void updateHand(Hand* deck)
{
  int sum = 0, aceCount = 0;
  for(int i = 0; i<deck->numCards; i++)
  {
    if(deck->hand[i]!=1)
      sum += deck->hand[i];
    else
      aceCount++;
  }
  deck->aceCount = aceCount;
  if(deck->aceCount>0&&(sum+11+deck->aceCount)-1<=21)
    deck->cardTotal = (sum+11+deck->aceCount)-1;
  else
    deck->cardTotal = sum + deck->aceCount;
}


/*After a recursive call is used when a player splits, the tracker
will update the count and point to the next hand index that their split
cards will go to. This is used for making sure the player splits no more than
three times.*/
void updateTracker(track_t* tracker)
{
  tracker->handIndex++;
  tracker->splitNum++;
}


/*Reset or initilize the tracker to zero. Method is always
called after a player's turn is finished.*/
void setTracker(track_t* tracker)
{
  tracker->handIndex = 0;
  tracker->splitNum = 0;
}


/*Create and set the player's statistics to be updated
later on.*/
void createStats(stats_t* STATS)
{
  int i;
  for(i = 0; i<2; i++)
  {
    STATS->softScore[i] = 0;
    STATS->splitScore[i] = 0;
    STATS->hardScore[i] = 0;
    STATS->doubleDown[i] = 0;
  }
  STATS->push = 0;
  STATS->natural21 = 0;
}


/*Creates and allocates resources for a player*/
void createPlayer(player_t *player)
{
  int i;
  for( i = 0; i<4; i++)
  {
    player->hands[i].hand = (int*) malloc(sizeof(int)*15);
    player->hands[i].cardTotal = 0;
    player->hands[i].numCards = 0;
    player->hands[i].aceCount = 0;
    player->hands[i].doubleDown = false;
  }
  player->canSplit = true;
  player->didSplit = false;
}


/*Returns the index of the non-ace card if there
is a soft hand. Returns -1 if there is no ace in
the player's first 2 cards.*/
int getAce(int* deck)
{
  if(deck[0] == 1)
    return 1;
  else if(deck[1] == 1)
    return 0;
  else
    return -1;
}


/*Used for debugging. Prints the contents of all the player's
cards.*/
void printPlayerHand(player_t* player)
{
  int n;
  for(int i = 0; i<4; i++)
  {
    n = player->hands[i].numCards;
    printf("DECK %d: ",i);
    for(int j = 0; j<n; j++)
    {
      printf("%d ",player->hands[i].hand[j]);
    }
    printf("\tTotal: %d \n",player->hands[i].cardTotal);
  }
}


/*Function for player 2's decision given dealer's up card and their current hand*/
PlayerDecision player1Decide(player_t* player, Hand* pCards, int upCard)
{
  int ace = getAce(pCards->hand);
  if(pCards->numCards == 2)
  {
    if(pCards->hand[0] == pCards->hand[1]&& player->canSplit)
     return P1Doubles(pCards->hand[0],upCard);
    else if(ace != -1)
      return P1SoftHand(pCards->hand[ace],upCard);
  }
    return P1HardHand(pCards->cardTotal,upCard);
}


/*Function for player 2's decision given dealer's up card and their current hand*/
PlayerDecision player2Decide(player_t* player, Hand* pCards, int upCard)
{
  int ace = getAce(pCards->hand);
  if(pCards->numCards == 2)
  {
    if(pCards->hand[0] == pCards->hand[1]&& player->canSplit)
     return P2Doubles(pCards->hand[0],upCard);
    else if(ace != -1)
      return P2SoftHand(pCards->hand[ace],upCard);
  }
  return P2HardHand(pCards->cardTotal,upCard);
}


/*Function for player 3's decision given dealer's up card and their current hand*/
PlayerDecision player3Decide(player_t* player, Hand* pCards, int upCard)
{
  int ace = getAce(pCards->hand);
  if(pCards->numCards == 2)
  {
    if(pCards->hand[0] == pCards->hand[1]&&player->canSplit)
     return P3Doubles(pCards->hand[0],upCard);
    else if(ace != -1)
      return P3SoftHand(pCards->hand[ace],upCard);
  }
    return P3HardHand(pCards->cardTotal,upCard);
}


/*Hard hand table implementation for player 3.*/
PlayerDecision P3HardHand(int pCard, int upCard)
{
  if(pCard >= 17)
    return STAND;
  else if(pCard <=16 && pCard >=14)
  {
    if(upCard >=2 && upCard <=6)
      return STAND;
    else
      return HIT;
  }
  else if(pCard == 13 || pCard == 12)
  {
    if(upCard >=4 && upCard <=6)
      return STAND;
    else
      return HIT;
  }
  else if(pCard == 10 || pCard == 11)
  {
    if(upCard >=2 && upCard <= 9)
      return DOUBLEDOWN;
    else
      return HIT;
  }
  else
    return HIT;
}


/*Soft hand table for player 3*/
PlayerDecision P3SoftHand(int pCard, int upCard)
{
  if(pCard >=7 && pCard <= 10)
    return STAND;
  else if(pCard == 6)
  {
    if(upCard >= 4 && upCard <=7)
      return STAND;
    else
      return HIT;
  }
  else
    return HIT;
}


/*Decision table for player 3 if there is two of the same card*/
PlayerDecision P3Doubles(int pCard, int upCard)
{
  if(pCard == 10||pCard == 9)
    return STAND;
  else if(pCard == 8)
  {
    if(upCard>=2)
      return SPLIT;
    else
      return HIT;
  }
  else if(pCard == 7 || pCard == 6 || pCard == 4)
    return HIT;
  else if(pCard == 5)
  {
    if(upCard >=2 && upCard <=9)
      return DOUBLEDOWN;
    else
      return HIT;
  }
  else if(pCard == 3)
  {
    if(upCard >=2 && pCard <= 7)
      return SPLIT;
    else
      return HIT;
  }
  else if(pCard == 2)
  {
    if(upCard >=2 && upCard <=6)
      return SPLIT;
    else
      return HIT;
  }
  else
    return SPLIT;
}


/*Hard hand table implementation for player 1.*/
PlayerDecision P2HardHand(int pCard, int upCard)
{
  if(pCard>=17)
    return STAND;
  else if(pCard <= 16 && pCard >= 13)
  {
    if(upCard >=2 && upCard <=6)
      return STAND;
    else
      return HIT;
  }
  else if(pCard == 12)
  {
    if(upCard >=4 && upCard <= 6)
      return STAND;
    else
      return HIT;
  }
  else if(pCard == 11)
  {
    if(upCard == 1)
      return HIT;
    else
      return DOUBLEDOWN;
  }
  else if(pCard == 10)
  {
    if(upCard >=2 && upCard <= 9)
      return DOUBLEDOWN;
    else
      return HIT;
  }
  else if(pCard == 9)
  {
    if(upCard >= 3 && upCard <= 6)
      return DOUBLEDOWN;
    else
      return HIT;
  }
  else
    return HIT;
}


/*Soft hand table implementation for player 2.*/
PlayerDecision P2SoftHand(int pCard, int upCard)
{
  if(pCard >=8 && pCard <=10)
    return STAND;
  else if(pCard == 7)
  {
    if(upCard >=3 && upCard <= 6)
      return DOUBLEDOWN;
    else if(upCard == 2 || upCard == 6 || upCard == 7)
      return STAND;
    else
      return HIT;
  }
  else if(pCard == 6)
  {
    if(upCard >=3 && upCard <=6)
      return DOUBLEDOWN;
    else
      return HIT;
  }
  else if(pCard == 5 || pCard == 4)
  {
    if(upCard >=4 && upCard <=6)
      return DOUBLEDOWN;
    else
      return HIT;
  }
  else if(pCard <=3)
  {
    if(upCard ==5 || upCard == 6)
      return DOUBLEDOWN;
    else
      return HIT;
  }
}


/*Player 2 table implementation if there is two of the
same card.*/
PlayerDecision P2Doubles(int pCard, int upCard)
{
  if(pCard == 10)
    return STAND;
  else if(pCard == 9)
  {
    if(upCard == 6 || upCard == 10 || upCard == 1)
      return STAND;
    else
      return SPLIT;
  }
  else if(pCard == 8)
    return SPLIT;
  else if(pCard == 7)
  {
    if(upCard >=2 && upCard <=7)
      return SPLIT;
    else
      return HIT;
  }
  else if(pCard == 6)
  {
    if(upCard >= 2 && upCard <=6)
      return SPLIT;
    else
      return HIT;
  }
  else if(pCard == 5)
  {
    if(upCard >=2 && upCard <=9)
      return DOUBLEDOWN;
    else
      return HIT;
  }
  else if(pCard == 4)
  {
    if(upCard == 5 || upCard == 6)
      return SPLIT;
    else
      return HIT;
  }
  if(pCard == 3 || pCard == 2)
  {
    if(upCard >=2 && upCard <=7)
      return SPLIT;
    else
      return HIT;
  }
  else
    return SPLIT;

}


/*Player 1 table implementation for a hard hand
(no aces or doubles).*/
PlayerDecision P1HardHand(int pCard, int upCard)
{
  if(pCard >= 17)
    return STAND;
  else if(pCard <=16 && pCard >= 13)
  {
    if(upCard>=2&&upCard<=6)
      return STAND;
    else
      return HIT;
  }
  else if(pCard == 12)
  {
    if(upCard >= 4 && upCard <= 6)
      return STAND;
    else
      return HIT;
  }
  else if(pCard == 11)
  {
    if(upCard >=2 && upCard <=10)
      return DOUBLEDOWN;
    else
      return HIT;
  }
  else if(pCard  == 10)
  {
    if(upCard >= 2 && upCard <=9)
      return DOUBLEDOWN;
    else
      return HIT;
  }
  else if(pCard == 9)
  {
    if(upCard >=3 && upCard <=6)
      return DOUBLEDOWN;
    else
      return HIT;
  }
  else
    return HIT;
}


/*Player 1 table for when there is a soft hand*/
PlayerDecision P1SoftHand(int pCard, int upCard)
{
  if(pCard >=8 && pCard <=10)
    return STAND;
  else if(pCard == 7)
  {
    if(upCard >= 3&&upCard <=6)
      return DOUBLEDOWN;
    else if(upCard == 2 || upCard == 7 || upCard == 8)
      return STAND;
    else
      return HIT;
  }
  else if(pCard == 6)
  {
    if(upCard == 2)
      return STAND;
    else if(upCard >=3 && upCard <=6)
      return DOUBLEDOWN;
    else
      return HIT;
  }
  else if(pCard == 5 || pCard == 4)
  {
    if(upCard >= 4 && upCard <=6)
      return DOUBLEDOWN;
    else
      return HIT;
  }
  else if(upCard == 5 || upCard == 6)
    return DOUBLEDOWN;
  else
    return HIT;
}


/*Player 1 table if there is two of the same card
when the first two cards are dealt.*/
PlayerDecision P1Doubles(int pCard, int upCard)
{
  if(pCard == 1 || pCard == 8)
    return SPLIT;
  else if(pCard == 10)
    return STAND;
  else if(pCard == 9)
  {
    if(upCard == 7 || upCard == 10 || upCard == 1)
      return STAND;
    else
      return SPLIT;
  }
  else if(pCard == 7)
  {
    if(upCard >=2 && upCard <=7)
      return SPLIT;
    else
      return HIT;
  }
  else if(pCard == 6)
  {
    if(upCard >=2 && upCard <= 6)
      return SPLIT;
    else
      return HIT;
  }
  else if(pCard == 5)
  {
    if(upCard >=2 && upCard <=9)
      return DOUBLEDOWN;
    else
      return HIT;
  }
  else if(pCard == 4)
  {
    if(upCard == 5 || upCard == 6)
      return SPLIT;
    else
      return HIT;
  }
  else if(upCard >= 2 && upCard <= 7)
    return SPLIT;
  else
    return HIT;
}
