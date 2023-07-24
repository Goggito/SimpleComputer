#include "../include/usersignal.h"
#include "../include/interface.h"
#include "../include/timer.h"

int
HandlerSIGUSR1 (int signal)
{
  if (signal != SIGUSR1)
    {
      return 1;
    }

  TimerStop ();
  sc_regInit ();
  sc_regSet (UNIF, 1);
  sc_memoryInit ();
  sc_counterInit ();
  sc_accSet (0);
  cell_addr = 0;
  UIUpdate ();

  return 0;
}
