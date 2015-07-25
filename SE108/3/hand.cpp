#include "hand.h"
Hand::Hand() {
  discardAll();
}
void Hand::discardAll() {
  curValue.count=0;
  curValue.soft=false;
}
void Hand::addCard(Card c) {
  int n=(c.spot>8)?10:c.spot+2;
  if (c.spot==ACE)
    if ((curValue.count+=11)>21) curValue.count-=10;
    else curValue.soft=true;
  else curValue.count+=n;
  if (curValue.soft && curValue.count>21) 
    curValue.count-=10, curValue.soft=false;
}
HandValue Hand::handValue() const {
  return curValue;
}
