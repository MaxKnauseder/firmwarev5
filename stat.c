#include "stat.h"
#include <math.h>

long_stat_t sum(volatile signed short int* data, int length)
{
    long_stat_t result = 0;
    unsigned int i;
    for (i = 0; i < length; i++) {
        result += data[i];
    }
    return result;
}

stat_t mean(volatile signed short int* data, int length)
{
    long_stat_t s = sum(data, length);
    volatile stat_t m = (s) / length;
    return m;
}

unsigned int sqrti(unsigned long int x)
{
    unsigned int bit = 16;
    unsigned int mask = 0x8000;
    unsigned int root = 0x0000;

    do {
        unsigned long int acc = root | mask;
        if (acc * acc <= x)
            root |= mask;
        mask >>= 1;
    } while (--bit);

    return root;
}

stat_t sd(volatile signed short int* data, stat_t mu, int length)
{
    stat_t var = 0;
    int i;
    for (i = 0; i < length; i++) {
        var += (data[i] - mu) * (data[i] - mu);
    }
    return sqrtf(var / (length - 1));
}
