#include <stdio.h>
#include <assert.h>
#include <unistd.h>

int main(void)
{
	pid_t p1 = fork();
	assert(p1 >= 0);

	pid_t p2 = fork();
	assert(p2 >= 0);

	printf("Hello from pid %d\n", getpid());
}
