#ifndef HAND_H
#define HAND_H
#include "card.h"
struct HandValue {
  //手牌的值
  int count; 

  //当为软牌时为真(true)
  bool soft; 
};

class Hand {
    //概述：一个Blackjack的手牌包括零张或多张扑克。
    //注意：这是你使用的唯一的一个私有的(private)陈述。

    HandValue curValue;

  public:

   //影响：建立一个空的手牌。
    Hand();
 
    //丢弃所有
    //修改：自己
    //影响：丢弃当前持有的所有牌，重置手牌状态为空。
    void discardAll();

    //添加牌
    //修改：自己
    //影响：将扑克牌c加入到当前的牌中
    void addCard(Card c);

    //手牌值
    //影响：返回当前手牌的大小。这个值是不超过21的最大可能的值。如
    //果有一个被当作11的A，那么软牌(soft)变量应当为真。
    //注意：常量(const)修饰意味着你不能在本函数中修改任何成员变量
    //的值。这是因为玩家拿到的牌是固定而不容许玩家更改的。
    HandValue handValue() const;

};
#endif
