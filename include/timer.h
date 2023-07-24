#include <signal.h>
#include <stdio.h>
#include <sys/time.h>

#include "../lib/mySimpleComputer/mySimpleComputer.h"

#ifndef TIMER_H
#define TIMER_H

int HandlerSIGALRM (int signal);
int TimerInit (int sec);
void TimerStart ();
void TimerStop ();

#endif