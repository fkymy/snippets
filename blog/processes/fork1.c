#include <stdio.h>
#include <unistd.h>
#include <assert.h>

int main(void)
{
	pid_t pid = fork();
	assert(pid >= 0);

	const char *who = (pid == 0 ? "child" : "parent");
	printf("Hello from %s, with pid %d\n", who, getpid());
	return 0;
}
