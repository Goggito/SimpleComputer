#include "mySimpleComputer.h"

#include <math.h>
#include <stdio.h>
#include <string.h>

int memory[100];
int accumulator = 0;
int counter = 0;
char registr_flag = 0;
int write_check = 0;
int comand_list[]
    = { 0x10, 0x11, 0x20, 0x21, 0x30, 0x31, 0x32, 0x33, 0x40, 0x41,
        0x42, 0x43, 0x51, 0x52, 0x53, 0x54, 0x55, 0x56, 0x57, 0x58,
        0x59, 0x60, 0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67, 0x68,
        0x69, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76 };

int
sc_memoryInit ()
{
  for (int i = 0; i < 100; i++)
    {
      memory[i] = 0;
    }

  return 0;
}

int
sc_memorySet (int address, int value)
{
  if (address < 0 || address >= 100)
    {
      // printf ("\n###Memory out of bounds");
      sc_regSet (MOUT, 1);
      return -1;
    }
  else
    {
      memory[address] = value;
    }

  return 0;
}

int
sc_memoryGet (int address, int *value)
{
  if (address < 0 || address >= 100)
    {
      // printf ("\n###Memory out of bounds");
      sc_regSet (MOUT, 1);
      return -1;
    }
  else
    {
      *value = memory[address];
    }

  return 0;
}

int
sc_memorySave (char *filename)
{
  FILE *file = fopen (filename, "wb");
  fwrite (memory, sizeof (int), 100, file);
  fclose (file);

  return 0;
}

int
sc_memoryLoad (char *filename)
{
  FILE *file = fopen (filename, "rb");
  if (file == NULL)
    {
      // printf("\n###Cannot open file. File not found");
      return -1;
    }
  else
    {
      fread (memory, sizeof (int), 100, file);
    }
  fclose (file);

  return 0;
}

int
sc_regInit (void)
{
  registr_flag = 0;

  return 0;
}

int
sc_regSet (int registr, int value)
{
  if ((registr != 1) && (registr != 2) && (registr != 4) && (registr != 8)
      && (registr != 16) && (registr != 32))
    {
      printf ("\n###Registr out of bounds");
      return -1;
    }

  else
    {
      if (value == 0)
        {
          reset_flag (registr, registr_flag);
        }
      else if (value == 1)
        {
          set_flag (registr, registr_flag);
        }
      else
        {
          printf ("\n###Value out of bounds");
          return -1;
        }
    }

  return 0;
}

int
sc_regGet (int registr, int *value)
{
  if ((registr != 1) && (registr != 2) && (registr != 4) && (registr != 8)
      && (registr != 16) && (registr != 32))
    {
      // printf ("\n###Registr out of bounds");
      return -1;
    }

  registr = log2 (registr);

  *value = ((registr_flag >> (registr)) & 0x1);

  return 0;
}

int
sc_accSet (int value)
{
  // value = value | (1 << (14));
  accumulator = value;
  return 0;
}

int
sc_accGet (int *value)
{
  *value = accumulator;
  return 0;
}

void
sc_counterInit ()
{
  counter = 0;
}

void
sc_counterForward ()
{
  counter++;
}

void
sc_counterSet (int value)
{
  counter = value;
}

void
sc_counterGet (int *value)
{
  *value = counter;
}

int
sc_commandEncode (int command, int operand, int *value)
{
  if (operand < -127 || operand > 127)
    {
      // printf ("\n###Operand out of bounds");
      return -1;
    }

  int check = 0;
  for (int i = 0; i < 40; i++)
    {
      if (command == comand_list[i])
        {
          check = 1;
        }
    }
  if (check == 0)
    {
      // printf ("\n###Command not found");
      return -1;
    }

  // int k = 1;
  int k = 13;

  // for (int i = 6; i >= 0; i--)
  for (int i = 6; i >= 0; i--)
    {
      int bit = (command >> (i)) & 0x1;
      if (bit == 1)
        {
          *value = *value | (1 << (k));
        }
      k--;
    }

  for (int i = 6; i >= 0; i--)
    {
      int bit = (operand >> (i)) & 0x1;
      if (bit == 1)
        {
          *value = *value | (1 << (k));
        }
      k--;
    }

  return 0;
}

int
sc_commandDecode (int value, int *command, int *operand)
{
  *command = 0;
  *operand = 0;

  int bit = 0;
  int k = 0;

  k = 6;
  for (int i = 13; i >= 7; i--)
    {
      bit = (value >> (i)) & 0x1;
      if (bit == 1)
        {
          *command = *command | (1 << (k));
        }
      k--;
    }

  k = 6;
  for (int i = 6; i >= 0; i--)
    {
      bit = (value >> (i)) & 0x1;
      if (bit == 1)
        {
          *operand = *operand | (1 << (k));
        }
      k--;
    }

  if (((value >> (0)) & 0x4000) != 0)
    {
      sc_regSet (INVL, 1);
      // printf ("\n###No command");
      return -1;
    }

  return 0;
}