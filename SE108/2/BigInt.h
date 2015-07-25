#ifndef BIGINT_H
#define BIGINT_H
#include <iostream>
using namespace std;

class BigInt {
  public:
    string t;
    BigInt();
    BigInt(string s);
    BigInt(signed long long b);
    BigInt operator-()const;
    BigInt& operator++();
    BigInt& operator--();
    BigInt operator++(int);
    BigInt operator--(int);
    BigInt operator~() const;
    BigInt operator&(const BigInt b) const;
    BigInt operator|(const BigInt b) const;
    BigInt operator^(const BigInt b) const;
    BigInt operator+(const BigInt b) const;
    BigInt operator-(const BigInt b) const;
    BigInt operator*(const BigInt b) const;
    BigInt operator/(const BigInt b) const;
    BigInt operator%(const BigInt b) const;
    BigInt operator<<(const long b) const;
    BigInt operator>>(const long b) const;
    BigInt& operator=(const BigInt b);
    BigInt& operator+=(const BigInt b) {return *this=*this+b;}
    BigInt& operator-=(const BigInt b) {return *this=*this-b;}
    BigInt& operator*=(const BigInt b) {return *this=*this*b;}
    BigInt& operator/=(const BigInt b) {return *this=*this/b;}
    BigInt& operator%=(const BigInt b) {return *this=*this%b;}
    BigInt& operator<<=(const int b) {return *this=*this<<b;}
    BigInt& operator>>=(const int b) {return *this=*this>>b;}
    BigInt& operator&=(const BigInt b) {return *this=*this&b;}
    BigInt& operator|=(const BigInt b) {return *this=*this|b;}
    BigInt& operator^=(const BigInt b) {return *this=*this^b;}
    friend bool operator>(const BigInt a, const BigInt b) {return a.compare(b)>0;}
    friend bool operator<(const BigInt a, const BigInt b) {return a.compare(b)<0;}
    friend bool operator==(const BigInt a, const BigInt b) {return a.compare(b)==0;}
    friend bool operator>=(const BigInt a, const BigInt b) {return a.compare(b)>=0;}
    friend bool operator<=(const BigInt a, const BigInt b) {return a.compare(b)<=0;}
    friend bool operator!=(const BigInt a, const BigInt b) {return a.compare(b)!=0;}
    friend ostream& operator<<(ostream & os, const BigInt a);
    friend istream& operator>>(istream & is, BigInt &a);
  private:
    static const int len=3408;
    bool d[len];
    int compare(const BigInt b) const;
};
#endif
