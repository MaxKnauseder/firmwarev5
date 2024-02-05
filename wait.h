#ifndef __WAIT_H
#define __WAIT_H
#include <msp430x16x.h>
#include <io.h>
#include <iostructures.h>
#include <signal.h>
#include "globals.h"
#include "pAM-definitions.h"
#include "v5_main.h"

void clock_setup(void);
void start_timer(unsigned int cycles);
void wait_for_timer();
void wait(unsigned int cycles);
void schedule(void (*func)(void), unsigned int cycles);

#endif
