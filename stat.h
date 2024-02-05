#ifndef __STAT_H
#define __STAT_H

#include <math.h>

typedef float stat_t;
typedef float long_stat_t;

//#define NAN nanf()

long_stat_t sum(volatile signed short int* data, int length);
stat_t mean(volatile signed short int* data, int length);
stat_t sd(volatile signed short int* data, stat_t mu, int length);

#endif
