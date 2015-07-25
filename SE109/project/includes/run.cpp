#include "run.h"
#include "token.h"
#include <iostream>
#include <fstream>

//#define DEBUG_RUNLINE
static void runline(ifstream &f) {
  char c, funcn[12]; int i;
  while ((c=f.peek())==' ' || c=='\t' ||
         c=='\15' || c=='\12') f.get();
  if (f.eof()) return;
  if (c==':' || c=='\'' || c=='%' || c==';')
  { f.ignore(1024,'\n'); return; }
  f>>funcn;
#ifdef DEBUG_RUNLINE
  cout<<endl<<funcn<<endl;
  system("pause");
#endif
  for (i=0;i<funcnum && strcmp(funcn,funclist[i].n);i++);
  if (i==funcnum) {
    cerr<<"Î´Ô¤ÆÚµÄÃüÁî£º\""<<funcn
        <<"\"£¬³ÌÐòÖÕÖ¹\n";
    system("pause"); exit(-1);
  }
  (*funclist[i].f)(f);
}

void getlabel(ifstream& f) {
  char line[1024]; int i=0;
  while (f) {
    f.getline(line,1023,'\n');
    i+=strlen(line)+2;
    if (line[0]==':') {
      streampos *p=(streampos*)(tokens[line+1]
        =malloc(sizeof(streampos)));
      *p=i;
    }
  }
}

void run(char* path) {
  ifstream *f=new ifstream;
  (*f).open(path); getlabel((*f)); (*f).close(); (*f).clear();
  (*f).open(path); while ((*f)) runline((*f)); (*f).close();
  delete f;
}
