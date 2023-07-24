#include "../include/CU.h"
#include "../include/accum.h"
#include "../include/big.h"
#include "../include/cell.h"
#include "../include/interface.h"
#include "../include/key.h"
#include "../include/timer.h"
#include "../include/usersignal.h"
#include "lib/myBigChars/myBigChars.h"
#include "lib/myReadkey/myReadkey.h"
#include "lib/mySimpleComputer/mySimpleComputer.h"
#include "lib/myTerm/myTerm.h"

int
main ()
{
  int x_size = 0;
  int y_size = 0;

  int exit_cheack = 1;
  int size_cheak = 0;

  int value = 0;

  signal (SIGALRM, (__sighandler_t)HandlerSIGALRM);
  signal (SIGUSR1, (__sighandler_t)HandlerSIGUSR1);

  sc_regInit ();
  sc_memoryInit ();
  TimerInit (1);
  sc_regSet (UNIF, 1);

  mt_clrscr ();
  UIOutput ();
  UIUpdate ();

  // rk_mytermregime (0, 1, 1, 0, 0);
  while (exit_cheack)
    {
      mt_getscreensize (&y_size, &x_size);
      if ((x_size >= 86) && (y_size >= 25))
        {
          if (size_cheak)
            {
              size_cheak = 0;
              mt_clrscr ();
              UIOutput ();
            }
          GoInput ();
          sc_regGet (UNIF, &value);
          if (value)
            {
              rk_mytermregime (0, 1, 10, 0, 0);
              KeyRead (&cell_addr, &exit_cheack);
            }
          else
            {
              rk_mytermregime (0, 1, 0, 0, 1);
              KeyReadReset ();
            }
        }
      else
        {
          if (!size_cheak)
            mt_clrscr ();

          size_cheak = 1;
          mt_gotoXY (2, 2);
          mt_write ("\n Small Terminal \n Min. Size 86x25");
        }
      fflush (stdout);
    }

  mt_clrscr ();

  rk_mytermregime (1, 1, 1, 1, 1);
}
