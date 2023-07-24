#include "../include/ALU.h"
#include "../include/interface.h"

int
ALU (int command, int operand)
{
  int a = 0;
  int b = 0;
  int addr = 0;
  int result = 0;
  int counter = 0;

  switch (command)
    {
    case 0x30: // ADD
      if (operand >= 0 && operand < 100)
        {
          sc_memoryGet (operand, &a);
          sc_accGet (&b);
          result = a + b;
          sc_accSet (result);
          sc_counterForward ();
        }

      break;

    case 0x31: // SUB
      if (operand >= 0 && operand < 100)
        {
          sc_memoryGet (operand, &a);
          sc_accGet (&b);
          sc_accSet (b - a);
          sc_counterForward ();
        }

      break;

    case 0x32: // DIVIDE
      if (operand >= 0 && operand < 100)
        {
          sc_memoryGet (operand, &a);
          sc_accGet (&b);
          sc_accSet (b / a);
          sc_counterForward ();
        }

      break;

    case 0x33: // MUL
      if (operand >= 0 && operand < 100)
        {
          sc_memoryGet (operand, &a);
          sc_accGet (&b);
          sc_accSet (b * a);
          sc_counterForward ();
        }

      break;

    case 0x41: // JNEG
      if (operand >= 0 && operand < 100)
        {
          sc_accGet (&b);

          if (b < 0)
            {
              sc_counterSet (operand);
              sc_counterGet (&counter);
              cell_addr = counter;
            }
          else
            {
              sc_counterForward ();
            }
        }

      break;

    case 0x42: // JZ
      if (operand >= 0 && operand < 100)
        {
          sc_accGet (&b);
          if (b == 0)
            {
              sc_counterSet (operand);
              sc_counterGet (&counter);
              cell_addr = counter;
            }
          else
            {
              sc_counterForward ();
            }
        }

      break;

    case 0x76: // SUBC
      if (operand >= 0 && operand < 100)
        {
          sc_memoryGet (operand, &a);
          sc_accGet (&addr);
          if (addr >= 0 && addr < 100)
            {
              sc_memoryGet (addr, &b);
              sc_accSet (a - b);
            }
          sc_counterForward ();
        }

      break;

    default:
      break;
    }

  return 0;
}