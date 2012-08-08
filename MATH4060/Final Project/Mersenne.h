#ifndef __Mersenne_H
#define __Mersenne_H

namespace Mersenne
{
	// Seed for random number generator
	void SetRandomSeed(long seed);

	// Random integer
	unsigned long RandomInt(void);

	// Random double
	double RandomDouble(void);
}

#endif
