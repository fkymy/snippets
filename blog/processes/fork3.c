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
		ssize_t nw = write(1, who, strlen(who));
		assert(nw == (ssize_t)strlen(who));
	}
	return 0;
}
