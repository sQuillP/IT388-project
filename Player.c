#include "Player.h"

/*Implementation file for player*/





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


/*Reset the players hand*/
void clearHand(int* hand)
{


}

void updateTracker(track_t* tracker)
{
  tracker->handIndex++;
  tracker->splitNum++;
}

void setTracker(track_t* tracker)
{
  tracker->handIndex = 0;
  tracker->splitNum = 0;
}


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
}


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
    createStats(&(player->STATS));
  }
  player->canSplit = true;
  player->didSplit = false;
}


/*Clears the hand of the player*/
//might not need this method
void clearPlayer(player_t *player)
{
  for(int i = 0; i<3; i++)
    free(player->hands[i].hand);

}

int getAce(int* deck)
{
  if(deck[0] == 1)
    return 1;
  else if(deck[1] == 1)
    return 0;
  else
    return -1;
}

//player is dealt two cards
//player looks at cards and updates hand
//then dealer asks player to hit, stand, double, or split.

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




/*Player makes a decision to hit, stand, doubledown, or split after looking
at the dealer's up card and the cards that are currently in their hand*/
PlayerDecision player1Decide(player_t* player, Hand* pCards, int upCard)
{
  int ace = getAce(pCards->hand);
  if(pCards->numCards == 2)
  {
    if(pCards->hand[0] == pCards->hand[1]&& player->canSplit)
     return P1Doubles(pCards->hand[0],upCard);
    else if(ace != -1)
      return P1SoftHand(ace,upCard);
  }
    return P1HardHand(pCards->cardTotal,upCard);
}



PlayerDecision player2Decide(player_t* player, Hand* pCards, int upCard)
{
  int ace = getAce(pCards->hand);
  if(pCards->numCards == 2)
  {
    if(pCards->hand[0] == pCards->hand[1]&& player->canSplit)
     return P2Doubles(pCards->hand[0],upCard);
    else if(ace != -1)
      return P2SoftHand(ace,upCard);
  }
  return P2HardHand(pCards->cardTotal,upCard);
}


PlayerDecision player3Decide(player_t* player, Hand* pCards, int upCard)
{
  int ace = getAce(pCards->hand);
  if(pCards->numCards == 2)
  {
    if(pCards->hand[0] == pCards->hand[1]&& player->canSplit)
     return P3Doubles(pCards->hand[0],upCard);
    else if(ace != -1)
      return P3SoftHand(ace,upCard);
  }
    return P3HardHand(pCards->cardTotal,upCard);
}



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

PlayerDecision P3SoftHand(int pCard, int upCard)
{
  if(pCard >=7 && pCard <= 9)
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

PlayerDecision P3Doubles(int pCard, int upCard)
{
  if(pCard == 10||pCard == 9)
    return STAND;
  else if(pCard == 8)
  {
    if(upCard>2)
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
  else //less than or equal to 8
    return HIT;
}

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










//
