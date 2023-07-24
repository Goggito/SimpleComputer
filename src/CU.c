#include "../include/CU.h"
#include "../include/ALU.h"
#include "../include/interface.h"
#include "../include/timer.h"

int
read_value (int base16)
{
  rk_mytermregime (1, 1, 0, 1, 1);
  char str[256];
  fgets (str, 256, stdin);
  int pos = 0, num = 0, let = 0, minus = 0, plus = 0;
  char c;
  while ((c = str[pos++]))
    {
      if (c >= 'A' && c <= 'Z')
        c += ' '; // lower case

      if (c >= '0' && c <= '9')
        {
          num = base16 ? num << 4 | (c - '0') : (num * 10 + (c - '0'));
          let++;
        }
      else if (base16 && c >= 'a' && c <= 'f')
        {
          num = num << 4 | (c - 'a' + 10);
          let++;
        }
      else if (base16 && c == '-')
        {
          minus = 0x4000;
          plus = 0;
        }
      else if (base16 && c == '+')
        {
          minus = 0;
          plus = 1;
        }
    }

  if (let || minus || plus)
    {
      if (base16)
        {
          if (sc_commandEncode (num >> 8 & 127, num & 127, &num) != 0)
            {
              rk_mytermregime (0, 1, 10, 0, 0);
              return -1;
            }
        }
      else if (num > 0x3fff)
        num |= 0xffff; // Смотрите команду READ в УУ
      num |= minus;
      rk_mytermregime (0, 1, 10, 0, 0);
      return num;
    }

  rk_mytermregime (0, 1, 10, 0, 0);
  return -1;
}

int
read_op (int position)
{
  while (1)
    {
      GoInput ();
      printf ("%u< ", position);
      int value = read_value (0);

      if (value == -2)
        return 2;
      if (value == -1)
        {
          GoInput ();
          printf ("%u< Nothing entered", position);
          GoOutput ();
        }
      else if (value > 32767)
        {
          GoInput ();
          printf ("%u< Number too large", position);
          GoOutput ();
        }
      else
        {
          GoOutput ();
          value = value | (1 << (14));
          if (sc_memorySet (position, value))
            return 3;
          return 0;
        }
    }
  GoOutput ();
}

int
CU ()
{
  int counter = 0;
  int value = 0;
  int command = 0;
  int operand = 0;

  sc_counterGet (&counter);
  sc_memoryGet (counter, &value);
  sc_commandDecode (value, &command, &operand);

  switch (command)
    {
    case 0x10: // READ
      if (operand >= 0 && operand < 100)
        {
          TimerStop ();
          // read_op (operand);
          // rk_mytermregime (0, 1, 10, 0, 0);

          rk_mytermregime (1, 1, 0, 1, 1);
          char buff[32];
          GoInput ();
          sprintf (buff, "Input value: ");
          write (STDOUT_FILENO, buff, 14);
          char buffAf[32];
          fgets (buffAf, 32, stdin);
          sc_memorySet (operand, atoi (buffAf));
          rk_mytermregime (0, 1, 10, 0, 0);

          TimerStart ();
          sc_counterForward ();
        }
      else
        {
          return -1;
        }
      break;

    case 0x11: // WRITE
      char buf[20];

      if (sc_memoryGet (operand, &value)
          || sc_commandDecode (value & 0x3FFF, &command, &operand))
        {
          sc_regSet (4, 1);
          return -1;
        }

      snprintf (buf, 20, "Output:> %c%02X%02X", (value & 0x4000) ? '-' : '+',
                command, operand);

      GoOutput ();
      write (1, buf, 15);

      sc_counterForward ();
      break;

    case 0x20: //  LOAD
      value = 0;
      sc_memoryGet (operand, &value);
      sc_accSet (value);
      sc_counterForward ();
      break;

    case 0x21: // STORE
      value = 0;
      sc_accGet (&value);
      sc_memorySet (operand, value);
      sc_counterForward ();
      break;

    case 0x40: // JUMP
      if (operand >= 0 && operand < 100)
        {
          sc_counterSet (operand);
          sc_counterGet (&counter);
          cell_addr = counter;
        }
      else
        {
          return -1;
        }
      break;

    case 0x43: // HALT
      sc_regSet (UNIF, 1);
      break;

    case 0x30:
    case 0x31:
    case 0x32:
    case 0x33:
    case 0x41:
    case 0x42:
    case 0x76:
      ALU (command, operand);
      break;

    default:
      return -1;
      break;
    }

  return 0;
}