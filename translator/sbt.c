#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void INPUT (char *arguments);
void PRINT (char *arguments);
void IF (char *arguments);
void GOTO (int address, int operand);
void function (char *command, char *arguments);
void parsRPN (char *rpn, char var);
void LET (char *arguments);
void END ();

#define ERR_EXPECTED_ADDRESS_OF_MEMORY_CELL -1
#define ERR_WRONG_COMMAND -2
#define ERR_WRONG_OPERAND -3
#define ERR_BY_ENCODE -4

struct variable
{
  char Name;
  int Address;
  int Value;
};

struct variable Variables[52];
int variableCount = 0;
char lastConstantName = 'a';

struct command
{
  int Number;
  char *Command;
  int Address;
};

FILE *input = NULL;
FILE *output = NULL;

const char commandID[][7]
    = { "REM", "INPUT", "PRINT", "GOTO", "IF", "LET", "END" };
int basicCommandCouner = 0;
int assemblerCommandCounter = 0;
struct command *program;

int gotoCounter = -1;
struct command *gotoRecords;

typedef struct NODE
{
  char data;
  struct NODE *next;
} Node;

void
stack_push (char data, Node **top)
{
  Node *tmp = (Node *)malloc (sizeof (Node));
  tmp->data = data;
  tmp->next = *top;
  *top = tmp;
}

char
stack_pop (Node **top)
{
  Node *tmp;
  char d;
  if (*top == NULL)
    {
      // printf("Stack/Queue is underflow!\n");
      return -1;
    }
  else
    {
      tmp = *top;
      *top = tmp->next;
      d = tmp->data;
      free (tmp);
      return d;
    }
}

char
stack_top (Node *top)
{
  if (top != NULL)
    {
      return top->data;
    }
  return 0;
}

int
checkPriority (char sign)
{
  switch (sign)
    {
    case '*':
    case '/':
      return 4;
    case '+':
    case '-':
      return 2;
    case '(':
    case ')':
      return 1;
    }
  return 0;
}

// Функция translateToRPN принимает в качестве аргументов строку инфиксного
// выражения и строку для хранения обратной польской записи
char *
translateToRPN (char *inf, char *rpn)
{
  // Создание корневого узла стека
  Node *root = NULL;
  int i = 0;
  int j = 0;

  // Пока не достигнут конец строки и не встречен символ новой строки
  while (inf[i] != '\0' && inf[i] != '\n')
    {
      char x = inf[i];

      // Если встречен операнд, добавляем его в выходную строку
      if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'))
        {
          rpn[j] = x;
          j++;
        }

      // Если встречена открывающая скобка, добавляем ее в стек
      else if (x == '(')
        {
          stack_push (x, &root);
        }

      // Если встречена закрывающая скобка, извлекаем элементы из стека, пока
      // не достигнем открывающую скобку и добавляем их в выходную строку
      else if (x == ')')
        {
          while (stack_top (root) != '(')
            {
              char c = stack_pop (&root);
              if (c != 0)
                {
                  rpn[j] = c;
                  j++;
                }
            }
          // Извлекаем открывающую скобку из стека
          stack_pop (&root);
        }

      // Если встречен оператор, извлекаем операторы из стека, пока их
      // приоритет не станет меньше приоритета текущего оператора, и добавляем
      // их в выходную строку
      else if (x == '+' || x == '-' || x == '*' || x == '/')
        {
          while (root != NULL
                 && checkPriority (root->data) >= checkPriority (x))
            {
              char c = stack_pop (&root);
              if (c != 0)
                {
                  rpn[j] = c;
                  j++;
                }
            }
          // Добавляем текущий оператор в стек
          stack_push (x, &root);
        }

      // Если встречен недопустимый символ, выводим сообщение об ошибке и
      // выходим из функции
      else if (x != ' ')
        {
          // free(rpn); // этот код, возможно, лишний
          fprintf (stderr, "Wrong expression!\n");
          exit (EXIT_FAILURE);
        }
      i++;
    }

  // Извлекаем все оставшиеся операторы из стека и добавляем их в выходную
  // строку
  while (root != NULL)
    {
      char c = stack_pop (&root);
      if (c != 0)
        {
          rpn[j] = c;
          j++;
        }
    }

  // Проверяем, что выходная строка корректно сформирована
  for (int k = 0; k < j; k++)
    {
      if (rpn[k] == '(' || rpn[k] == ')')
        {
          fprintf (stderr, "Check your expression!\n");
          exit (EXIT_FAILURE);
        }
    }

  // Добавляем нуль-терминальный символ в конец выходной строки
  rpn[j] = '\0';
  return rpn;
}

