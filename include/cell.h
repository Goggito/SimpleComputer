#include <stdio.h>

#include "../lib/myReadkey/myReadkey.h"
#include "../lib/mySimpleComputer/mySimpleComputer.h"
#include "../lib/myTerm/myTerm.h"

#ifndef CELL_H
#define CELL_H

void CellPrint (int address);
void CellSelect (int address);
void CellEnter (int address);
void CellEnterVar (int address);

#endif