#include "player.h"
#include <iostream>
#include <algorithm>
using std::cin; using std::cout; using std::endl;
using std::min; using std::max;

//简单玩家
extern Player *get_Simple()
{return &simplePlayer;}
int Simple::bet(unsigned int, unsigned int minimum) 
  { return minimum;}
bool Simple::draw(Card dealer, const Hand &player){
  if (!player.handValue().soft) {
    if (player.handValue().count<=11) return true;
    else if (player.handValue().count==12) {
      if (dealer.spot/3==1) return true;
    } else if (player.handValue().count<17) {
      if (dealer.spot<5) return true;
    }
  } else {
    if (player.handValue().count<=17) return true;
    else if (player.handValue().count==18 && !(dealer.spot==2 ||
             dealer.spot==7 || dealer.spot==8)) return true;
  }
  return false;
}
void Simple::expose(Card){} //有意留空
void Simple::shuffled(){}   //有意留空

//数牌玩家
extern Player *get_Counting()
{return &countingPlayer;}
Counting::Counting() { count=0;}
int Counting::bet(unsigned int bankroll, unsigned int minimum) {
  if (count>=2) return min(minimum*2,bankroll);
  return minimum;
}
void Counting::expose(Card c) {
  if (c.spot>=8) count--;
  else if (c.spot<=4) count++;
}
void Counting::shuffled() {count=0;}

//竞争者
extern Player *get_Competitor() {return &competitorPlayer;}
Competitor::Competitor(){shuffled();}
int Competitor::bet(unsigned int bankroll, unsigned int minimum){
  int r=0,i,total=0;
  for (i=0;i<13;i++) total+=rest[(enum Spot)(i)];
  r=int(minimum*double(count-1)/total*52+0.5);
  if (r<int(minimum)) r=minimum;
  if (bankroll<r+minimum) r=bankroll;
  return r;
}
void Competitor::expose(Card c){
  rest[c.spot]--;
  if (c.spot>=8) count--;
  else if (c.spot<=4) count++;
}
void Competitor::shuffled(){
  for (int i=0;i<13;i++) rest[(enum Spot)(i)]=4;
  count=0;
}
bool Competitor::draw(Card dealer, const Hand &player){
  if (player.handValue().count<=11) return true;
  if (player.handValue().soft && player.handValue().count<16) return true;
  return (pj(dealer, player)>pg(dealer, player));
}
double Competitor::pj(Card dealer, const Hand &player){
  int i,total=0; Card c; c.suit=SPADES; Hand h;
  double e=0;
  for (i=0;i<13;i++) total+=rest[(enum Spot)(i)];
  for (i=0;i<13;i++) if (rest[(enum Spot)(i)]) {
    c.spot=(enum Spot)(i); rest[(enum Spot)(i)]--;
    h=player; h.addCard(c);
    if (h.handValue().count<=21) 
      e+=double(rest[(enum Spot)(i)]+1)/total*max(pj(dealer,h),pg(dealer,h));
    else e+=-1.02*double(rest[(enum Spot)(i)]+1)/total;
    rest[(enum Spot)(i)]++;
  }
  return e;
}
double Competitor::pg(Card dealer, const Hand &player){
  Hand h; h.discardAll(); h.addCard(dealer);
  return pg(h,player);
}
double Competitor::pg(Hand dealer, const Hand &player){
  double e=0; int i, total=0; Card c; c.suit=SPADES;
  Hand h;
  for (i=0;i<13;i++) total+=rest[(enum Spot)(i)];
  for (i=0;i<13;i++) if (rest[(enum Spot)(i)]) {
    c.spot=(enum Spot)(i); rest[(enum Spot)(i)]--;
    h=dealer; h.addCard(c);
    if (h.handValue().count<17)
      e+=double(rest[(enum Spot)(i)]+1)/total*pg(h,player);
    else if (h.handValue().count<=21)
      e+=double(rest[(enum Spot)(i)]+1)/total*
         ((h.handValue().count>player.handValue().count)?-1.02:
         ((h.handValue().count<player.handValue().count)?1.:0));
    else e+=double(rest[(enum Spot)(i)]+1)/total;
    rest[(enum Spot)(i)]++;
  }
  return e;
}

//人机交互
extern Player *get_Interactive()
{return &interactivePlayer;}
int Interactive::bet(unsigned int bankroll, unsigned int minimum) {
  cout<<endl;
  unsigned int n;
  do {
    cout<<"Player bets (between "<<minimum<<" and "<<bankroll<<") :";
    cin>>n; cin.ignore(1024,'\n');
  } while (n<minimum || n>bankroll);
  return n;
}
bool Interactive::draw(Card dealer, const Hand &player){
  cout<<endl;
  cout<<"Dealer shows :"<<dealer<<endl;
  cout<<"This hand :"<<player.handValue().count<<" ("
      <<((player.handValue().soft)?("Soft"):("Hard"))<<")"<<endl;
  cout<<"Your choice :\n  [H]its\n  [S]tand\n  >";
  char c;
  do if (!(cin>>c)) cin.ignore(1024,'\n'); while (!strchr("HShs",c));
  if (c=='H' || c=='h') return true;
  else return false;
}
void Interactive::expose(Card){} //有意留空
void Interactive::shuffled(){}   //有意留空

//Player汇总
extern Player *get_Player(int n) {
  switch (n) {
    case 0 :return get_Simple();
    case 1 :return get_Counting();
    case 2 :return get_Competitor();
    default:return get_Interactive();
  }
}
