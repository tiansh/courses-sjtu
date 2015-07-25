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

typedef struct {                       /* 严禁修改该结构体                */
  char            name[11];            /* 字段名称，最多10个字符，大写    */
  char            type_of_field;       /* 类型N:数字 D:日期 L:逻辑 C:字符 */
  unsigned long   field_data_address;  /* 偏移量                          */
  unsigned char   length;              /* 字段长度                        */
  unsigned char   dec;                 /* 小数位数（仅对N有效）           */
  char            reserved_bytes[14];  /* 保留占位用                      */
} FIELD_RECORD;

typedef struct {
  char            filename[80];  /* 文件路径                              */
  char            status;        /* 0=close 1=read 2=update               */
  FILE           *file_ptr;      /* 文件指针                              */
  /* 以下几项严禁修改                                                     */
  unsigned char   version;       /* 03h=无memo 83h=有memo                 */
  unsigned char   update_y;      /* 最后修改年份                          */
  unsigned char   update_m;      /* 月                                    */
  unsigned char   update_d;      /* 日                                    */
  unsigned long   records;       /* 记录数                                */
  unsigned short  header_len;    /* 头长度                                */
  unsigned short  record_len;    /* 每个记录的长度                        */
  /* 以上几项严禁修改                                                     */
  FIELD_RECORD   *fields;        /* 表单每一项表头信息                    */
  NDX           **ndxs;
} DBF;

void d_open    (DBF *d);                             /* 文件打开 USE      */
void d_close   (DBF *d);                             /* 文件关闭 CLOSE    */
void d_getrec  (DBF *d, unsigned long  n, char *current_rec);           /* 读入记录 EDIT     */
void d_getfld  (DBF *d, unsigned short n, char *r, char *current_rec);  /* 读入字段 EDIT     */
void d_putfld  (DBF *d, unsigned short n, char *r, char *current_rec);  /* 读入字段 EDIT     */
void d_putrec  (DBF *d, unsigned long  n, char *current_rec);           /* 写出记录 EDIT     */
void d_delrec  (DBF *d, unsigned long  n);           /* 标记删除 DELETE   */
void d_rclrec  (DBF *d, unsigned long  n);           /* 标记恢复 RECALL   */
void d_addrec  (DBF *d, char *current_rec);                             /* 追加记录 APPEND   */
unsigned long
     d_search  (DBF *d, unsigned short n, unsigned long start, char *r);  /* 查找记录          */
void d_addndx  (DBF *d, unsigned short n, char* path);
/* 所有计数从1开始计数，fld0表示是否删除的位置，查找返回0为失败。				  */

#ifdef __cplusplus
}
#endif

#endif
