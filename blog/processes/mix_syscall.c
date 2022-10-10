#include <unistd.h>
#include <assert.h>
#include <string.h>

int main(void)
{
	pid_t pid = fork();
	assert(pid >= 0);

	const char *text;
	if (pid == 0)
		text = "new process\n";
	else
		text = "old process\n";

	for (int i = 0; i < 1000000; i++)
	{
		ssize_t r = write(1, text, strlen(text));
		assert(r == (ssize_t)strlen(text));
	}
	return 0;
}