int
getVariableAddress (char name)
{
  // printf("%c ", name);
  for (int i = 0; i < 52; i++)
    {
      if (Variables[i].Name == name)
        {
          return Variables[i].Address;
        }

      if (!Variables[i].Name)
        {
          Variables[i].Name = name;
          Variables[i].Address = 99 - i;
          variableCount++;
          return Variables[i].Address;
        }
    }
  return 0;
}

char
intToConstant (int value)
{
  for (int i = 0; i < 52; i++)
    {
      if (!Variables[i].Name)
        {
          Variables[i].Name = lastConstantName;
          lastConstantName++;
          Variables[i].Address = 99 - i;
          Variables[i].Value = value;
          fprintf (output, "%.2i = %x\n", Variables[i].Address,
                   abs (Variables[i].Value));
          variableCount++;
          return Variables[i].Name;
        }

      if (Variables[i].Value)
        {
          if (Variables[i].Value == value)
            {
              return Variables[i].Name;
            }
        }
    }
  return 0;
}

char
preCalcProcessing (char *expr)
{
  char *ptr = strtok (expr, " =");
  char val;
  sscanf (ptr, "%c", &val);
  if (!((val) >= 'A' && (val) <= 'Z'))
    {
      fprintf (stderr, "NOT CORRECT!\n");
      exit (EXIT_FAILURE);
    }
  ptr = strtok (NULL, " ");
  char *eq = ptr;
  if (strcmp (eq, "=") != 0)
    {
      fprintf (stderr, "Wrong expression!\n");
      exit (EXIT_FAILURE);
    }
  ptr = strtok (NULL, "");
  char *exp = ptr;
  int i = 0;
  int pos = 0;
  int j = 0;
  int operat = 0;
  int flg = 1;
  int m = 0;
  char *assign = (char *)malloc (sizeof (char) * 255);
  for (int k = 0; k < strlen (exp); k++)
    {
      if (exp[k] == '-' && flg)
        {
          assign[m] = '0';
          m++;
        }
      flg = 0;
      if (exp[k] == '+' || exp[k] == '-' || exp[k] == '/' || exp[k] == '*')
        {
          operat++;
        }
      if (exp[k] == '+' || exp[k] == '-' || exp[k] == '/' || exp[k] == '*'
          || exp[k] == '(')
        {
          flg = 1;
        }

      assign[m] = exp[k];
      m++;
    }
  if (operat == 0) // 0+ перед ним, если перед минусом нет аргумента, то пишем
                   // 0 перед миунсом
    {
      sprintf (exp, "0 + %s", assign);
    }
  else
    {
      sprintf (exp, "%s", assign);
    }
  while (exp[i] != '\n' && exp[i] != '\0')
    {
      if (exp[i] >= '0' && exp[i] <= '9')
        {
          char num[256];
          j = 0;
          num[j] = exp[i];
          j++;
          pos = i;
          exp[i] = ' ';
          i++;
          while (exp[i] >= '0' && exp[i] <= '9')
            {
              num[j] = exp[i];
              j++;
              exp[i] = ' ';
              i++;
            }
          num[j] = '\0';
          exp[pos] = intToConstant (atoi (num));
        }
      i++;
    }
  sprintf (expr, "%s", exp);

  return val;
}

void
loadFile (const char *filename, const char *secondFilename)
{
  if ((input = fopen (filename, "r")) == NULL)
    {
      fprintf (stderr, "Can't open file: no such file.\n");
      exit (EXIT_FAILURE);
    }

  output = fopen (secondFilename, "w");
  return;
}

