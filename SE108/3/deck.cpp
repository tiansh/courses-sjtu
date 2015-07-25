#include "deck.h"
#define max(x,y) (((x)>(y))?(x):(y))
#define min(x,y) (((x)<(y))?(x):(y))
Deck::Deck() {
  reset();
}
void Deck::reset() {
  int k=0;
  for (int i=0;i<=12;i++)
   for (int j=0;j<=3;j++)
    deck[ k ].spot=(enum Spot)(i),
    deck[k++].suit=(enum Suit)(j);
  next=0;
}
void Deck::shuffle(int n) {
  Deck l,r;
  if (n<1||n>51) return;
  int i,j=2*min(n,DeckSize-n);
  for (i=0;i<n;i++) l.deck[i]=deck[i];
  for (;i<DeckSize;i++) r.deck[i-n]=deck[i];
  for (i=0;i<j;i++)
    deck[i]=(((i)&1)?l:r).deck[i/2];
  for (i--;j<DeckSize;j++)
    deck[j]=((n>DeckSize-n)?l:r).deck[j-i+i/2];
  next=0;
}
Card Deck::deal() {
  DeckEmpty deckEmpty;
  if (next==DeckSize) throw(deckEmpty);
  else return deck[next++];
}
int Deck::cardsLeft()
  {return DeckSize-next;}
