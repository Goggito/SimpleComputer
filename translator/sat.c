#include <stdio.h>
#include <string.h>

#define MAX_LINE_LENGTH 256

int memory[100];

int
memorySet (int address, int value)
{
  if (address < 0 || address >= 100)
    {
      // printf ("\n###Memory out of bounds");
      return -1;
    }
  else
    {
      memory[address] = value;
    }

  return 0;
}

int
commandEncode (int command, int operand, int *value)
{
  if (operand < -127 || operand > 127)
    {
      // printf ("\n###Operand out of bounds");
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

void
translateToArray (int address, int command, int operand)
{
  int value = 0;

  commandEncode (command, operand, &value);
  memorySet (address, value);
}

void
translateCommand (const char *address, const char *command,
                  const char *operand)
{
  int addr = 0;
  int comm = 0;
  int oper = 0;

  addr = atoi (address);
  oper = atoi (operand);

  if (strcmp (command, "=") == 0)
    {
      oper = strtol (operand, NULL, 16);
      memorySet (addr, oper);
    }
  else if (strcmp (command, "READ") == 0)
    comm = 0x10;
  else if (strcmp (command, "WRITE") == 0)
    comm = 0x11;
  else if (strcmp (command, "LOAD") == 0)
    comm = 0x20;
  else if (strcmp (command, "STORE") == 0)
    comm = 0x21;
  else if (strcmp (command, "ADD") == 0)
    comm = 0x30;
  else if (strcmp (command, "SUB") == 0)
    comm = 0x31;
  else if (strcmp (command, "DIVIDE") == 0)
    comm = 0x32;
  else if (strcmp (command, "MUL") == 0)
    comm = 0x33;
  else if (strcmp (command, "JUMP") == 0)
    comm = 0x40;
  else if (strcmp (command, "JNEG") == 0)
    comm = 0x41;
  else if (strcmp (command, "JZ") == 0)
    comm = 0x42;
  else if (strcmp (command, "HALT") == 0)
    comm = 0x43;
  else if (strcmp (command, "SUBC") == 0)
    comm = 0x76;

  translateToArray (addr, comm, oper);
}

void
translateToObject (FILE *outputFile)
{
  fwrite (memory, sizeof (int), 100, outputFile);
}

int
main (int argc, char *argv[])
{
  if (argc != 3)
    {
      printf ("Usage: sat input_file.sa output_file.o\n");
      return 1;
    }

  const char *inputFileName = argv[1];
  const char *outputFileName = argv[2];

  FILE *inputFile = fopen (inputFileName, "r");
  if (inputFile == NULL)
    {
      printf ("Error opening input file: %s\n", inputFileName);
      return 1;
    }

  FILE *outputFile = fopen (outputFileName, "wb");
  if (outputFile == NULL)
    {
      printf ("Error opening output file: %s\n", outputFileName);
      fclose (inputFile);
      return 1;
    }

  char line[MAX_LINE_LENGTH];

  // Чтение файла построчно
  while (fgets (line, MAX_LINE_LENGTH, inputFile) != NULL)
    {
      // Игнорируем строки с комментариями
      if (line[0] == ';' || line[0] == '\n')
        continue;

      // Разделение строки на поля
      char *address = strtok (line, " \t");
      char *command = strtok (NULL, " \t");
      char *operand = strtok (NULL, " \t\n");

      // Вызов функции трансляции команды
      translateCommand (address, command, operand);
    }

  translateToObject (outputFile);

  fclose (inputFile);
  fclose (outputFile);

  return 0;
}
