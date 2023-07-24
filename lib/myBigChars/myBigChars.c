#include "myBigChars.h"

#include <fcntl.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

#include "lib/myTerm/myTerm.h"
//#include <wchar.h>

int
bc_printA (char *str)
{
  char buf[32];
  sprintf (buf, "\E(0%s\E(B\n", str);
  write (STDOUT_FILENO, buf, strlen (buf));

  return 0;
}

int
bc_box (int x1, int y1, int x2, int y2)
{
  mt_gotoXY (x1, y1);
  bc_printA (BOXCHAR_TL);

  for (int x = x1 + 1; x < x2; x++)
    {
      mt_gotoXY (x, y1);
      bc_printA (BOXCHAR_HOR);
    }

  mt_gotoXY (x2, y1);
  bc_printA (BOXCHAR_TR);

  mt_gotoXY (x1, y2);
  bc_printA (BOXCHAR_BL);

  for (int x = x1 + 1; x < x2; x++)
    {
      mt_gotoXY (x, y2);
      bc_printA (BOXCHAR_HOR);
    }

  mt_gotoXY (x2, y2);
  bc_printA (BOXCHAR_BR);

  for (int y = y1 + 1; y < y2; y++)
    {
      mt_gotoXY (x1, y);
      bc_printA (BOXCHAR_VERT);
    }

  for (int y = y1 + 1; y < y2; y++)
    {
      mt_gotoXY (x2, y);
      bc_printA (BOXCHAR_VERT);
    }

  return 0;
}

int
bc_printbigchar (int *A, int x, int y, enum colors fg, enum colors bg)
{
  int row, col, bit;

  for (col = 0; col < 8; col++)
    {
      for (row = 0; row < 8; row++)
        {
          bit = (A[col / 4] >> ((col % 4) * 8 + row)) & 1;

          mt_gotoXY (x + row, y + col);
          if (bit)
            {
              bc_printA (BOXCHAR_REC);
            }
          else
            {
              bc_printA (" ");
            }
        }
    }

  mt_setfgcolor (RESET);
  mt_setbgcolor (RESET);

  return 0;
}

int
bc_setbigcharpos (int *big, int x, int y, int value)
{
  int k = 0;

  int pos = x + (8 * (y - 1)) - 1;
  if (pos >= 32)
    {
      pos -= 32;
      k = 1;
    }

  if (value)
    {
      big[k] = big[k] | (1 << (pos));
    }
  else
    {
      big[k] = big[k] & (~(1 << (pos)));
    }

  return 0;
}

int
bc_getbigcharpos (int *big, int x, int y, int *value)
{
  int k = 0;

  int pos = x + (8 * (y - 1)) - 1;
  if (pos >= 32)
    {
      pos -= 32;
      k = 1;
    }

  *value = (big[k] >> (pos)) & 0x1;

  return 0;
}

int
bc_bigcharwrite (int fd, int **big, int count)
{
  if (!big)
    return 1;

  for (int really_count = 0; really_count < count; really_count++)
    {
      if (write (fd, big, sizeof (int)) < sizeof (int))
        return 1;
      big++;
    }
  return 0;
}

int
bc_bigcharread (int fd, int **big, int need_count, int *count)
{
  if (!big || !count)
    return 1;

  int really_count;
  for (really_count = 0; really_count < need_count; really_count++)
    {
      if (read (fd, (void *)&big[really_count], sizeof (int)) < sizeof (int))
        break;
    }

  *count = really_count + 1 == need_count ? really_count + 1 : 0;

  return 0;
}
