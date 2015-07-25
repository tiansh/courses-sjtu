#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "hand.h"

class Player {
    //一个抽象的基类，提供玩家类的接口
    
  public:

    //要求资金(bankroll)≥最小下注额(mininum)
    //影响：返回玩家的下注额，它要介于这两者之间（含）。
    virtual int bet(unsigned int bankroll,
                    unsigned int minimum) = 0;

    //如果玩家想要加牌则返回真，否则返回假。
    virtual bool draw(Card dealer, //发面朝上的牌
                      const Hand &player) = 0; //当前玩家的一手

    //暴露
    //影响：允许用户“看”刚刚被露出来的牌c。例如，每个正面发下来
    //的牌都是要被暴露(expose()d)的。另外，如果庄家需要亮出暗牌
    //(hole card)，那么他也要被暴露(expose()d)的。注意：并不是所
    //有的牌都是暴露的，如果玩家爆牌或是natural 21，则发牌者不需
    //要向他暴露暗牌。
    virtual void expose(Card c) = 0;

    //洗牌
    //影响：告知玩家牌堆已经重新洗牌了。
    virtual void shuffled() = 0;

    //注意：这个函数的唯一用途是让编译器不发出警告，在这个大作业中
    //析构函数是不需要的。
    virtual ~Player() { }

};

class Simple: public Player{
  public:
    int bet(unsigned int bankroll, unsigned int minimum);
    bool draw(Card dealer, const Hand &player);
    void expose(Card c);
    void shuffled();
};
static Simple simplePlayer;
extern Player *get_Simple();

class Counting: public Simple{
    int count;
  public:
    Counting();
    int bet(unsigned int bankroll, unsigned int minimum);
    void expose(Card c);
    void shuffled();
};   
static Counting countingPlayer;
extern Player *get_Counting();

class Competitor: public Player{
    int count;
    int rest[ACE+1];
    double pj(Card dealer, const Hand &player);
    double pg(Card dealer, const Hand &player);
    double pg(Hand dealer, const Hand &player);
  public:
    Competitor();
    int bet(unsigned int bankroll, unsigned int minimum);
    bool draw(Card dealer, const Hand &player);
    void expose(Card c);
    void shuffled();
}; 
static Competitor competitorPlayer;
extern Player *get_Competitor();


class Interactive: public Player{
  public:
    int bet(unsigned int bankroll, unsigned int minimum);
    bool draw(Card dealer, const Hand &player);
    void expose(Card c);
    void shuffled();
};
static Interactive interactivePlayer;
extern Player *get_Interactive();

extern Player *get_Player(int n);

#endif
