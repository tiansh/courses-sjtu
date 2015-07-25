#include "dbf.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <time.h>

void d_open    (DBF *d) {
  d->status=1;
  d->file_ptr=fopen(d->filename,"r+b");
  fread(&(d->version),0xC,1,d->file_ptr);
  d->fields=(FIELD_RECORD *)malloc(d->header_len-0x21);
  fseek(d->file_ptr,0x20,SEEK_SET);
  fread(d->fields,d->header_len-0x21,1,d->file_ptr);
  d->ndxs=(NDX**)malloc((d->header_len-0x21)/0x20*sizeof(NDX*));
  memset(d->ndxs,0,(d->header_len-0x21)/0x20*sizeof(NDX*));
}

void d_close   (DBF *d) {
  unsigned short n;
  d->status=0;
  free(d->fields);
  free(d->ndxs);
  fclose(d->file_ptr);
  for (n=0;n<(d->header_len-0x21)/0x20;n++)
    if (d->ndxs[n]) n_close(d->ndxs[n]);
}

void d_getrec  (DBF *d, unsigned long  n, char *current_rec) {
  fseek(d->file_ptr,d->header_len+(n-1)*d->record_len,SEEK_SET);
  fread(current_rec,d->record_len,1,d->file_ptr);
}

void d_getfld  (DBF *d, unsigned short n, char *r, char *current_rec) {
  memcpy(r,current_rec+d->fields[n-1].field_data_address,
         d->fields[n-1].length);
  r[d->fields[n-1].length]=0;
}

static void updated(DBF *d) {
  time_t t=time(NULL);
  struct tm m;
  fseek(d->file_ptr,1,SEEK_SET);
  d->status=2;
  memcpy(&m,localtime(&t),sizeof(struct tm));
  m.tm_year%=100; m.tm_mon+=1;
  fwrite(&m.tm_year,1,1,d->file_ptr); d->update_y=m.tm_year;
  fwrite(&m.tm_mon ,1,1,d->file_ptr); d->update_y=m.tm_mon;
  fwrite(&m.tm_mday,1,1,d->file_ptr); d->update_y=m.tm_mday;
}

void d_putrec  (DBF *d, unsigned long  n, char *current_rec) {
  fseek(d->file_ptr,d->header_len+(n-1)*d->record_len,SEEK_SET);
  fwrite(current_rec,d->record_len,1,d->file_ptr);
  if (d->status==1) updated(d);
}

void d_putfld  (DBF *d, unsigned short n, char *r, char *current_rec) {
  memcpy(current_rec+d->fields[n-1].field_data_address,r,
	 d->fields[n-1].length);
}

void d_rclrec  (DBF *d, unsigned long n) {
  char t=' ', *c=&t;
  fseek(d->file_ptr,d->header_len+(n-1)*d->record_len,SEEK_SET);
  fwrite(c,1,1,d->file_ptr);
  if (d->status==1) updated(d);
}
void d_delrec  (DBF *d, unsigned long n) {
  char t='*', *c=&t;
  fseek(d->file_ptr,d->header_len+(n-1)*d->record_len,SEEK_SET);
  fwrite(c,1,1,d->file_ptr);
  if (d->status==1) updated(d);
}

void d_addrec  (DBF *d, char *current_rec) {
  int i;
  fseek(d->file_ptr,d->header_len+(d->records++)*d->record_len,SEEK_SET);
  fwrite(current_rec,d->record_len,1,d->file_ptr);
  fprintf(d->file_ptr,"\x1A");
  fseek(d->file_ptr,4,SEEK_SET);
  fwrite(&(d->records),sizeof(long),1,d->file_ptr);
  if (d->status==1) updated(d);
  for (i=0;i<(d->header_len-0x21)/0x20;i++)
    if (d->ndxs[i]) n_addrec(d->ndxs[i],
      current_rec+d->fields[i].field_data_address,d->records);
}

static bool compare_str(char *r1, char *r2,int len) {
  int i;
  for (i=0;r1[i] && r2[i] && i<len;i++)
    if (r1[i]!=r2[i]) return false;
  if (!r1[i] && !r2[i]) return true;
  if (r1[i]) for (;(r1[i]==' ' || !r1[i]) && i<len;i++)
    if (!r1[i]) return true; else;
  else for (;(r2[i]==' ' || !r2[i]) && i<len;i++)
    if (!r2[i]) return true; else;
  return (i==len);
}

unsigned long
     d_search  (DBF *d, unsigned short n, unsigned long start, char *r) {
  unsigned long i;
  char* current_rec;
  if (d->ndxs[n]) return n_fndrec(d->ndxs[n],r);
  current_rec=(char*)malloc(d->record_len);
  fseek(d->file_ptr,d->header_len,SEEK_SET);
  for (i=start;i<=d->records;++i) {
    fread(current_rec,d->record_len,1,d->file_ptr);
    if (compare_str(current_rec+d->fields[n-1].field_data_address,r,
	  d->fields[n-1].length)) return i;
  }
  return 0;
}

void d_addndx  (DBF *d, unsigned short n, char* path) {
  d->ndxs[n]=(NDX*)malloc(sizeof(NDX));
  strcpy(d->ndxs[n]->filename,path);
  n_open(d->ndxs[n]);
}


#ifdef __cplusplus
}
#endif
