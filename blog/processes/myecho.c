#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	fprintf(stderr, "myecho running in pid %d\n", getpid());
	for (int i = 0; i < argc; i++)
		fprintf(stderr, "arg %d: \"%s\"\n", i, argv[i]);
}
