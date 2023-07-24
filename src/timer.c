#include "../include/timer.h"

#include "../include/CU.h"
#include "../include/interface.h"

struct itimerval timer, oval;

int
HandlerSIGALRM (int signal)
{
  int ignore_check = 0;
  int counter = 0;

  // for (int i=0; i < 5; i++)
  // {
  //   int check;
  //   sc_regGet (i, &check);
  //   if (check)
  //   {
  //     sc_regSet (3, 1);
  //     return;
  //   }
  // }

  if (signal != SIGALRM)
    {
      return 1;
    }

  sc_regGet (UNIF, &ignore_check);

  if (0 == ignore_check)
    {
      CU ();
      // sc_counterForward ();
      sc_counterGet (&counter);
      if (counter >= 100)
        {
          sc_counterSet (0);
          raise (SIGUSR1);
          TimerStop ();
          UIUpdate ();
          GoOutput ();
          GoInput ();

          return -1;
        }
      cell_addr = counter;
      ualarm (100000, 0);
    }

  UIUpdate ();
  return 0;
}

int
TimerInit (int sec)
{
  // struct itimerval timer;

  timer.it_interval.tv_sec = sec;
  timer.it_interval.tv_usec = 0;
  timer.it_value.tv_sec = 1;
  timer.it_value.tv_usec = 0;

  // setitimer (ITIMER_REAL, &timer, NULL);
  return 0;
}

void
TimerStart ()
{
  setitimer (ITIMER_REAL, &timer, &oval);
}

void
TimerStop ()
{
  alarm (0);
}