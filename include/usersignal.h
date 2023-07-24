#include <signal.h>

#include "../lib/mySimpleComputer/mySimpleComputer.h"

#ifndef SIGNAL_H
#define SIGNAL_H

int HandlerSIGUSR1 (int signal);
int UserSignalInit (void);

#endif