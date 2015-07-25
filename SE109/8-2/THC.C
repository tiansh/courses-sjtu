// this program is compiled under Turbo C++ 3.0

#define VERSION 1

#include <stdio.h>
#include <memory.h>
#include <malloc.h>

// define bool type
#ifndef bool
#define bool char
#define false ((bool)0)
#define true  ((bool)1)
#endif

// swap 2 variables
#define swap(a,b) ((a)^=(b)^=(a)^=(b))

// "simple" stack
#define stack_init()    \
  (top=0)
#define push(data,size) \
  memcpy(stack[top++]=(void*)malloc(size),&(data),(size))
#define pop(data,size)  \
  {memcpy(&(data),stack[top-1],(size)); free(stack[--top]);}
#define is_empty()      \
  (!top)

#define print(fo,buffer) \
  fwrite((char*)&buffer+3,1,1,fo); \
  fwrite((char*)&buffer+2,1,1,fo); \
  fwrite((char*)&buffer+1,1,1,fo); \
  fwrite((char*)&buffer+0,1,1,fo); \
  file_len+=4


struct Node{
  unsigned char c;
  unsigned long t;
  struct Node *l, *r, *n;
};
typedef struct Node Node;

int main(int argc, char* argv[]) {
  // variables
  void *stack[0x200]; int top;
  FILE *fi, *fo;
  int i, j, u;
  Node *p, *q, *h;
  unsigned long n[0x100];
  unsigned char hs[0x100];
  unsigned char l01[0x100];
  unsigned long  c01[0x100];
  unsigned char tree_header[0x200];
  unsigned long tree_header_len;
  unsigned long header_len;
  unsigned long file_len;
  unsigned long binl;
  unsigned long buffer;
  char th[3]="TH";
  unsigned char ic;
  unsigned char ver;
  unsigned char buffer_len;
  unsigned char binl_len;
  unsigned char spacel;
  unsigned char last_len;

  // init variables
  stack_init();
  if (argc<3) return -1;
  for (i=0;i<0x100;i++)
    n[i]=0, hs[i]=i;
  memset(c01,0,sizeof(c01));
  memset(l01,0,sizeof(l01));
  ver=VERSION;

  // input & count characters
  fi=fopen(argv[1],"rb");
  while (1) {
    fread(&ic,1,1,fi);
    if (!feof(fi)) n[ic]++;
    else break;
  }
  fclose(fi);

  // sort characters with times
  for (i=0;i<0x100-1;i++)
   for (j=i+1;j<0x100;j++)
    if (n[i]<n[j])
     swap(n[i],n[j]), swap(hs[i],hs[j]);

  // array => list
  for (i=0;i<0x100 && n[i];i++);
  for (h=NULL,u=i,i=0;i<u;i++) {
    p=(Node*)malloc(sizeof(Node));
    p->c=hs[i]; p->t=n[i];
    p->l=p->r=NULL; p->n=h; h=p;
  }
  // list => Huffman tree
  if (!h) goto none_chr;
  if (!h->n) goto one_chr;
  while (h->n) {
    p=(Node*)malloc(sizeof(Node));
    p->n=NULL;p->c=0;
    p->t=(p->l=h)->t;p->t+=(p->r=h->n)->t;
    if (!(q=h->n->n)) h=p; else
    if (p->t<q->t) (h=p)->n=q; else
    if (!q->n) (h=q)->n=p; else {
      for (h=q;q->n && q->n->t<p->t;q=q->n);
      p->n=q->n; q->n=p;
    }
  }

  // Huffman tree => tree header
  spacel=0;
  tree_header_len=0;
  push(h,sizeof(void*));
  while (!is_empty()) {
    pop(p,sizeof(void*));
    if (p->r || p->l) spacel++;
    else {
      tree_header[tree_header_len++]=spacel;
      tree_header[tree_header_len++]=p->c;
      spacel=0;
    }
    if (p->r) push(p->r,sizeof(void*));
    if (p->l) push(p->l,sizeof(void*));
    free(p);
  }

  // list(1) => tree header
  goto multy_chr;
  one_chr:
  tree_header_len=4;
  tree_header[0]=1;
  tree_header[1]=h->c;
  tree_header[2]=0;
  tree_header[3]=h->c;
  goto multy_chr;
  // list(0) => tree header
  none_chr:
  tree_header_len=4;
  tree_header[0]=1;
  tree_header[1]=0;
  tree_header[2]=0;
  tree_header[3]=0;
  multy_chr:


  // tree header => bin list
  binl_len=0x20; binl=0;
  for (i=0;i<tree_header_len;i+=2) {
    binl_len-=tree_header[i];
    c01[tree_header[i+1]]=binl;
    l01[tree_header[i+1]]=0x20 -binl_len;
    binl+=1UL<<binl_len;
    while (!(binl&(1UL<<binl_len)) &&
	   binl_len<0x20) binl_len++;
  }

  // print file header (1)
  header_len=(tree_header_len+16+3)&(~3);
  fo=fopen(argv[2],"w+b");
  fseek(fo,8,SEEK_SET);
  fwrite(&header_len,      4,              1,fo);
  fwrite(&tree_header_len, 4,              1,fo);
  fwrite( tree_header,     tree_header_len,1,fo);

  // print file main body
  fseek(fo,header_len,SEEK_SET);
  fi=fopen(argv[1],"rb");
  file_len=0;
  buffer=0; buffer_len=0;
  while (1) {
    fread(&ic,1,1,fi);
    if (feof(fi)) break;
    if (buffer_len+l01[ic]<=0x20) {
      buffer|=c01[ic]>>buffer_len;
      buffer_len+=l01[ic];
    } else {
      buffer|=c01[ic]>>buffer_len;
      print(fo,buffer);
      buffer=c01[ic]<<(0x20-buffer_len);
      buffer_len=l01[ic]+buffer_len-0x20;
    }
  }
  if (buffer_len) print(fo,buffer);
  last_len=buffer_len;
  fclose(fi);

  // print file header (2)
  fclose(fo);
  fo=fopen(argv[2],"r+b");
  fwrite( th,              2,              1,fo);
  fwrite(&ver,             1,              1,fo);
  fwrite(&last_len,        1,              1,fo);
  fwrite(&file_len,        4,              1,fo);
  fclose(fo);

  return 0;
}


