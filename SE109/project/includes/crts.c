#include "crts.h"
#include <conio.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef __MSDOS__
#include <windows.h>
void gotoxy(const int x,const int y) {
  COORD coord = {x, y};
  SetConsoleCursorPosition ( GetStdHandle ( STD_OUTPUT_HANDLE ), coord );
}
int wherex() {
  CONSOLE_SCREEN_BUFFER_INFO info={0};  
  GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ) , &info );  
  return info.dwCursorPosition.X;
}
int wherey() {
  CONSOLE_SCREEN_BUFFER_INFO info={0};  
  GetConsoleScreenBufferInfo( GetStdHandle( STD_OUTPUT_HANDLE ) , &info );  
  return info.dwCursorPosition.Y;
}
void clrscr() { system("CLS"); }
char readkey() { return getch(); }
char keypressed() { return kbhit(); }
#endif

#ifdef __cplusplus
}
#endif
