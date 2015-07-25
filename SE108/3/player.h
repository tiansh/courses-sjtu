#ifndef PLAYER_H
#define PLAYER_H

#include "card.h"
#include "hand.h"

class Player {
    //һ������Ļ��࣬�ṩ�����Ľӿ�
    
  public:

    //Ҫ���ʽ�(bankroll)����С��ע��(mininum)
    //Ӱ�죺������ҵ���ע���Ҫ����������֮�䣨������
    virtual int bet(unsigned int bankroll,
                    unsigned int minimum) = 0;

    //��������Ҫ�����򷵻��棬���򷵻ؼ١�
    virtual bool draw(Card dealer, //���泯�ϵ���
                      const Hand &player) = 0; //��ǰ��ҵ�һ��

    //��¶
    //Ӱ�죺�����û��������ոձ�¶��������c�����磬ÿ�����淢����
    //���ƶ���Ҫ����¶(expose()d)�ġ����⣬���ׯ����Ҫ��������
    //(hole card)����ô��ҲҪ����¶(expose()d)�ġ�ע�⣺��������
    //�е��ƶ��Ǳ�¶�ģ������ұ��ƻ���natural 21�������߲���
    //Ҫ������¶���ơ�
    virtual void expose(Card c) = 0;

    //ϴ��
    //Ӱ�죺��֪����ƶ��Ѿ�����ϴ���ˡ�
    virtual void shuffled() = 0;

    //ע�⣺���������Ψһ��;���ñ��������������棬���������ҵ��
    //���������ǲ���Ҫ�ġ�
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
