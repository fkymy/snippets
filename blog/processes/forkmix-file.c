#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

int main(int argc, char *argv[])
{
	FILE *f = fopen(argv[1], "w");

	pid_t pid = fork();
	assert(pid >= 0);

	const char *identity;
	if (pid == 0)
		identity = "new process\n";
	else
		identity = "old process\n";

	for (int i = 0; i < 1000000; i++)
	{
		fputs(identity, f);
	}
	return 0;
}
