#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

int main(int argc, char *argv[])
{
	FILE *f = fopen(argv[1], "w");

	pid_t pid = fork();
	assert(pid >= 0);

	// Try moving it here:
	// FILE *f = fopen(argv[1], "w");

	const char *who;
	if (pid == 0)
		who = "new process\n";
	else
		who = "old process\n";

	for (int i = 0; i < 1000000; i++)
	{
		fputs(who, f);
	}
	return 0;
}
