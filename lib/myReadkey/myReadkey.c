#include "myReadkey.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <termios.h>
#include <unistd.h>

#define MAX_KEY_BUFFER_SIZE 6

static struct termios conserved = { 0 };

const char *codes[]
    = { "\E[A",   "\E[B",   "\E[D",   "\E[C",   "\E[2~",  "\E[3~",  "\E[5~",
        "\E[6~",  "\EOP",   "\EOQ",   "\EOR",   "\EOS",   "\E[15~", "\E[17~",
        "\E[18~", "\E[19~", "\E[20~", "\E[21~", "\E[23~", "\E[24~" };

int
rk_readkey (enum keys *key)
{
  char key_buffer[MAX_KEY_BUFFER_SIZE] = "";
  if (!read (STDIN_FILENO, key_buffer, sizeof (key_buffer)))
    {
      return -1;
    }

  *key = KEY_UNDEFINED;

  for (enum keys iterator = KEY_UP; iterator < KEY_LAST; iterator++)
    {
      if (!strncmp (key_buffer, codes[iterator], strlen (codes[iterator])))
        *key = iterator;
    }

  // Проверяем клавиши букв
  if (*key == KEY_UNDEFINED)
    {
      if (key_buffer[0] >= 'a' && key_buffer[0] <= 'z')
        {
          *key = KEY_A + key_buffer[0]
                 - 'a'; // преобразование ASCII кода в номер ключа
        }
      else if (key_buffer[0] >= 'A' && key_buffer[0] <= 'Z')
        {
          *key = KEY_A + key_buffer[0]
                 - 'A'; // преобразование ASCII кода в номер ключа
        }
    }

  return 0;
}

int
rk_mytermsave (void)
{
  tcgetattr (STDIN_FILENO, &conserved);
  return 0;
}

int
rk_mytermrestore (void)
{
  if (tcsetattr (STDIN_FILENO, TCSANOW, &conserved) < 0)
    return 1;
  return 0;
}

int
rk_mytermregime (int regime, int vtime, int vmin, int echo, int sigint)
{
  struct termios term;
  int status = tcgetattr (STDIN_FILENO,
                          &term); // получаем текущие настройки терминала

  if (status == -1)
    {
      return -1; // ошибка при получении настроек
    }

  if (regime == 1)
    {                         // канонический режим
      term.c_lflag |= ICANON; // включаем канонический режим
      term.c_cc[VTIME] = 0; // время ожидания ввода
      term.c_cc[VMIN] = 1; // минимальное количество символов для чтения
    }
  else
    {                          // неканонический режим
      term.c_lflag &= ~ICANON; // выключаем канонический режим
      term.c_cc[VTIME] = vtime; // время ожидания ввода
      term.c_cc[VMIN] = vmin; // минимальное количество символов для чтения
    }

  if (echo == 1)
    {
      term.c_lflag |= ECHO; // включаем отображение вводимых символов
    }
  else
    {
      term.c_lflag &= ~ECHO; // выключаем отображение вводимых символов
    }

  if (sigint == 1)
    {
      term.c_lflag |= ISIG; // включаем обработку сигнала прерывания
    }
  else
    {
      term.c_lflag &= ~ISIG; // выключаем обработку сигнала прерывания
    }

  status = tcsetattr (STDIN_FILENO, TCSANOW,
                      &term); // устанавливаем новые настройки терминала

  if (status == -1)
    {
      return -1; // ошибка при установке настроек
    }

  return 0; // успешное выполнение функции
}