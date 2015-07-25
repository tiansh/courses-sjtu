// this program is compiled under Turbo C++ 3.0

#define VERSION 1

#define ERR_FILE_TYPE_FAIL {                      \
  fprintf(stderr,"Unrecognized file.\n");         \
  return 1;                                       \
}
#define ERR_FILE_VERSION_FAIL  {                  \
  fprintf(stderr,"Unrecognized version.\n");      \
  return 2;                                       \
}
#define ERR_FILE_INPUT_FAIL    {                  \
  fprintf(stderr,"Input file broken.\n");         \
  return 3;                                       \
}

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
#define peek(data,size)  \
  memcpy(&(data),stack[top-1],(size))
#define is_empty()      \
  (!top)

struct Node{
  unsigned char c;
  struct Node *r, *l;
};
typedef struct Node Node;

struct Header{
  char th[2];
  unsigned char ver;
  unsigned char last_len;
  unsigned long file_len;
  unsigned long header_len;
  unsigned long tree_header_len;
};
typedef struct Header Header;

int main(int argc, char* argv[]) {
  void *stack[0x100]; int top;
  Header header;
  FILE *fi, *fo;
  unsigned char c;
  unsigned char tree_header[0x200];
  Node *h, *p, *q;
  unsigned long i;
  unsigned long bin_file_len;
  unsigned char bin_buffer_len;
  unsigned char buffer;
  unsigned char buffer_len;
  unsigned char binp;
  char outputpath[256];

  if (argc<3) return -1;

  stack_init();

  // input file header and check
  fi=fopen(argv[1],"rb");
  fread(&header,sizeof(Header),1,fi);
  if (header.th[0]!='T' || header.th[1]!='H') ERR_FILE_TYPE_FAIL;
  if (header.ver>VERSION) ERR_FILE_VERSION_FAIL;
  if (header.last_len>32 || header.file_len&3) ERR_FILE_INPUT_FAIL;

  // input Huffman tree
  fseek(fi,16,SEEK_SET);
  fread(tree_header,header.tree_header_len,1,fi);
  p=h=(Node*)malloc(sizeof(Node));
  h->c=0; h->l=h->r=0;
  push(h,sizeof(void*));
  tree_header[i=0]--;
  for (i=0;i<header.tree_header_len;i+=2) {
    if (is_empty()) ERR_FILE_INPUT_FAIL;
    tree_header[i]++;
    while (tree_header[i]--) {
      peek(p,sizeof(void*));
      q=(Node*)malloc(sizeof(Node));
      q->l=q->r=NULL; q->c=0;
      if (p->l) p->r=q; else p->l=q;
      push(q,sizeof(void*));
    }
    pop(p,sizeof(void*));
    p->c=tree_header[i+1];
    while (!is_empty()) {
      peek(p,sizeof(void*));
      if (p->r) { pop(p,sizeof(void*))}
      else break;
    }
  }
  if (!is_empty()) {
    fprintf(stderr,"Input file broken.");
    return 3;
  }

  // Huffman code => source code
  fseek(fi,header.header_len,SEEK_SET);
  fo=fopen(argv[2],"wb");
  buffer=binp=buffer_len=0; p=h;
  bin_file_len=header.file_len-4+header.last_len/8+1;
  bin_buffer_len=header.last_len%8;
  while (1) {
    if (!buffer_len) {
      if (feof(fi)) ERR_FILE_INPUT_FAIL;
      fread(&buffer,1,1,fi);
      binp='\1'<<7; buffer_len=8;
      if (bin_file_len==1) buffer_len=bin_buffer_len;
      if (!(bin_file_len--)) break;
    }
    if (buffer&binp) p=p->r; else p=p->l;
    binp>>=1; buffer_len--;
    if (!p) ERR_FILE_INPUT_FAIL;
    if (!p->l && !p->r) {
      fwrite(&p->c,1,1,fo);
      p=h;
    }
  }
  if (p!=h) ERR_FILE_INPUT_FAIL;
  fclose(fo);

  return 0;
}



