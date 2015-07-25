#include "funclist.h"
#include <fstream>
#include <iostream>
#include "math.h"
#include "token.h"
#include "crts.h"
#include "md5\md5.h"
#include "run.h"
#include "dbase\dbf.h"

using namespace std;

static void inputnum(long &n, long min, long max) {
  int px=wherex() ,py=wherey(), l;
  char c;
  for (l=n>=0?printf("%d",n):0;l<int(log10((double)max)+1);l++) printf(" ");
  do
    while ((c=readkey())>='0' && c<='9' || c=='\10') {
      if (c=='\10') if (n==-1) return;else if (n<10) n=-1; else n/=10;
      else if (n==-1) n=c-'0'; else if ((n=n*10+c-'0')>max) n/=10;
      gotoxy(px,py);
      for (l=n>=0?printf("%d",n):0;l<int(log10((double)max)+1);l++) printf(" ");
  } while (!(n>=min && n<=max));
}
void f_inputd(ifstream& f) {
  f_cleark(f);
  char* s=(char*)gettoken(f);
  int px=wherex() ,py=wherey();
  long y=-1,m=-1,d=-1;
  int maxd[12]={31,28,31,30,31,30,31,31,30,31,30,31};
  cout<<" 四位 年    月    日";
  label1: gotoxy(px+1,py);  inputnum(y,1800,2010);
  if (y<0) goto label1;
  gotoxy(10,10);
  label2: gotoxy(px+9,py);  inputnum(m,1,12);
  if (m<0) goto label1;
  gotoxy(px+15,py); inputnum(d,1,maxd[m-1]+
           (m==2 && (!(y%400) || (y%100) && !(y&3))));
  if (d<0) goto label2;
  gotoxy(px+20,py);
  s[0]=y/1000+'0'; s[1]=y/100%10+'0';
  s[2]=y/10%10+'0'; s[3]=y%10+'0';
  s[4]=m/10+'0'; s[5]=m%10+'0';
  s[6]=d/10+'0'; s[7]=d%10+'0';
  s[8]=0;
}
void f_inputr(ifstream& f) {
  long *p=(long*)gettoken(f);
  int px=wherex() ,py=wherey();
  cout<<"         元 整";
  int l,e=0; char c; long n=0,nn=0;
 do {
  gotoxy(px+1,py); if (n>=0) printf("%7d",n); else printf("%7c",32);
  while ((c=readkey())>='0' && c<='9' || c=='\10') {
    if (c=='\10') if (n==-1); else if (n<10) n=-1; else n/=10;
    else if (n==-1) n=c-'0'; else if ((n=n*10+c-'0')>10000000) n/=10;
    gotoxy(px+1,py); if (n>=0) printf("%7d",n); else printf("%7c",32);
  }
  gotoxy(px+14,py);
  if (c=='.') while ((c=readkey())>='0' && c<='9' || c=='\10') {
    if (c=='\10') if (!e) break; else nn/=10, e--;
    else if (e!=2) nn=nn*10+c-'0', e++; else nn=nn%10*10+c-'0';
    gotoxy(px+12,py);
    if (!e) cout<<"整"; else if (e-1) cout<<nn/10<<nn%10; else cout<<nn<<" ";
  }
 } while (c=='\10');
  gotoxy(px+15,py);
  *p=n*100+((!e)?0:((e-1)?nn:nn*10));
}
void f_inputn(ifstream& f) {
  long *n=(long*)gettoken(f);
  do inputnum(*n,0,1<<31-1); while (*n<0);
};
void f_printn(ifstream& f) {
  long *n=(long*)gettoken(f);
  cout<<*n;
}
void f_inputs(ifstream& f) {
  f_cleark(f);
  char* s=(char*)gettoken(f);
  long* len=(long*)gettoken(f);
  char* c=(char*)gettoken(f);
  bool z[256]; memset(z,0,sizeof(z));
  int n=strlen(s); int i;
  for (i=0;s[i];i++) if (s[i]<0) z[i++]=true;
  cout<<s;
  while (keypressed()) readkey();
  while ((*c=readkey())>=32 || *c=='\10' || *c<0 && (*c!=-32 || n && z[n-1])) {
   if (*c!='\10')
    if (*c<0 && (n==0 || !z[n-1]) && n+1>*len);
    else {if (n<*len) {
      z[n]=(*c<0)&&(n==0 || !z[n-1]);
      s[n++]=*c; putchar(*c);
    }}
   else if (n)
    if (n>1 && z[n-2]) {
      putchar(*c); putchar(' '); putchar(*c);
      putchar(*c); putchar(' '); putchar(*c);
      n-=2;
    } else {
      putchar(*c); putchar(' '); putchar(*c);
      n--;
    }
  }
  s[n]=0;
}
void f_prints(ifstream& f) {
  char *s=(char*)gettoken(f);
  cout<<s;
}
void f_inputp(ifstream& f) {
  char *s=(char*)gettoken(f);
  unsigned char d[16];
  int n=0; char c;
  while (keypressed()) readkey();
  while (n<8)
   while ((c=readkey())!='\n' && c!='\15') {
    if (c>=32) {
      if (n==32) continue;
      cout<<"*"; s[n++]=c;
    } else if (c==8 && n>0) {
      cout<<"\10 \10"; n--;
    } else if (c<0) readkey();
  }
  s[n]=0;
  MDString((unsigned char*)(void*)s,d);
//  cout<<endl<<s<<endl;
//  MD5Print(d);
//  cout<<endl;
//  system("pause");
  memcpy(s,d,16);
  s[16]=0;
}
void f_getdate(ifstream& f) {
  char *s=(char*)gettoken(f);
  time_t t=time(NULL);
  struct tm mt;
  memcpy(&mt,localtime(&t),sizeof(struct tm));
  mt.tm_year+=1900; mt.tm_mon+=1;
  int &y=mt.tm_year, &m=mt.tm_mon, d=mt.tm_mday;
  s[0]=y/1000+'0'; s[1]=y/100%10+'0';
  s[2]=y/10%10+'0'; s[3]=y%10+'0';
  s[4]=m/10+'0'; s[5]=m%10+'0';
  s[6]=d/10+'0'; s[7]=d%10+'0';
  s[8]=0;
}
void f_printr(ifstream& f) {
  long *n=(long*)gettoken(f);
  if (*n==0) cout<<"免费  ！"; else {
  cout<<*n/100<<" 元 ";
  if (*n%100==0) cout<<"整";
  else cout<<*n%100;}
}
void f_printd(ifstream& f) {
  char* s=(char*)gettoken(f);
  cout<<s[0]<<s[1]<<s[2]<<s[3]<<" 年 "
      <<s[4]<<s[5]<<" 月 "
      <<s[6]<<s[7]<<" 日";
}
void f_putc(ifstream& f) {
  char *s=(char*)gettoken(f);
  cout<<*s;
}
void f_gotoxy(ifstream& f) {
  long *x=(long*)gettoken(f);
  long *y=(long*)gettoken(f);
  gotoxy(*x,*y);
}
void f_wherexy(ifstream& f) {
  long *x=(long*)gettoken(f);
  long *y=(long*)gettoken(f);
  *x=wherex();
  *y=wherey();
}
void f_pause(ifstream& f) {
  readkey();
}
void f_getc(ifstream& f) {
  char *c=(char*)gettoken(f);
  *c=readkey();
}
void f_upcase(ifstream& f) {
  char *c=(char*)gettoken(f);
  if (*c>='a' && *c<='z') *c-='a'-'A';
}
void f_cleark(ifstream& f) {
  while (keypressed()) readkey();
}

