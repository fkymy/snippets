#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

int main(int argc, char *argv[])
{
	int use_stdio = 1;
	if (argc > 1)
	{
		assert(strcmp(argv[1], "-s") == 0 || strcmp(argv[1], "-y") == 0);
		use_stdio = strcmp(argv[1], "-s") == 0;
	}

	pid_t pid = fork();
	assert(pid >= 0);

	const char *identity;
	if (pid == 0)
		identity = "new process\n";
	else
		identity = "old process\n";

	if (use_stdio)
	{
		for (int i = 0; i < 1000000; i++)
		{
			printf("%s", identity);

			/* fputs(identity, stdout); */

			/* for (const char *s = identity; *s; s++) */
			/* { */
			/* 	fputc(*s, stdout); */
			/* } */
		}
	}
	else
	{
		for (int i = 0; i < 1000000; i++)
		{
			ssize_t nw = write(1, identity, strlen(identity));
			assert(nw == (ssize_t)strlen(identity));
		}
	}
	return 0;
}
