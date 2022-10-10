#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

int main(void)
{
	pid_t pid = fork();
	assert(pid >= 0);

	const char *who;
	if (pid == 0)
		who = "new process\n";
	else
		who = "old process\n";

	for (int i = 0; i < 1000000; i++)
	{
		printf("%s", who);

		// or other stdio:

		// fputs(who, stdout);

		// for (const char *s = who; *s; s++)
		// 	fputc(*s, stdout);
	}
	return 0;
}
