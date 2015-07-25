#ifndef CRTS_H
#define CRTS_H

#ifdef __cplusplus
extern "C" {
#endif

// #Warning 这个单元只在DOS/Windows下可以正常工作。

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
