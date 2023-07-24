#include "../include/accum.h"

#include "../include/interface.h"

void
AccumPrint ()
{
  int accum, command, operand;

  sc_accGet (&accum);
  sc_commandDecode (accum, &command, &operand);
  
  char buf[6];
  snprintf (buf, 6, "%c%02X%02X", (accum & 0x4000) ? '-' : '+', command,
            operand);
  write (1, buf, 5);
}

void
AccumEnter (int address)
{
  int value = 0;
  char input[7];

  GoInput ();
  
  rk_mytermregime (1, 1, 1, 1, 1);
  read (STDIN_FILENO, input, 7);
  sscanf (input, "%d", &value);

  rk_mytermregime (0, 1, 1, 0, 0);

  // value = value | (1 << (14));

  sc_accSet (value);
}