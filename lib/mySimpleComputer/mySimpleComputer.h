#pragma once

#ifndef __mySimpleComputer__
#define __mySimpleComputer__

#define OVRF 0b00000001
#define NULD 0b00000010
#define MOUT 0b00000100
#define UNIF 0b00001000 // ignor
#define INVL 0b00010000
#define WRIT 0b00100000 // write_check

#define set_flag(mask, flag) flag |= mask
#define reset_flag(mask, flag) flag &= ~(mask)

int sc_memoryInit ();

int sc_memorySet (int address, int value);

int sc_memoryGet (int address, int *value);

int sc_memorySave (char *filename);

int sc_memoryLoad (char *filename);

int sc_regInit (void);

int sc_regSet (int registr, int value);

int sc_regGet (int registr, int *value);

int sc_accSet (int value);

int sc_accGet (int *value);

void sc_counterInit ();

void sc_counterForward ();

void sc_counterSet (int value);

void sc_counterGet (int *value);

int sc_commandEncode (int command, int operand, int *value);

int sc_commandDecode (int value, int *command, int *operand);

#endif