void f_letn(ifstream& f) {
  long *n1=(long*)gettoken(f);
  long *n2=(long*)gettoken(f);
  *n1=*n2;
}
void f_lets(ifstream& f) {
  char *s1=(char*)gettoken(f);
  char *s2=(char*)gettoken(f);
  strcpy(s1,s2);
}
void f_letc(ifstream& f) {
  char *c1=(char*)gettoken(f);
  char *c2=(char*)gettoken(f);
  *c1=*c2;
}

void f_add(ifstream& f) {
  long *n1=(long*)gettoken(f);
  long *n2=(long*)gettoken(f);
  long *n3=(long*)gettoken(f);
  (*n1)=(*n2)+(*n3);
}
void f_sub(ifstream& f) {
  long *n1=(long*)gettoken(f);
  long *n2=(long*)gettoken(f);
  long *n3=(long*)gettoken(f);
  (*n1)=(*n2)-(*n3);
}
void f_mul(ifstream& f) {
  long *n1=(long*)gettoken(f);
  long *n2=(long*)gettoken(f);
  long *n3=(long*)gettoken(f);
  (*n1)=(*n2)*(*n3);
}
void f_divl(ifstream& f) {
  long *n1=(long*)gettoken(f);
  long *n2=(long*)gettoken(f);
  long *n3=(long*)gettoken(f);
  (*n1)=(*n2)/(*n3);
}
void f_divh(ifstream& f) {
  long *n1=(long*)gettoken(f);
  long *n2=(long*)gettoken(f);
  long *n3=(long*)gettoken(f);
  (*n1)=(*n2)%(*n3);
}
void f_call(ifstream& f) {
  char *path=(char*)gettoken(f);
  run(path);
}
void f_gotof(ifstream& f) {
  char *path=(char*)gettoken(f);
  f.close(); f.clear();
  f.open(path); getlabel(f); f.close(); f.clear();
  f.open(path);
}
void f_exit(ifstream& f) {
  f.setstate(ios::badbit);
}
void f_halt(ifstream& f) {
  exit(0);
}
void f_clear(ifstream& f) {
  tptk::iterator i;
  for (i=tokens.begin();i!=tokens.end();i++)
  { free(i->second); i->second=NULL; }
  tokens.clear();
}
void f_ston(ifstream& f) {
  char *s=(char*)gettoken(f);
  long *n=(long*)gettoken(f);
  int i, t=0; *n=0;
  for (i=strlen(s)-1;i>=0 && s[i]>='0' && s[i]<='9';i--) 
    t?t*=10:t=1, *n+=t*(s[i]-'0');
  if (i>=0 && s[i]=='-') *n=-*n;
}
void f_ntos(ifstream& f) {
  long *n=(long*)gettoken(f);
  char *s=(char*)gettoken(f);
  long *len=(long*)gettoken(f);
  int i=*len; long num; s[*len]=0;
  for (num=abs(*n);num>0;num/=10) s[--i]=num%10+'0';
  if (*n<0) s[--i]='-';
  for (--i;i>=0;--i) s[i]=' ';
  if (*n==0) s[*len-1]='0';
}
void f_ctos(ifstream& f) {
  char *c=(char*)gettoken(f);
  char *s=(char*)gettoken(f);
  *s=*c; *(s+1)=0;
}
void f_cton(ifstream& f) {
  char *c=(char*)gettoken(f);
  long *n=(long*)gettoken(f);
  *n=long(*c);
}
void f_ntoc(ifstream& f) {
  long *n=(long*)gettoken(f);
  char *c=(char*)gettoken(f);
  *c=char(*n);
}
void f_stoc(ifstream& f) {
  char *s=(char*)gettoken(f);
  char *c=(char*)gettoken(f);
  long *i=(long*)gettoken(f);
  *c=s[*i];
}
void f_lens(ifstream& f) {
  char *s=(char*)gettoken(f);
  long *l=(long*)gettoken(f);
  *l=strlen(s);
}
void f_goto(ifstream& f) {
  streampos *l=(streampos*)gettoken(f);
  f.seekg(*l);
}
void f_ifs(ifstream& f) {
  char *s1=(char*)gettoken(f);
  char *s2=(char*)gettoken(f);
  if (!strcmp(s1,s2)) f_goto(f);
  else gettoken(f);
}
void f_ifn(ifstream& f) {
  long *n1=(long*)gettoken(f);
  long *n2=(long*)gettoken(f);
  if (*n1==*n2) f_goto(f);
  else gettoken(f);
}
void f_ifc(ifstream& f) {
  char *c1=(char*)gettoken(f);
  char *c2=(char*)gettoken(f);
  if (*c1==*c2) f_goto(f);
  else gettoken(f);
}
void f_ifp(ifstream& f) {
  long *n=(long*)gettoken(f);
  if ((*n)>0) f_goto(f);
  else gettoken(f);
}
void f_ifpw(ifstream& f) {
  char *s1=(char*)gettoken(f);
  char *s2=(char*)gettoken(f);
  int i; for (i=0;i<16;i++)
   if (s1[i]!=s2[i]) break;
  if (i==16) f_goto(f);
  else gettoken(f);
}

