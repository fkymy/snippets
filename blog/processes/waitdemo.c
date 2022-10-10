#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main() {
	pid_t pid = fork();
	assert(pid >= 0);
	if (pid == 0) {
		usleep(2000 * 1000);
		fprintf(stderr, "goodbye from child pid %d\n", getpid());
		exit(0);
	}

	int status;
	pid_t exited_pid = waitpid(pid, &status, 0);
	assert(exited_pid == pid);

	if (WIFEXITED(status))
		fprintf(stderr, "Child exited with status %d\n", WEXITSTATUS(status));
	else
		fprintf(stderr, "Child exited abnormally [%x]\n", status);
	return 0;
}
