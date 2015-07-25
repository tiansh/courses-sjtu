#include "driver.h"
#include <iostream>
using std::cout;

Driver driver;

void printUseage() {
  cout<<"USEAGE: \n   blackjack [-b] <bankroll>"
      <<" <hand> [simple|counting|competitor]\n";
}
bool params(int n, char* s[], int &bankroll, int &hand, int &playerIndex) {
  int i, a[2], t=0, u=0;
  playerIndex=-1; bool userInterface=false;
  for (i=1;i<n;i++) 
    if (t=atoi(s[i])) if (u<2) a[u++]=t; else return false;
    else if (!stricmp(s[i],"simple")) if (playerIndex==-1) playerIndex=0; else return false;
    else if (!stricmp(s[i],"counting")) if (playerIndex==-1) playerIndex=1; else return false;
    else if (!stricmp(s[i],"competitor")) if (playerIndex==-1) playerIndex=2; else return false;
    else if (strchr("-\\/",s[i][0]) && strchr("bB",s[i][1])
      && s[i][2]==0) if (!userInterface) userInterface=true; else return false;
    else return false;
  if (userInterface) playerIndex=3; if (playerIndex==-1) return false; 
  if (u!=2) return false; bankroll=a[0]; hand=a[1];
  return true;
}
int main(int paramnum, char* paramstr[]) {
  int bankroll, hand, playerIndex;
  if (!params(paramnum,paramstr,bankroll,hand,playerIndex))
    printUseage();
  else driver.run(bankroll,hand,playerIndex);
  return 0;
}