// Функция, отвечающая за перевод программы из языка BASIC в ассемблерный код
void
translation ()
{
  int instructionCounter = 1;
  // Считываем и подсчитываем количество команд в программе
  while (1)
    {
      char temp[255];
      fgets (temp, 255, input);
      if (feof (input))
        {
          break;
        }
      instructionCounter++;
    }
  basicCommandCouner = instructionCounter; // Записываем количество команд
  rewind (input); // Возвращаем указатель на начало файла

  // Выделяем память для массива команд программы и для массива gotoRecords
  program = (struct command *)malloc (sizeof (struct command)
                                      * instructionCounter);
  gotoRecords = (struct command *)malloc (sizeof (struct command)
                                          * instructionCounter);

  // Считываем команды программы и проверяем, удалось ли их считать
  for (int i = 0; i < instructionCounter; i++)
    {
      program[i].Command = (char *)malloc (sizeof (char) * 255);
      if (!fgets (program[i].Command, 254, input))
        {
          if (feof (input))
            {
              return;
            }
          else
            {
              fprintf (stderr, "Line %d: can't read line from file.\n", i++);
              return;
            }
        }
    }

  // Проходимся по всем командам и разбираем их на номер строки, команду и
  // аргументы
  for (int i = 0; i < instructionCounter; i++)
    {
      char *lin;
      char *thisCommand = (char *)malloc (sizeof (char) * 255);
      sprintf (thisCommand, "%s", program[i].Command);
      char *ptr = strtok (thisCommand, " ");
      lin = ptr;
      int line = atoi (lin);

      // Если не удалось прочитать номер строки, выводим ошибку и выходим из
      // функции
      if ((!line) && (strcmp (lin, "0") != 0) && (strcmp (lin, "00") != 0))
        {
          fprintf (stderr, "Line %d: expected line number.\n", i++);
          break;
        }

      // Проверяем, что номер строки больше, чем у предыдущей команды
      if (i - 1 >= 0)
        {
          if (line <= program[i - 1].Number)
            {
              fprintf (stderr, "Line %d: Wrong line number\n", i++);
              break;
            }
        }

      program[i].Number = line; // Записываем номер строки

      char *command;
      ptr = strtok (NULL, " ");
      command = ptr;
      char *arguments;
      ptr = strtok (NULL, "");
      arguments = ptr;
      program[i].Address
          = assemblerCommandCounter; // Записываем адрес команды помощью
                                     // счетчика ассемблерного кода

      // Если это не команда GOTO, вызываем соответствующую функцию с
      // аргументами
      if (strcmp (command, "GOTO") != 0)
        {
          function (command, arguments);
        }
      // Иначе увеличиваем счетчик GOTO и сохраняем строку, адрес и номер
      else
        {
          gotoCounter++;
          gotoRecords[gotoCounter].Number = program[i].Number;
          gotoRecords[gotoCounter].Command = program[i].Command;
          gotoRecords[gotoCounter].Address = program[i].Address;
          assemblerCommandCounter++;
        }
    }

  // Обрабатываем все команды GOTO в конце
  for (int i = 0; i <= gotoCounter; i++)
    {
      int address = gotoRecords[i].Address;
      char *ptr = strtok (gotoRecords[i].Command, " ");
      ptr = strtok (NULL, " ");
      ptr = strtok (NULL, "");
      int operand = atoi (ptr);
      GOTO (address, operand);
    }
}

void
INPUT (char *arguments)
{
  if (!((strlen (arguments) == 2) && (arguments[0] >= 'A')
        && (arguments[0] <= 'Z')))
    {
      fprintf (stderr, "Wrong variable name.\n");
      exit (EXIT_FAILURE);
    }

  fprintf (output, "%.2i READ %d\n", assemblerCommandCounter,
           getVariableAddress (arguments[0]));
  assemblerCommandCounter++;
}

void
PRINT (char *arguments)
{
  if (!((strlen (arguments) == 2) && (arguments[0] >= 'A')
        && (arguments[0] <= 'Z')))
    {
      fprintf (stderr, "Wrong variable name.\n");
      exit (EXIT_FAILURE);
    }

  fprintf (output, "%.2i WRITE %d\n", assemblerCommandCounter,
           getVariableAddress (arguments[0]));
  assemblerCommandCounter++;
}

