#ifndef HAND_H
#define HAND_H
#include "card.h"
struct HandValue {
  //���Ƶ�ֵ
  int count; 

  //��Ϊ����ʱΪ��(true)
  bool soft; 
};

class Hand {
    //������һ��Blackjack�����ư������Ż�����˿ˡ�
    //ע�⣺������ʹ�õ�Ψһ��һ��˽�е�(private)������

    HandValue curValue;

  public:

   //Ӱ�죺����һ���յ����ơ�
    Hand();
 
    //��������
    //�޸ģ��Լ�
    //Ӱ�죺������ǰ���е������ƣ���������״̬Ϊ�ա�
    void discardAll();

    //�����
    //�޸ģ��Լ�
    //Ӱ�죺���˿���c���뵽��ǰ������
    void addCard(Card c);

    //����ֵ
    //Ӱ�죺���ص�ǰ���ƵĴ�С�����ֵ�ǲ�����21�������ܵ�ֵ����
    //����һ��������11��A����ô����(soft)����Ӧ��Ϊ�档
    //ע�⣺����(const)������ζ���㲻���ڱ��������޸��κγ�Ա����
    //��ֵ��������Ϊ����õ������ǹ̶�����������Ҹ��ĵġ�
    HandValue handValue() const;

};
#endif
