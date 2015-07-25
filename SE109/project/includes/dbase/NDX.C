#include "ndx.h"

#ifndef bool
#define bool char
#define true  ((bool)1)
#define false ((bool)0)
#endif


typedef struct {
  unsigned long left_page;
  unsigned long index;
  char          key_data[1];
} NDX_NODE;
typedef struct {
  unsigned long   page_index;
  unsigned long   key_num;
  NDX_NODE      **keys;
} NDX_PAGE;

NDX_PAGE pages[64];

static void get_page(NDX *n, unsigned long index, unsigned char num) {
  unsigned char i;
  fseek(n->file_ptr,0x200*index,SEEK_SET);
  pages[num].page_index=index;
  fread(&(pages[num].key_num),4,1,n->file_ptr);
  pages[num].keys=(NDX_NODE**)malloc(pages[num].key_num*sizeof(NDX_NODE*));
  for (i=0;i<pages[num].key_num;i++) {
    pages[num].keys[i]=(NDX_NODE*)malloc(n->key_length+8);
    fread(pages[num].keys[i],n->key_length+8,1,n->file_ptr);
  }
}
static void put_page(NDX *n, unsigned char num) {
  unsigned char i;
  fseek(n->file_ptr,0x200*pages[num].page_index,SEEK_SET);
  fwrite(&pages[num].key_num,4,1,n->file_ptr);
  for (i=0;i<pages[num].key_num;i++)
    fwrite(pages[num].keys[i],n->key_length+8,1,n->file_ptr);
}
static void dispose_page(unsigned char num) {
  unsigned char i;
  for (i=0;i<pages[num].key_num;i++)
    free(pages[num].keys[i]);
  free(pages[num].keys);
}

void n_open(NDX *n) {
  n->file_ptr=fopen(n->filename,"r+b");
  if (!n->file_ptr)
  { fprintf(stderr,"\n打开索引文件 \"%s\" 时出错，程序终止。\n",
      n->filename); exit(-1); }
  fread(&(n->root_page),0x12,1,n->file_ptr);
  fread(&(n->record_size),0x11,1,n->file_ptr);
}

void n_close(NDX *n) {
  fclose(n->file_ptr);
}

static void strscpy(char* a, char* b, int len) {
  while (*b && len--) *(a++)=*(b++);
  while (len--) *(a++)=' ';
}

unsigned long n_fndrec (NDX *n, char *r) {
  char top, i;
  int cmp;
  char *rs;
  unsigned long page_index;
  rs=(char*)malloc(n->key_length);
  strscpy(rs,r,n->key_length);
  page_index=n->root_page; top=-1;
  while (page_index) {
    get_page(n,page_index,++top);
    for (i=0;i<pages[top].key_num;i++) {
      cmp=memcmp(rs,pages[top].keys[i]->key_data,n->key_length);
      if (cmp==0 && pages[top].keys[i]->index!=0)
	return pages[top].keys[i]->index;
      if (cmp<=0) { page_index=pages[top].keys[i]->left_page; break; }
    }
    if (i==pages[top].key_num) return 0;
  }
  while (top>=0) dispose_page(top--);
  return 0;
}

static void addrec(NDX *n, unsigned char num, NDX_NODE* data) {
  char i;
  int cmp;
  NDX_NODE **nkeys=(NDX_NODE**)
   malloc(sizeof(NDX_NODE*)*(pages[num].key_num+1));
  for (i=pages[num].key_num++;i;i--) {
    cmp=memcmp(data->key_data,pages[num].keys[i-1]->key_data,n->key_length);
    if (cmp>0 || cmp==0 && pages[num].keys[i-1]->index>data->index) break;
    nkeys[i]=pages[num].keys[i-1];
  }
  nkeys[i]=data;
  for (;i;i--) nkeys[i-1]=pages[num].keys[i-1];
  free(pages[num].keys);
  pages[num].keys=nkeys;
}

NDX_NODE* make_node(NDX *n,
 unsigned long left_page, unsigned long index, char* key_data) {
  NDX_NODE* key;
  key=(NDX_NODE*)malloc(n->key_length+8);
  key->left_page=left_page;
  key->index=index;
  memcpy(key->key_data,key_data,n->key_length);
  return key;
}

void spilt_page(NDX* n, unsigned char num, unsigned char top)  {
  unsigned char spos, i;
  spos=(pages[num].key_num)/2;
  pages[top].key_num=pages[num].key_num-spos;
  pages[top].page_index=++n->total_pages;
  pages[top].keys=(NDX_NODE**)malloc(pages[top].key_num*sizeof(NDX_NODE*));
  for (i=spos;i<pages[num].key_num;i++)
    pages[top].keys[i-spos]=pages[num].keys[i];
  pages[num].key_num=spos;
}

NDX_NODE* get_last(NDX* n, unsigned char num) {
  NDX_NODE* keys;
  keys=(NDX_NODE*)malloc(8+n->key_length);
  memcpy(keys,pages[num].keys[pages[num].key_num-1],n->key_length+8);
  return keys;

}

void make_page(NDX* n, unsigned char num1,
 unsigned char num2, unsigned char top) {
  pages[top].page_index=++n->total_pages;
  pages[top].key_num=2;
  pages[top].keys=(NDX_NODE**)malloc(2*sizeof(NDX_NODE*));
  pages[top].keys[0]=get_last(n,num1);
  pages[top].keys[1]=get_last(n,num2);
}

void write_total(NDX* n) {
  fseek(n->file_ptr,4,SEEK_SET);
  fwrite(&n->total_pages,4,1,n->file_ptr);
}

void n_addrec (NDX *n, char *r, unsigned long index_num) {
  unsigned char i, num;
  char *rs;
  int cmp, top;
  unsigned long page_index;
  bool changed[64], header_changed;
  NDX_NODE* key;
  key=(NDX_NODE*)malloc(8+n->key_length);
  rs=(char*)malloc(n->key_length);
  strscpy(rs,r,n->key_length);
  memset(changed,0,sizeof(changed));
  header_changed=false;

  page_index=n->root_page; top=-1;
  while (page_index) {
    get_page(n,page_index,++top);
    if (pages[top].keys[0]->index) break;
    for (i=0;i<pages[top].key_num;i++) {
      cmp=memcmp(rs,pages[top].keys[i]->key_data,n->key_length);
      if ((cmp<0) || (cmp==0 && index_num<pages[top].keys[i]->index))
       { page_index=pages[top].keys[i]->left_page; break; }
    }
    if (i==pages[top].key_num) {
      memcpy(pages[top].keys[--i],rs,n->key_length);
      changed[i]=true;
      page_index=pages[top].keys[--i]->left_page;
    }
  }
  key=make_node(n,0,index_num,rs);
  addrec(n,num=top,key);
  changed[top]=true;
  while (pages[num].key_num>n->max_keys && num>1) {
    spilt_page(n,num,++top);
    header_changed=true;
    addrec(n,num-1,get_last(n,num));
    changed[num]=changed[top]=true;
  }
  if (pages[num].key_num>n->max_keys) {
    spilt_page(n,num,++top);
    header_changed=true;
    changed[num]=changed[top]=true;
    make_page(n,num,top,top+1); top++;
    changed[top]=true;
  }
  for (i=0;i<=top;i++) {
    if (changed[i]) put_page(n,i);
    dispose_page(i);
  }
  if (header_changed) write_total(n);
}
