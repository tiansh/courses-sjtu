#include "BigInt.h"
#include <iostream>
#include <string>
using namespace std;


static inline BigInt tens(long n) {
  static BigInt ten[1027]={1,10};
  static bool tena[1027]={true};
  if (!tena[n]) {ten[n]=(ten[n-1]<<3)+(ten[n-1]<<1); tena[n]=true; }
  return ten[n];
}

BigInt::BigInt() {
  memset(d,false,sizeof(d));
}
BigInt::BigInt(signed long long b) {
  memset(d,b<0,sizeof(d));
  for (int i=1;b;i++) {
    d[len-i]=b&1;
    b>>=1;
  }
}
BigInt::BigInt(string s) {
  memset(d,0,sizeof(d));
  unsigned int i; bool f=s[0]=='-';
  if (strchr("+-",s[0])) s.erase(0,1);
  if (f) for (i=0;i<s.length();i++) *this=((*this)<<3)+((*this)<<1)-(s[i]-48);
  else for (i=0;i<s.length();i++) *this=((*this)<<3)+((*this)<<1)+(s[i]-48);
}

BigInt& BigInt::operator=(const BigInt b){
  for (int i=0;i<len;i++) d[i]=b.d[i];
  return *this;
}

BigInt BigInt::operator-() const {
  int i; BigInt c;
  for (i=len-1;i>=0 && !d[i];i--) c.d[i]=false;
  c.d[i]=true;
  for (i=i-1;i>=0;i--) c.d[i]=!d[i]; 
  return c;
}

BigInt& BigInt::operator++() {
  int i;
  for (i=len-1;i>0 && d[i];i--) d[i]=false;
  d[i]=!d[i];
  return *this;
}
BigInt& BigInt::operator--() {
  int i;
  for (i=len-1;i>0 && !d[i];i--) d[i]=true;
  d[i]=!d[i];
  return *this;
}
BigInt BigInt::operator++(int) {
  BigInt t(*this); int i;
  for (i=len-1;i>0 && d[i];i--) d[i]=false;
  d[i]=!d[i];
  return t;
}
BigInt BigInt::operator--(int) {
  BigInt t(*this); int i;
  for (i=len-1;i>0 && !d[i];i--) d[i]=true;
  d[i]=!d[i];
  return t;
}

BigInt BigInt::operator~() const {
  BigInt c;
  for (int i=0;i<len;i++) c.d[i]=!d[i];
  return c;
}
BigInt BigInt::operator&(const BigInt b) const {
  BigInt c;
  for (int i=0;i<len;i++) c.d[i]=d[i]&&b.d[i];
  return c;
}
BigInt BigInt::operator|(const BigInt b) const {
  BigInt c;
  for (int i=0;i<len;i++) c.d[i]=d[i]||b.d[i];
  return c;
}
BigInt BigInt::operator^(const BigInt b) const {
  BigInt c;
  for (int i=0;i<len;i++) c.d[i]=d[i]!=b.d[i];
  return c;
}

ostream& operator<<(ostream & os, const  BigInt a) {
  BigInt b;
  if (a.d[0]) {os.put('-'); b=-a;} 
  else {if (os.flags()&ostream::showpos) os.put('+'); b=a;}
  if (os.flags()&ostream::dec) {
    BigInt c(1); int n=0,m;
    while (tens(n)<=b) n++;
    if (n==0) os.put('0'); else 
    for (n--;n>=0;n--) {
      for (m=0;tens(n)<=b;m++) b-=tens(n);
      os.put('0'+m);
    }
  } else if (os.flags()&ostream::oct) {
    if (os.flags()&ostream::showbase) os.put('0');
    int i; 
    for (i=0;i<b.len;i++) 
      if (b.d[i]) break;
    if (i%3!=0) i=(i/3)*3;
    for (;i<b.len;i+=3) os.put('0'+4*(b.d[i])+2*(b.d[i+1])+(b.d[i+2]));
  } else if (os.flags()&ostream::hex) {
    int i; char s[]="0123456789ABCDEF";
    if (!os.uppercase) 
      for (i=10;s[i];i++) s[i]=tolower(s[i]);
    if (os.flags()&ostream::showbase) {
      os.put('0');
      if (os.uppercase) os.put('X'); else os.put('x');
    }
    for (i=0;i<b.len;i++) if (b.d[i]) break;
    if (i%4!=0) i=(i/4)*4;
    for (;i<b.len;i+=4) os.put(s[8*(b.d[i])+4*(b.d[i+1])+2*(b.d[i+2])+(b.d[i+3])]);
  };
  return os;
}
istream & operator >>(istream & is, BigInt &a) {
  char c; string s=""; BigInt n;
  while (c=is.peek(),c==32 || c==7 || c==10 || c==13) is.get(c);
  if (is.peek()=='-') { is.get(c); s=c;} else s="+";
  while (c=is.peek(),48<=c && c<=57) is.get(c), s+=c;
  if (s.length()==1) cin.setstate(ios_base::failbit);
  a=s;
  return is;
}

BigInt BigInt::operator+(const BigInt b) const {
  int i; BigInt c; bool t=false;
  for (i=len-1;i>=0;i--) {
    c.d[i]=d[i]^b.d[i]^t;
    t=t&&d[i]||t&&b.d[i]||d[i]&&b.d[i];
  }
  return c;
}
BigInt BigInt::operator-(const BigInt b) const {
  int i; BigInt c; bool t=false;
  for (i=len-1;i>=0;i--) {
    c.d[i]=d[i]!=b.d[i]!=t;
    t=t&&b.d[i]||t&&!d[i]||b.d[i]&&!d[i];
  }
  return c;
}
BigInt BigInt::operator*(const BigInt b) const {
  BigInt c=*this, d;
  for (int i=len-1;i>=0;i--,c=c<<1) {
    if (b.d[i]) d+=c;
  }
  return d;
}
BigInt BigInt::operator/(const BigInt b) const {
  if (b==0) exit(-1);
  BigInt c=b,e=*this,f; int n=0;

  bool l=(c.d[0]!=e.d[0]);
  if (c.d[0]) c=-c; if (e.d[0]) e=-e;
 
  while (c<=e) c<<=1,n++;
  for (c>>=1;n>=1;n--,c>>=1) f.d[len-n]=(e>=c)?(e-=c,true):false; 
  if (l) f=-f;
  return f;
}
BigInt BigInt::operator%(const BigInt b) const {
  if (b==0) exit(-1);
  BigInt c=b,e=*this; int n=0;

  bool l=(e.d[0]);
  if (c.d[0]) c=-c; if (e.d[0]) e=-e;

  while (c<=e) c<<=1,n++;
  for (c>>=1;n>=1;n--,c>>=1) if (e>=c) e-=c;
  if (l) e=-e;
  return e;
}

BigInt BigInt::operator<<(const long b) const {
  BigInt c=*this; int i;
  for (i=b;i<=len;i++) c.d[i-b]=c.d[i];
  for (i=len-b;i<len;i++) c.d[i]=false;
  return c;
}
BigInt BigInt::operator>>(const long b) const {
  BigInt c=*this; int i;
  for (i=len-b-1;i>=0;i--) c.d[i+b]=c.d[i];
  for (i=0;i<=b-1;i++) c.d[i]=false;
  return c;
}

int BigInt::compare(const BigInt b) const {
  if (d[0]!=b.d[0]) return d[0]?-1:1;
  int i; for (i=1;i<len && b.d[i]==d[i];i++) continue;
  if (i==len) return 0; else return d[i]?1:-1;
}
