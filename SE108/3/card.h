#ifndef CARD_H
#define CARD_H
#include <ostream>
using std::ostream;
enum Suit {
  SPADES, HEARTS, CLUBS, DIAMONDS
};
extern const char *SuitNames[DIAMONDS+1];

enum Spot {
  TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN,
  JACK, QUEEN, KING, ACE
};
extern const char *SpotNames[ACE+1];

struct Card {
  //点数
  //值从TWO到ACE的枚举类型
  Spot spot;

  //花色
  //值从SPADES到DIAMONDS的枚举类型
  Suit suit;
};

ostream& operator<<(ostream & os, const Card c);

#endif
