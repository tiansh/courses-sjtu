#include "driver.h"
#include "player.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
using std::cout;
using std::endl;

Driver::Driver() {
  hand4player.discardAll();
  hand4dealer.discardAll();
  srand((unsigned) time(NULL));
}

void Driver::shuffleDeck(bool interactiveMode) {
  cout<<endl;
  cout << "Shuffling the deck\n";
  int cut;
  for (int i=1;i<=7;i++) {
    cut=rand()%26+13;
    if (!interactiveMode) cout << "cut at " << cut << endl;
    deck.shuffle(cut);
  }
}

bool Driver::checkShuffle(bool interactiveMode) {
  if (deck.cardsLeft()<20) shuffleDeck(interactiveMode);
  else return false;
  return true;
}

void Driver::run(int bankroll,int hand, int playerIndex) {
  Player *player=get_Player(playerIndex);
  bool interactiveMode=(playerIndex==3);
  shuffleDeck(interactiveMode); player->shuffled();
  int thishand=0, wager=0;
  while (++thishand<=hand && bankroll>=5) {
    if (checkShuffle(interactiveMode)) player->shuffled();
    hand4player.discardAll(); hand4dealer.discardAll();
    cout<<endl;
    cout << "Hand " << thishand << " bankroll " << bankroll << endl;
    wager=player->bet(bankroll,5);
    cout << "Player bets " << wager << endl;
    Card card, card0, card1;
    card=deck.deal(); hand4player.addCard(card);
      player->expose(card); cout<<"Player dealt "<<card<<endl;
    card=deck.deal(); hand4dealer.addCard(card); 
      player->expose(card); cout<<"Dealer dealt "<<card<<endl; card0=card;
    card=deck.deal(); hand4player.addCard(card); 
      player->expose(card); cout<<"Player dealt "<<card<<endl;
    card=deck.deal(); hand4dealer.addCard(card);
      cout<<"Dealer dealt a hole card"<<endl; card1=card;
    if (hand4player.handValue().count==21) {
      cout << "Player dealt natural 21\n";
      bankroll+=int(wager*1.5);
    } else {
      while (hand4player.handValue().count<21 && player->draw(card0,hand4player)) {
        card=deck.deal(); hand4player.addCard(card); 
        player->expose(card); cout<<"Player dealt "<<card<<endl;
      }
      cout<<endl;
      cout << "Player's total is " << hand4player.handValue().count << endl;
      if (hand4player.handValue().count>21) {
        cout << "Player busts\n";
        bankroll-=wager;
      } else {
        cout<<endl;
        cout<<"Dealer's hole card is "<<card1<<endl; player->expose(card1);
        while (hand4dealer.handValue().count<17) {
          card=deck.deal(); hand4dealer.addCard(card); 
          player->expose(card); cout<<"Dealer dealt "<<card<<endl;
        }
        cout<<endl;
        cout << "Dealer's total is " << hand4dealer.handValue().count << endl;
        if (hand4dealer.handValue().count<=21) {
          cout<<endl;
          if (hand4dealer.handValue().count>hand4player.handValue().count) {
            cout << "Dealer wins\n"; bankroll-=wager;
          } else if (hand4dealer.handValue().count<hand4player.handValue().count) {
            cout << "Player wins\n"; bankroll+=wager;
          } else cout << "Push\n";
        } else {
          cout << "Dealer busts\n";
          bankroll+=wager;
        }
      }
    }
  }
  cout<<endl;
  cout << "Player has " << bankroll << " after " << thishand-1 << " hands\n";
} 
