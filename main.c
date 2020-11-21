#include "Game.h"





int main()
{

  game_t game;

  createDeck(&(game.deck));
  createDealer(&(game.dealer));
  for(int i = 0; i<3 ; i++)
    createPlayer(&(game.players[i]));
  setTracker(&(game.tracker));
  game.dealer.deck.hand[0] = dealCard(&(game.deck));
  game.players[0].hands[0].hand[0] = dealCard(&(game.deck));
  game.players[0].hands[0].hand[1] = dealCard(&(game.deck));
  game.players[0].hands[0].numCards = 2;
  updateHand(&(game.players[0].hands[0]));

  printf("Before\nDealer's up card %d\n",game.dealer.deck.hand[0]);
  printPlayerHand(&(game.players[0]));
  printf("--------\n");

  playerTurn(&game,&(game.players[0].hands[0]),0);


  printPlayerHand(&(game.players[0]));


  // printf("Hand Count: %d\n",test.hands[0].cardTotal);
  // printf("Value: %d \n",(int)player1Decide(&test,0,1));

  return 0;
}
