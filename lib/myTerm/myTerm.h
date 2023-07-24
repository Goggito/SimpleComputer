#pragma once

#ifndef __myTerm__
#define __myTerm__

#define ESC "\033"

enum colors
{
  BLACK = 30,
  RED = 31,
  GREEN = 32,
  YELLOW = 33,
  BLUE = 34,
  MAGENTA = 35,
  CYAN = 36,
  WHITE = 37,
  RESET = 0
};

int mt_write (const char *text);

int mt_clrscr (void);

int mt_gotoXY (int x, int y);

int mt_getscreensize (int *rows, int *cols);

int mt_setfgcolor (enum colors);

int mt_setbgcolor (enum colors);

#endif