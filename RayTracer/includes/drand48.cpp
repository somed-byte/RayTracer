#include "drand48.hpp"

double wdrand48(void)
{
	seed = (aa * seed + cc) & 0xFFFFFFFFFFFFLL;
	unsigned int x = seed >> 16;
	return 	((double)x / (double)mm);

}

void wsrand48(unsigned int i)
{
	seed = (((long long int)i) << 16) | rand();
}