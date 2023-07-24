#include "../include/cell.h"

#include "../include/interface.h"

void
CellPrint (int address)
{
  int value, row, col, command, operand;

  row = address / 10;
  col = address % 10;

  sc_memoryGet (address, &value);
  sc_commandDecode (value, &command, &operand);

  char buf[6];
  snprintf (buf, 6, "%c%02X%02X", (value & 0x4000) ? '-' : '+', command,
            operand);

  mt_gotoXY (2 + col * 6, 2 + row);
  write (1, buf, 5);

  return;
}

void
CellSelect (int address)
{
  mt_setbgcolor (WHITE);
  mt_setfgcolor (BLACK);
  CellPrint (address);
  mt_setbgcolor (RESET);
}

void
CellEnter (int address)
{
  int command, operand, value;
  char input[7];

  value = 0;
  GoInput ();
  char buff[32];
  sprintf (buff, "Input value: ");
  write (STDOUT_FILENO, buff, 14);

  rk_mytermregime (1, 1, 1, 1, 1);
  read (STDIN_FILENO, input, 7);
  sscanf (input, "%x %d", &command, &operand);

  sc_commandEncode (command, operand, &value);
  sc_memorySet (address, value);

  rk_mytermregime (0, 1, 1, 0, 0);
}
