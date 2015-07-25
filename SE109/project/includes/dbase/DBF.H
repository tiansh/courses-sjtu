#ifndef DBF_H
#define DBF_H

#include <stdio.h>
#include "ndx.h"

#ifdef __cplusplus
extern "C" {
#else
#ifndef bool
#define bool char
#define true  ((bool)1)
#define false ((bool)0)
#endif
#endif

typedef struct {                       /* �Ͻ��޸ĸýṹ��                */
  char            name[11];            /* �ֶ����ƣ����10���ַ�����д    */
  char            type_of_field;       /* ����N:���� D:���� L:�߼� C:�ַ� */
  unsigned long   field_data_address;  /* ƫ����                          */
  unsigned char   length;              /* �ֶγ���                        */
  unsigned char   dec;                 /* С��λ��������N��Ч��           */
  char            reserved_bytes[14];  /* ����ռλ��                      */
} FIELD_RECORD;

typedef struct {
  char            filename[80];  /* �ļ�·��                              */
  char            status;        /* 0=close 1=read 2=update               */
  FILE           *file_ptr;      /* �ļ�ָ��                              */
  /* ���¼����Ͻ��޸�                                                     */
  unsigned char   version;       /* 03h=��memo 83h=��memo                 */
  unsigned char   update_y;      /* ����޸����                          */
  unsigned char   update_m;      /* ��                                    */
  unsigned char   update_d;      /* ��                                    */
  unsigned long   records;       /* ��¼��                                */
  unsigned short  header_len;    /* ͷ����                                */
  unsigned short  record_len;    /* ÿ����¼�ĳ���                        */
  /* ���ϼ����Ͻ��޸�                                                     */
  FIELD_RECORD   *fields;        /* ����ÿһ���ͷ��Ϣ                    */
  NDX           **ndxs;
} DBF;

void d_open    (DBF *d);                             /* �ļ��� USE      */
void d_close   (DBF *d);                             /* �ļ��ر� CLOSE    */
void d_getrec  (DBF *d, unsigned long  n, char *current_rec);           /* �����¼ EDIT     */
void d_getfld  (DBF *d, unsigned short n, char *r, char *current_rec);  /* �����ֶ� EDIT     */
void d_putfld  (DBF *d, unsigned short n, char *r, char *current_rec);  /* �����ֶ� EDIT     */
void d_putrec  (DBF *d, unsigned long  n, char *current_rec);           /* д����¼ EDIT     */
void d_delrec  (DBF *d, unsigned long  n);           /* ���ɾ�� DELETE   */
void d_rclrec  (DBF *d, unsigned long  n);           /* ��ǻָ� RECALL   */
void d_addrec  (DBF *d, char *current_rec);                             /* ׷�Ӽ�¼ APPEND   */
unsigned long
     d_search  (DBF *d, unsigned short n, unsigned long start, char *r);  /* ���Ҽ�¼          */
void d_addndx  (DBF *d, unsigned short n, char* path);
/* ���м�����1��ʼ������fld0��ʾ�Ƿ�ɾ����λ�ã����ҷ���0Ϊʧ�ܡ�				  */

#ifdef __cplusplus
}
#endif

#endif