void f_load(ifstream& f) {
  DBF* db=(DBF*)gettoken(f);
  char* p=(char*)gettoken(f);
  char* t=(char*)gettoken(f);
  string s=t; int i; long *n;
  strcpy(db->filename,p);
  d_open(db);
  for (i=0;i<(db->header_len-0x21)/0x20;i++) {
    n=(long*)malloc(sizeof(long)); *n=i+1;
    tokens[s+"@"+db->fields[i].name+"$INDEX#"]=n;
    n=(long*)malloc(sizeof(long)); *n=db->fields[i].length;
    tokens[s+"@"+db->fields[i].name+"$LEN#"]=n;
  }
  tokens[s+"@RECORDS#"]=&db->records;
}
void f_seti(ifstream& f) {
  DBF* db=(DBF*)gettoken(f);
  char* p=(char*)gettoken(f);
  long* i=(long*)gettoken(f);
  d_addndx(db,*i,p);
}

void f_getr(ifstream& f) {
  DBF* db=(DBF*)gettoken(f);
  long* i=(long*)gettoken(f);
  char* r=(char*)gettoken(f);
  d_getrec(db,*i,r);
}
void f_putr(ifstream& f) {
  DBF* db=(DBF*)gettoken(f);
  long* i=(long*)gettoken(f);
  char* r=(char*)gettoken(f);

  if (*i) d_putrec(db,*i,r);
  else    d_addrec(db,r);
}
void f_ftor(ifstream& f) {
  DBF* db=(DBF*)gettoken(f);
  char* r=(char*)gettoken(f);
  long* s=(long*)gettoken(f);
  char* v=(char*)gettoken(f);
  d_putfld(db,*s,v,r);
}
void f_rtof(ifstream& f) {
  DBF* db=(DBF*)gettoken(f);
  char* r=(char*)gettoken(f);
  long* s=(long*)gettoken(f);
  char* v=(char*)gettoken(f);
  d_getfld(db,*s,v,r);
}
void f_find(ifstream& f) {
  DBF* db=(DBF*)gettoken(f);
  char* v=(char*)gettoken(f);
  long* i=(long*)gettoken(f);
  long* s=(long*)gettoken(f);
  *s=d_search(db,*i,*s,v);
}
void f_del(ifstream& f) {
  DBF* db=(DBF*)gettoken(f);
  long* i=(long*)gettoken(f);
  d_delrec(db,*i);
}
void f_rec(ifstream& f) {
  DBF* db=(DBF*)gettoken(f);
  long* i=(long*)gettoken(f);
  d_rclrec(db,*i);
}
void f_close(ifstream &f) {
  DBF* db=(DBF*)gettoken(f);
  d_close(db);
}
void f_command(ifstream& f) {
  char *s=(char*)gettoken(f);
  system(s);
}
void f_delst(ifstream& f) {
  char *s=(char*)gettoken(f);
  int i=strlen(s)-1;
  while (i>=0 && s[i]==' ') s[i--]=0;
}
