#ifndef NDX_H
#define NDX_H

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

typedef struct {
  char            filename[80];
  FILE           *file_ptr;         /* ����ΪNDX�ļ�ͷ��Ϣ���Ͻ��޸� */
  unsigned long   root_page;        /* ��һҳλ��                    */
  unsigned long   total_pages;      /* ��ҳ����                      */
  unsigned long   reserved1;        /* ����                          */
  unsigned short  key_length;       /* ������                        */
  unsigned short  max_keys;         /* ÿҳ��������                */
  unsigned short  key_type;         /* ������                        */
  /* ������������������ֽ��Զ����ڴ�                                */
  /* �ʶ�дʱӦ���Դ�Ϊ���޷ֿ���д                                  */
  unsigned long   record_size;      /* ��¼��С                      */
  unsigned char   reserved2;        /* ����                          */
  unsigned char   unique;           /* ���ص�                        */
  char            key_name[11];     /* ������                        */
  /* �ļ�ͷ����Ϊֹ                                                  */
} NDX;

void          n_open   (NDX *n);           /* ��NDX�ļ�            */
void          n_close  (NDX *n);           /* �ͷ��ڴ�               */
unsigned long n_fndrec (NDX *n, char *r);  /* �������ݣ����ض�Ӧ��� */
void          n_addrec (NDX *n, char *r, unsigned long index_num);
					   /* ����һ����¼           */

#endif
