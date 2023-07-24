#include "myTerm.h"

#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

int
mt_write (const char *text)
{
  write (STDOUT_FILENO, text, strlen (text));

  return 0;
}

int
mt_clrscr ()
{
  mt_write ("\033[H\033[2J");

  return 0;
}

int
mt_gotoXY (int x, int y)
{
  if (x < 0 || y < 0)
    {
      return (-1);
    }

  char buf[16];
  sprintf (buf, "\033[%d;%dH", y, x);
  write (STDOUT_FILENO, buf, strlen (buf));

  return 0;
}

int
mt_getscreensize (int *rows, int *cols)
{
  struct winsize ws;

  if (!ioctl (1, TIOCGWINSZ, &ws))
    {
      *rows = ws.ws_row;
      *cols = ws.ws_col;
      return (0);
    }
  else
    {
      return (-1);
    }
}

int
mt_setfgcolor (enum colors color)
{
  if ((color < 30 || color > 37) && color != 0)
    {
      return (-1);
    }

  char buf[16];
  sprintf (buf, "\033[%dm", color);
  write (STDOUT_FILENO, buf, strlen (buf));

  return (0);
}

int
mt_setbgcolor (enum colors color)
{
  if ((color < 30 || color > 37) && color != 0)
    {
      return (-1);
    }

  if (color != 0)
    {
      color += 10;
    }

  char buf[16];
  sprintf (buf, "\033[%dm", color);
  write (STDOUT_FILENO, buf, strlen (buf));

  return (0);
}
