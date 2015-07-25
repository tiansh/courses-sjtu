#include "card.h"
#include <iostream>
using std::cout;
const char *SuitNames[DIAMONDS+1]
  ={"Spades", "Hearts", "Clubs", "Diamonds"};

const char *SpotNames[ACE+1]
  ={"Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine", "Ten",
    "Jack", "Queen", "King", "Ace"};

ostream& operator<<(ostream & os, const Card c)
{return os<<SpotNames[c.spot]<<" of "<<SuitNames[c.suit];}

