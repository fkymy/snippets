#ifndef HASH_H
# define HASH_H

#define BASE (256)
#define MULTIPLIER (37)
#define BITS_PER_CHAR (8)
#define MAX_STRING_SIZE (128)
#define MAX_BITS (BITS_PER_CHAR * MAX_STRING_SIZE)

unsigned long modhash(const char *s, unsigned long m);
unsigned long mulhash(const char *s);
unsigned long xorhash(const char *s, unsigned long x[]);
unsigned long sdbm(const char *s);
unsigned long djb2(const char *s);

#endif
