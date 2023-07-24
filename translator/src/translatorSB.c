#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINES 100
#define MAX_LINE_LENGTH 256

// Структура для представления строки программы Simple Basic
typedef struct
{
  int lineNumber;
  int start;
} SimpleBasicLine;

typedef struct
{
  int address;
  char name;
} Variable;

SimpleBasicLine lineStart[MAX_LINE_LENGTH];
Variable VarList[26];
int var_counter = 99;

void
InitVarList ()
{
  for (int i = 0; i < 26; i++)
    {
      VarList[i].name = 0;
      VarList[i].address = -1;
    }
}

int
VariableAddressing (char name)
{
  VarList[name - 65].name = name;
  VarList[name - 65].address = var_counter;

  var_counter--;
  return 0;
}

int
VariableCreate (char *name)
{
  char var_name = name[0];
  if (!(var_name >= 'A' && var_name <= 'Z'))
    {
      return -1;
    }

  if (VarList[var_name - 65].address < 0)
    {
      VariableAddressing (var_name);
    }

  return VarList[var_name - 65].address;
}

// Функция для чтения программы Simple Basic из файла
int
SimpleBasicProgram (const char *filename_in, const char *filename_out,
                    SimpleBasicLine *program)
{
  int lineCount = 0;
  char line_buffer[MAX_LINE_LENGTH];

  InitVarList ();

  FILE *file_in = fopen (filename_in, "r");
  if (file_in == NULL)
    {
      printf ("Ошибка при открытии файла.\n");
      return 0;
    }

  FILE *file_out = fopen (filename_out, "w");
  if (file_out == NULL)
    {
      printf ("Ошибка при создании файла.\n");
      return 0;
    }

  // Чтение файла построчно
  while (fgets (line_buffer, MAX_LINE_LENGTH, file_in) != NULL)
    {
      // Игнорируем строки с комментариями
      if (line_buffer[0] == '\n')
        continue;

      // Разделение строки на поля
      char *number = strtok (line_buffer, " \t");
      char *command = strtok (NULL, " \t");

      printf ("\n %s %s", number, command);

      if (strcmp (command, "REM") == 0)
        {
          continue;
        }
      else if (strcmp (command, "INPUT") == 0)
        {
          char *operand = strtok (NULL, " \t\n");
          int address = VariableCreate (operand);
          fprintf (file_out, "%d READ %d\n", lineCount, address);
          lineCount++;
        }
      else if (strcmp (command, "OUTPUT") == 0)
        {
          char *operand = strtok (NULL, " \t\n");
          int address = VariableCreate (operand);
          fprintf (file_out, "%d WRITE %d\n", lineCount, address);
          lineCount++;
        }
      else if (strcmp (command, "GOTO") == 0)
        {
        }
      else if (strcmp (command, "IF") == 0)
        {
        }
      else if (strcmp (command, "LET") == 0)
        {
        }
      else if (strcmp (command, "END") == 0)
        {
        }
    }

  // DEB ########
  printf ("\n");
  for (int i = 0; i < 26; i++)
    {
      printf ("\n VAR %c ADR %d", VarList[i].name, VarList[i].address);
    }

  fclose (file_in);
  return lineCount;
}

int
main ()
{
  const char *inputFilename
      = "input_file.sb"; // Имя входного файла Simple Basic
  const char *outputFilename
      = "output_file.sb"; // Имя выходного файла Simple Assembler

  SimpleBasicLine program[MAX_LINES];
  int lineCount = SimpleBasicProgram (inputFilename, outputFilename, program);
  if (lineCount > 0)
    {
      // writeSimpleAssemblerProgram (outputFilename, program, lineCount);
      printf ("Программа успешно преобразована в файл Simple Assembler.\n");
    }

  return 0;
}
