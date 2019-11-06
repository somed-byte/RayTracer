#ifndef DRAND48_HPP
#define DRAND48_HPP

#include <stdlib.h>

#define mm 0x100000000LL
#define cc 0xB16
#define aa 0x5DEECE66DLL

static unsigned long long seed = 1;

double drand48(void);

void srand48(unsigned int i);

#endif