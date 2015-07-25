#include "token.h"
#include "dbase\dbf.h"
#include <iostream>
using namespace std;


//#define DEBUG_RUNLINE

tptk tokens;

static void* getstr(ifstream &f) {
  static char tokename[4]="~0$";
  if (++tokename[1]>'9') tokename[1]='0';
  int sizes=256; int len=0;
  char *s=(char*) malloc (sizes*sizeof(char)), *s_, *d;
  char c=f.get(), c1, c2;
  while ((c=f.get())!='\"' && !f.eof()) {
    if (len==sizes) {
      // 自扩增
      s_=(char*) malloc ((sizes<<=1)*sizeof(char));
      memcpy(s_,s,len*sizeof(char));
      free(s); s=s_;
    }
    if (c=='\\') {
      c1=f.get(); c2=f.get();
      c1=(c1>='a')?(c1-'a'+10):((c1>='A')?(c1-'A'+10):(c1-'0'));
      c2=(c2>='a')?(c2-'a'+10):((c2>='A')?(c2-'A'+10):(c2-'0'));
      c=c1*16+c2;
    }
    s[len++]=c;
  }
  d=(char*) malloc ((len+1)*sizeof(char));
  memcpy(d,s,(len*sizeof(char)));
  free(s);
  d[len]=0;
  return tokens[tokename]=(void*)d;
}

static void* getdec(ifstream &f) {
  static char tokename[4]="~0#";
  char c=f.get(); long *n=(long*)malloc(sizeof(long));
  if (++tokename[1]>'9') tokename[1]='0';
  for ((*n)=0;(c=f.get())!='#';(*n)+=c-'0') *n*=10;
  return tokens[tokename]=(void*)n;
}
static void* gethex(ifstream &f) {
  static char tokename[4]="~A#";
  char c=f.get(); long *n=(long*)malloc(sizeof(long));
  if (++tokename[1]>'J') tokename[1]='0';
  for (*n=0;(c=f.get())!='#';*n+=c)
    c=(c>='a')?(c-'a'+10):((c>='A')?(c-'A'+10):(c-'0')), *n<<=4;
  return tokens["##"]=(void*)n;
}

static void* getvar(ifstream &f) {
  string s; char c;
  while (1) {
    while ((c=f.peek())!='[' && c!=' ' && c!='\t' && c!='\15' && c!='\12' && c!=']')
      s+=f.get();
    if (c=='[') {
      s+=f.get();
      long* n=(long*)gettoken(f), m=*n;
      while (m) { s+=char(m%10+'0'); m/=10; }
      s+=f.get();
    } else break;
  }
  #ifdef DEBUG_RUNLINE
  cout<<"gettoken="<<s<<endl;
  system("pause");
  #endif
  if (!tokens[s])
   switch (s[s.length()-1]) {
    case '#':tokens[s]=malloc(sizeof(long)); break;
    case '@':tokens[s]=malloc(sizeof(DBF)); break;
    case '$':tokens[s]=malloc(0x100*sizeof(char)); break;
    case '!':tokens[s]=malloc(sizeof(char)); break;
    case ':':{
      cerr<<"未定义的行标号：\""<<s<<"\"，程序终止\n";
      system("pause");
      exit(-1);
    }
    default :{
      cerr<<"未知类型的变量：\""<<s<<"\"，程序终止\n";
      system("pause");
      exit(-1);
    }
  }
  return tokens[s];
}

void* gettoken(ifstream &f) {
  char c;
  while ((c=f.peek())==' ' || c=='\t' ||
          c=='\15' || c=='\12') f.get();
  #ifdef DEBUG_RUNLINE
  cout<<"gettoken=s="<<c<<endl;
  #endif
  switch (c) {
    case '\"': return getstr(f);
    case '#' : return getdec(f);
    case '$' : return gethex(f);
    default  : return getvar(f);
  }
}

