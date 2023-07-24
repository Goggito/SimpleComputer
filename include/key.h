#include <signal.h>
#include <stdio.h>

#include "../lib/myReadkey/myReadkey.h"
#include "../lib/mySimpleComputer/mySimpleComputer.h"
#include "../lib/myTerm/myTerm.h"

#ifndef KEY_H
#define KEY_H

void KeyRead (int *address, int *exit_cheack);
void KeyReadReset ();

#endif