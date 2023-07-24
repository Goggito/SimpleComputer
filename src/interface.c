#include "../include/interface.h"
#include "../include/accum.h"
#include "../include/big.h"
#include "../include/cell.h"

int cell_addr = 0;

void
GoInput ()
{
  mt_gotoXY (1, 25);
  mt_write ("\033[2K\r");
}

void
GoOutput ()
{
  mt_gotoXY (1, 26);
  mt_write ("\033[2K\r");
}

void
FlagPrint ()
{
  char buf[3];
  int value = 0;
  sc_regGet (OVRF, &value);
  snprintf (buf, 3, "%c ", (value) ? 'O' : ' ');
  write (STDOUT_FILENO, buf, strlen (buf));
  sc_regGet (NULD, &value);
  snprintf (buf, 3, "%c ", (value) ? 'N' : ' ');
  write (STDOUT_FILENO, buf, strlen (buf));
  sc_regGet (MOUT, &value);
  snprintf (buf, 3, "%c ", (value) ? 'M' : ' ');
  write (STDOUT_FILENO, buf, strlen (buf));
  sc_regGet (UNIF, &value);
  snprintf (buf, 3, "%c ", (value) ? 'U' : ' ');
  write (STDOUT_FILENO, buf, strlen (buf));
  sc_regGet (INVL, &value);
  snprintf (buf, 3, "%c ", (value) ? 'I' : ' ');
  write (STDOUT_FILENO, buf, strlen (buf));
}

void
CounterPrint ()
{
  int counter = 0;
  sc_counterGet (&counter);

  char buf[6];
  snprintf (buf, 6, "%4d", counter);
  write (1, buf, 5);
}

void
OperationPrint (int address)
{
  int value = 0;
  int command = 0;
  int operand = 0;

  sc_memoryGet (address, &value);
  sc_commandDecode (value, &command, &operand);

  char buf[10];
  snprintf (buf, 10, "%02X : %d", command, operand);
  write (1, buf, 9);
}

void
UIUpdate ()
{
  for (int i = 0; i < 100; i++)
    {
      CellPrint (i);
    }

  CellSelect (cell_addr);

  mt_gotoXY (70, 2);
  AccumPrint ();

  mt_gotoXY (71, 5);
  CounterPrint ();

  mt_gotoXY (70, 8);
  OperationPrint (cell_addr);

  mt_gotoXY (71, 11);
  FlagPrint ();

  BigCellPrint (1, 14, cell_addr);
}

void
UIOutput ()
{
  bc_box (1, 1, 61, 12);
  mt_gotoXY (27, 1);
  mt_write (" Memory \n");

  bc_box (62, 1, 83, 3);
  mt_gotoXY (67, 1);
  mt_write (" accumulator \n");

  bc_box (62, 4, 83, 6);
  mt_gotoXY (63, 4);
  mt_write (" instructionCounter \n");

  bc_box (62, 7, 83, 9);
  mt_gotoXY (68, 7);
  mt_write (" Operation \n");

  bc_box (62, 10, 83, 12);
  mt_gotoXY (69, 10);
  mt_write (" Flags \n");

  bc_box (1, 13, 45, 22);
  bc_box (46, 13, 83, 22);

  int keys_x = 47;
  int keys_y = 13;
  mt_gotoXY (keys_x, keys_y);
  mt_write ("Keys:");

  mt_gotoXY (keys_x, keys_y + 1);
  mt_write ("l - load\n");
  mt_gotoXY (keys_x, keys_y + 2);
  mt_write ("s - save\n");
  mt_gotoXY (keys_x, keys_y + 3);
  mt_write ("r - run\n");
  mt_gotoXY (keys_x, keys_y + 4);
  mt_write ("t - step\n");
  mt_gotoXY (keys_x, keys_y + 5);
  mt_write ("i - reset\n");
  mt_gotoXY (keys_x, keys_y + 6);
  mt_write ("F5 - accumulator\n");
  mt_gotoXY (keys_x, keys_y + 7);
  mt_write ("F6 - instructionCounter\n");

  mt_gotoXY (1, 24);
  mt_write ("Input/Output:");
}