#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "dbf.h"

int main() {
/*  DBF *db=(DBF *)malloc(sizeof(DBF));*/
  NDX *index=(NDX*)malloc(sizeof(NDX));
  char username[32]; unsigned long int pagenum;
  strcpy(index->filename,"username.ndx");
  scanf("%s",username);
  scanf("%d",&pagenum);
  n_open(index);
  n_addrec(index,username,pagenum);
  printf("\n%d\n",n_fndrec(index,username));
  n_close(index);
/*  strcpy(db->filename,"user.dbf");/*"..\\..\\..\\user.dbf");*/
/*  d_open(db);*/

/*  d_close(db);*/
  return 0;
}
