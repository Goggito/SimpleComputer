#include "../include/big.h"

int Big_C[18][2] = { { 0x4242423C, 0x3C424242 },   // 0
                     { 0x48506040, 0x40404040 },   // 1
                     { 0x2042423C, 0x7E020418 },   // 2
                     { 0x3840423C, 0x3C424040 },   // 3
                     { 0x7E424478, 0x40404040 },   // 4
                     { 0x3E02027E, 0x3C424040 },   // 5
                     { 0x3E02423C, 0x3C424242 },   // 6
                     { 0x2040407E, 0x10101010 },   // 7
                     { 0x3C42423C, 0x3C424242 },   // 8
                     { 0x7C42423C, 0x3C424040 },   // 9
                     { 0x66663C18, 0x66667E7E },   // A
                     { 0x3E66663E, 0x3E666666 },   // B
                     { 0x0202423C, 0x3C420202 },   // C
                     { 0x4444443E, 0x3E444444 },   // D
                     { 0x3E02027E, 0x7E020202 },   // E
                     { 0x1E02027E, 0x02020202 },   // F
                     { 0x7E181800, 0x0018187E },   // +
                     { 0x7E000000, 0x00000000 } }; // -

void
BigCellPrint (int x, int y, int address)
{
  int value, command, operand;
  int A[2];

  sc_memoryGet (address, &value);
  sc_commandDecode (value, &command, &operand);

  int max_hex_digits = sizeof (int) * 2;
  char hex_buffer[max_hex_digits + 1];
  int numb = 0;

  if (value & 0x4000)
    {
      numb = 17; //-
    }
  else
    {
      numb = 16; //+
    }

  A[0] = Big_C[numb][0];
  A[1] = Big_C[numb][1];
  bc_printbigchar (A, x + 1, y, RESET, RESET);

  sprintf (hex_buffer, "%02X%02X", command, operand);
  for (int i = 0; i < max_hex_digits; i++)
    {
      char hex_digit = hex_buffer[i];
      if (hex_digit == '\0')
        {
          break;
        }
      if (hex_digit <= 57)
        numb = hex_digit - 48; // 1-9
      else
        numb = hex_digit - 55; // A-F

      A[0] = Big_C[numb][0];
      A[1] = Big_C[numb][1];
      bc_printbigchar (A, x += 9, y, RESET, RESET);
    }
}