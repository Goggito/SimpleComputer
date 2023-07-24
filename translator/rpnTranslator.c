#include "rpnTranslator.h"

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
