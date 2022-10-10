#include <stdio.h>
#include <unistd.h>

int main(void)
{
	const char *args[] = {
		"./myecho",
		"hello",
		"world",
		NULL
	};

	pid_t pid = fork();

	if (pid == 0)
	{
		fprintf(stderr, "executing myecho program from pid %d\n", getpid());

		int r = execvp("./myecho", (char **)args);
		fprintf(stderr, "finished executing myecho from pid %d, with status %d\n", getpid(), r);
	}
	else
	{
		fprintf(stderr, "child pid %d should execute myecho\n", pid);
	}
}
