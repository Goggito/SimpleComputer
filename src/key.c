#include "../include/key.h"
#include "../include/CU.h"
#include "../include/accum.h"
#include "../include/cell.h"
#include "../include/interface.h"
#include "../include/timer.h"

void
KeyRead (int *address, int *exit_cheack)
{
  char filename[256];

  enum keys key;
  rk_readkey (&key);
  switch (key)
    {
    case 0: // UP
      if (*address > 9)
        {
          *address -= 10;
        }
      UIUpdate ();
      break;

    case 1: // DOWN
      if (*address <= 89)
        {
          *address += 10;
        }

      UIUpdate ();
      break;

    case 2: // LEFT
      if (*address > 0)
        {
          *address -= 1;
        }
      UIUpdate ();
      break;

    case 3: // RIGHT
      if (*address < 99)
        {
          *address += 1;
        }
      UIUpdate ();
      break;

    case 4: // Insert
      CellEnter (*address);
      UIUpdate ();
      break;

    case 34: // Load
      GoInput ();
      rk_mytermregime (1, 1, 1, 1, 1);

      write (1, "Enter filename for load: ", 26);
      fgets (filename, 256, stdin);
      filename[strlen (filename) - 1] = 0;
      GoOutput ();
      if (sc_memoryLoad (filename))
        {
          mt_write ("File not found!");
        }

      rk_mytermregime (0, 1, 1, 0, 0);

      UIUpdate ();
      break;

    case 41: // Save
      GoInput ();

      rk_mytermregime (1, 1, 1, 1, 1);
      GoInput ();
      write (1, "Enter filename for save: ", 26);
      fgets (filename, 256, stdin);
      filename[strlen (filename) - 1] = 0;
      sc_memorySave (filename);

      rk_mytermregime (0, 1, 1, 0, 0);
      UIUpdate ();
      break;

    case 40: // Run
      TimerStart ();
      int value = 0;
      sc_counterGet (&value);
      cell_addr = value;
      sc_regSet (UNIF, 0);
      UIUpdate ();
      break;

    case 42: // Step
      // HandlerSIGALRM (SIGALRM);
      sc_counterSet (cell_addr);
      CU ();
      TimerStop ();
      UIUpdate ();
      break;

    case 31: // Reset
      raise (SIGUSR1);
      GoOutput ();
      GoInput ();
      break;

    case 12: // Accum
      AccumEnter (*address);
      UIUpdate ();
      break;

    case 13: // Counter
      sc_counterSet (cell_addr);
      UIUpdate ();
      break;

    case 39: // Quit
      *exit_cheack = 0;
      break;

    default:
      break;
    }
}

void
KeyReadReset ()
{
  enum keys key;
  rk_readkey (&key);
  switch (key)
    {
    case 31: // RERSET
      raise (SIGUSR1);
      GoOutput ();
      GoInput ();
      break;

    default:
      break;
    }
}