#include <stdio.h>
#include <limits.h>

int
chkadd(int a, int b)
{
	if(b < 0)
		return chksub(a, -b);
	if(INT_MAX - b < a) {
		fputs("int overflow\n", stderr);
		return INT_MAX;
	}
	return a + b;
}

int
chksub(int a, int b)
{
	if(b < 0)
		return chkadd(a, -b);
	if(INT_MIN + b > a) {
		fputs("int underflow\n", stderr);
		return INT_MIN;
	}
	return a - b;
}

int
chkmul(int a, int b)
{
	int sign = 1;
	if(a == 0 || b == 0) return 0;
	if(a < 0) { a = -a; sign = -sign; }
	if(b < 0) { b = -b; sign = -sign; }
	if(INT_MAX / b < a) {
		fputs("int overflow\n", stderr);
		return (sign > 0) ? INT_MAX : INT_MIN;
	}
	return sign * a * b;
}
