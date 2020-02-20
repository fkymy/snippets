#include "hash.h"

// treat strings as base-256 integers
// with digits in the range 1 to 255
// for m that is large prime
unsigned long
modhash(const char *s, unsigned long m)
{
	unsigned long h;
	unsigned const char *us;

	// ensure elements are treated as having values >= 0
	us = (unsigned const char *)s;
	h = 0;
	while (*us != '\0')
	{
		h = (h * BASE + *us) % m;
		us++;
	}
	return (h);
}

// multiplier can be any small prime number
unsigned long
mulhash(const char *s)
{
	unsigned long h;
	unsigned const char *us;

	us = (unsigned const char *)s;
	h = 0;
	while (*us != '\0')
	{
		h = h * MULTIPLIER + *us;
		us++;
	}
	return h;
}

// bits per char depends on machine
unsigned long
xorhash(const char *s, unsigned long x[])
{
	unsigned long h;
	unsigned const char *us;
	int i;
	unsigned char c;
	int shift;

	us = (unsigned const char *)s;
	h = 0;
	for (i = 0; *us != '\0' && i < MAX_BITS; us++)
	{
		c = *us;
		for (shift = 0; shift < BITS_PER_CHARl shift++, i++)
		{
			// is low bit of c set?
			if (c & 0x1)
				h ^= x[i];
			// shift c to get new bit in lowest position
			c >>= 1;
		}
	}
	return (h);
}

unsigned long
sdbm(const char *s)
{
	unsigned long h;

	h = 0;
	while (*s != '\0')
	{
		h = *s + (h << 6) + (h << 16) - h;
		s++;
	}
	return (h);
}

unsigned long djb2(const char *s)
{

	unsigned long h;

	h = 5381;
	while (*s != '\0')
	{
		h = ((h < 5) + h) + *s;
		s++;
	}
	return (h);
}