void
parsRPN (char *rpn, char var)
{
  int i = 0;
  int depth = 0;
  int operand1, operand2;
  char memoryCounter = '1';
  while (rpn[i] != '\0' && rpn[i] != '\n')
    {
      char x = rpn[i];
      if ((x >= 'a' && x <= 'z') || (x >= 'A' && x <= 'Z'))
        {
          fprintf (output, "%.2i LOAD %d\n", assemblerCommandCounter,
                   getVariableAddress (x));
          assemblerCommandCounter++;
          fprintf (output, "%.2i STORE %d\n", assemblerCommandCounter,
                   getVariableAddress (memoryCounter));
          memoryCounter++;
          assemblerCommandCounter++;
          depth++;
        }
      if (x == '+' || x == '-' || x == '*' || x == '/')
        {
          if (depth < 2)
            {
              fprintf (stderr,
                       "Uncorrect LET statement, check your expression.\n");
              exit (EXIT_FAILURE);
            }
          else
            {
              operand1 = getVariableAddress (memoryCounter - 2);
              operand2 = getVariableAddress (memoryCounter - 1);
              fprintf (output, "%.2i LOAD %d\n", assemblerCommandCounter,
                       operand1); //закидываем самый правый операнд в акк
              assemblerCommandCounter++;
              switch (x)
                {
                case '+':
                  fprintf (output, "%.2i ADD %d\n", assemblerCommandCounter,
                           operand2);
                  assemblerCommandCounter++;
                  break;
                case '-': // SUB
                  fprintf (output, "%.2i SUB %d\n", assemblerCommandCounter,
                           operand2);
                  assemblerCommandCounter++;
                  break;
                case '/': // DIVIDE
                  fprintf (output, "%.2i DIVIDE %d\n", assemblerCommandCounter,
                           operand2);
                  assemblerCommandCounter++;
                  break;
                case '*': // MUL
                  fprintf (output, "%.2i MUL %d\n", assemblerCommandCounter,
                           operand2);
                  assemblerCommandCounter++;
                  break;
                }
              fprintf (output, "%.2i STORE %d\n", assemblerCommandCounter,
                       getVariableAddress (memoryCounter - 2));
              assemblerCommandCounter++;
              depth--;
              memoryCounter = memoryCounter - 1;
            }
        }
      i++;
    }
  if (depth != 1)
    {
      fprintf (stderr, "Uncorrect LET statement, check your expression.\n");
      exit (EXIT_FAILURE);
    }
  fprintf (output, "%.2i STORE %d\n", assemblerCommandCounter,
           getVariableAddress (var));
  assemblerCommandCounter++;
}

void
GOTO (int address, int operand)
{
  for (int i = 0; i < basicCommandCouner; i++)
    {
      if (program[i].Number == operand)
        {
          fprintf (output, "%.2i JUMP %d\n", address, program[i].Address);
          return;
        }
    }
  fprintf (stderr, "Reference to an inspecifed memory location.\n");
  exit (EXIT_FAILURE);
}

