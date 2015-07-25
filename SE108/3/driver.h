#ifndef DRIVER_H
#define DRIVER_H
#include "deck.h"
#include "hand.h"
class Driver {
  private:
    Deck deck;
    Hand hand4player, hand4dealer;
    bool checkShuffle(bool interactiveMode);
    void shuffleDeck(bool interactiveMode);
  public:
    //同时是交互和命令行方法
    Driver();
    void run(int bankroll,int hand, int playerIndex); 
};

#endif
