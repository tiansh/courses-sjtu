#ifndef CRTS_H
#define CRTS_H

#ifdef __cplusplus
extern "C" {
#endif

// #Warning �����Ԫֻ��DOS/Windows�¿�������������

#ifndef __TURBO__
#include <windows.h>
extern void gotoxy(const int x,const int y);
extern int  wherex();
extern int wherey();
extern void clrscr();
#endif
extern char readkey();
extern char keypressed();

#endif

#ifdef __cplusplus
}
#endif
