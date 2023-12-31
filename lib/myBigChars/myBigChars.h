#pragma once

#ifndef __myBigChars__
#define __myBigChars__

#define ESC "\033"

#define BOXCHAR_REC "a"
#define BOXCHAR_BR "j"
#define BOXCHAR_BL "m"
#define BOXCHAR_TR "k"
#define BOXCHAR_TL "l"
#define BOXCHAR_VERT "x"
#define BOXCHAR_HOR "q"

#include "lib/myTerm/myTerm.h"

int bc_printA (char *str);

int bc_box (int x1, int y1, int x2, int y2);

int bc_printbigchar (int *A, int x, int y, enum colors fg, enum colors bg);

int bc_setbigcharpos (int *big, int x, int y, int value);

int bc_getbigcharpos (int *big, int x, int y, int *value);

int bc_bigcharwrite (int fd, int **big, int count);

int bc_bigcharread (int fd, int **big, int need_count, int *count);

#endif