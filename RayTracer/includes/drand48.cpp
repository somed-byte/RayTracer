#include "drand48.hpp"

double drand48(void)
{
	seed = (aa * seed + cc) & 0xFFFFFFFFFFFFLL;
	unsigned int x = seed >> 16;
	return 	((double)x / (double)mm);

}

void srand48(unsigned int i)
{
	seed = (((long long int)i) << 16) | rand();
}