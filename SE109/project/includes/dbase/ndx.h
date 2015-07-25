#ifndef NDX_H
#define NDX_H

#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

typedef struct {
  char            filename[80];
  FILE           *file_ptr;         /* 以下为NDX文件头信息，严禁修改 */
  unsigned long   root_page;        /* 第一页位置                    */
  unsigned long   total_pages;      /* 总页面数                      */
  unsigned long   reserved1;        /* 保留                          */
  unsigned short  key_length;       /* 键长度                        */
  unsigned short  max_keys;         /* 每页中最大键数                */
  unsigned short  key_type;         /* 键类型                        */
  /* 这里编译器会增加两字节以对齐内存                                */
  /* 故读写时应当以此为界限分开读写                                  */
  unsigned long   record_size;      /* 记录大小                      */
  unsigned char   reserved2;        /* 保留                          */
  unsigned char   unique;           /* 独特的                        */
  char            key_name[11];     /* 键名称                        */
  /* 文件头到此为止                                                  */
} NDX;

void          n_open   (NDX *n);           /* 打开NDX文件            */
void          n_close  (NDX *n);           /* 释放内存               */
unsigned long n_fndrec (NDX *n, char *r);  /* 查找数据，返回对应编号 */
void          n_addrec (NDX *n, char *r, unsigned long index_num);
					   /* 增加一条记录           */

#endif
