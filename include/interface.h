#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "../lib/myBigChars/myBigChars.h"
#include "../lib/mySimpleComputer/mySimpleComputer.h"
#include "../lib/myTerm/myTerm.h"

#ifndef INTERFACE_H
#define INTERFACE_H

extern int cell_addr;

void GoInput ();
void GoOutput ();
void FlagPrint ();
void CounterPrint ();
void OperationPrint (int address);
void UIUpdate ();
void UIOutput ();

#endif