void
IF (char *arguments)
{
  int mySign = -1;

  int before = 0;

  int after = 0;

  for (int i = 0; i < strlen (arguments); i++)
    {
      if ((arguments[i] == '>') || (arguments[i] == '<')
          || (arguments[i] == '='))
        {
          mySign = i;

          if (!(arguments[i - 1] == ' '))
            {
              before = 1;
            }

          if (!(arguments[i + 1] == ' '))
            {
              after = 1;
            }
          break;
        }
    }

  char *expression = (char *)malloc (sizeof (char) * 255);
  if (!(before) && !(after))
    {
      expression = strtok (arguments, "");
    }
  else
    {
      int j = 0;
      for (int i = 0; i < strlen (arguments); i++)
        {
          if (i == mySign)
            {
              if (before)
                {
                  expression[j] = ' ';
                  j++;
                }
              expression[j] = arguments[i];
              j++;
              if (after)
                {
                  expression[j] = ' ';
                  j++;
                }
            }
          else
            {
              expression[j] = arguments[i];
              j++;
            }
        }
      expression[j] = '\0';
    }

  char *ptr = strtok (expression, " ");
  char *operand1 = ptr;
  char operand1Name;

  if (strlen (operand1) > 1)
    {
      if (atoi (operand1))
        {
          operand1Name = intToConstant (atoi (operand1));
        }
    }
  else
    {
      if ((operand1[0] >= '0') && (operand1[0] <= '9'))
        {
          operand1Name = intToConstant (atoi (operand1));
        }
      else if ((operand1[0] >= 'A') && (operand1[0] <= 'Z'))
        {
          operand1Name = operand1[0];
        }
      else
        {
          fprintf (stderr, "Wrong statement!\n");
          exit (EXIT_FAILURE);
        }
    }

  ptr = strtok (NULL, " ");
  char *logicalSign = ptr;

  ptr = strtok (NULL, " ");
  char *operand2 = ptr;
  char operand2Name;

  if (strlen (operand2) > 1)
    {
      if (atoi (operand2))
        {
          operand2Name = intToConstant (atoi (operand2));
        }
    }
  else
    {
      if ((operand2[0] >= '0') && (operand2[0] <= '9'))
        {
          operand2Name = intToConstant (atoi (operand2));
        }
      else if ((operand2[0] >= 'A') && (operand2[0] <= 'Z'))
        {
          operand2Name = operand2[0];
        }
      else
        {
          fprintf (stderr, "Wrong statement!\n");
          exit (EXIT_FAILURE);
        }
    }

  int falsePosition = -1;

  if (logicalSign[0] == '<')
    {
      fprintf (output, "%.2i LOAD %d\n", assemblerCommandCounter,
               getVariableAddress (operand1Name));
      assemblerCommandCounter++;
      fprintf (output, "%.2i SUB %d\n", assemblerCommandCounter,
               getVariableAddress (operand2Name));
      assemblerCommandCounter++;
      fprintf (output, "%.2i JNEG %d\n", assemblerCommandCounter,
               assemblerCommandCounter + 2);
      assemblerCommandCounter++;
      falsePosition = assemblerCommandCounter;
      assemblerCommandCounter++;
    }
  else if (logicalSign[0] == '>')
    {
      fprintf (output, "%.2i LOAD %d\n", assemblerCommandCounter,
               getVariableAddress (operand2Name));
      assemblerCommandCounter++;
      fprintf (output, "%.2i SUB %d\n", assemblerCommandCounter,
               getVariableAddress (operand1Name));
      assemblerCommandCounter++;
      fprintf (output, "%.2i JNEG %d\n", assemblerCommandCounter,
               assemblerCommandCounter + 2);
      assemblerCommandCounter++;
      falsePosition = assemblerCommandCounter;
      assemblerCommandCounter++;
    }
  else if (logicalSign[0] == '=')
    {
      fprintf (output, "%.2i LOAD %d\n", assemblerCommandCounter,
               getVariableAddress (operand1Name));
      assemblerCommandCounter++;
      fprintf (output, "%.2i SUB %d\n", assemblerCommandCounter,
               getVariableAddress (operand2Name));
      assemblerCommandCounter++;
      fprintf (output, "%.2i JZ %d\n", assemblerCommandCounter,
               assemblerCommandCounter + 2);
      assemblerCommandCounter++;
      falsePosition = assemblerCommandCounter;
      assemblerCommandCounter++;
    }

  ptr = strtok (NULL, " ");
  char *command = ptr;
  ptr = strtok (NULL, "");
  char *commandArguments = ptr;

  if (strcmp (command, "IF") == 0)
    {
      fprintf (stderr, "Multiple IF!\n");
      exit (EXIT_FAILURE);
    }
  else if (strcmp (command, "GOTO") != 0)
    {
      function (command, commandArguments);
    }
  else
    {
      gotoCounter++;
      gotoRecords
          = realloc (gotoRecords, sizeof (struct command) * gotoCounter + 1);
      struct command gotoCommand;
      gotoCommand.Address = assemblerCommandCounter;
      char *buff = (char *)malloc (sizeof (char) * 255);
      sprintf (buff, "%d GOTO %s", falsePosition, commandArguments);
      gotoCommand.Command = buff;
      gotoRecords[gotoCounter] = gotoCommand;
      assemblerCommandCounter++;
    }

  fprintf (output, "%.2i JUMP %d\n", falsePosition, assemblerCommandCounter);
  // assemblerCommandCounter++;
}

void
LET (char *arguments)
{
  char fin[255];
  char var;
  var = preCalcProcessing (arguments);
  translateToRPN (arguments, fin);
  // printf("%c\n", var);
  parsRPN (fin, var);
}

void
END ()
{
  fprintf (output, "%.2i HALT 00\n", assemblerCommandCounter);
  assemblerCommandCounter++;
}

void
function (char *command, char *arguments)
{
  if (strcmp (command, "REM") == 0)
    {
    }
  else if (strcmp (command, "INPUT") == 0)
    {
      INPUT (arguments);
    }
  else if (strcmp (command, "PRINT") == 0)
    {
      PRINT (arguments);
    }
  else if (strcmp (command, "IF") == 0)
    {
      IF (arguments);
    }
  else if (strcmp (command, "LET") == 0)
    {
      LET (arguments);
    }
  else if (strcmp (command, "END") == 0)
    {
      END ();
    }
  else
    {
      fprintf (stderr, "%s is not a command.\n", command);
    }
}

int
main (int argc, const char **argv)
{
  if (argc < 3)
    {
      fprintf (stderr, "Usage: %s input.sa output.o\n", argv[0]);
      exit (EXIT_FAILURE);
    }

  loadFile (argv[1], "tmp.sa");

  translation ();
  fclose (input);
  fclose (output);

  if (variableCount + assemblerCommandCounter > 100)
    {
      fprintf (stderr, "RAM overflow error!\n");
      system ("rm -rf tmp.sa");
    }

  char sat[255];
  sprintf (sat, "./sat tmp.sa %s\n", argv[2]);
  system (sat);

  if (argc >= 4)
    {
      if (argv[3][0] == '1')
        {
          return 0;
        }
    }
  // system("rm -rf tmp.sa");
  return 0;
}