#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

#define OP_PIPE 2
#define OP_SEQUENTIAL 3

int exit_status;

void report(const char *fmt, va_list params)
{
	vfprintf(stderr, fmt, params);
}

void die(const char *fmt, ...)
{
	va_list params;

	va_start(params, fmt);
	report(fmt, params);
	va_end(params);
	exit(exit_status);
}

typedef struct command command;
struct command
{
	struct command *next;
	int argc;
	char **argv;
	int op;
	pid_t pid;
};

static command *command_alloc()
{
	struct command *c;

	c = malloc(sizeof(command));
	if (!c)
		return NULL;
	c->next = NULL;
	c->argc = 0;
	c->argv = NULL;
	c->op = 0;
	c->pid = -1;
	return c;
}

static void command_append_arg(command *c, char *word) {
    c->argv = (char**)realloc(c->argv, sizeof(char*) * (c->argc + 2));
    c->argv[c->argc] = word;
    c->argv[c->argc + 1] = NULL;
    c->argc++;
}

static void command_free(command *c)
{
    free(c->argv);
    free(c);
}

static void command_clear(command **c)
{
	command *tmp;

	if (c == NULL)
		return ;
	while (*c)
	{
		tmp = (*c)->next;
		command_free(*c);
		*c = tmp;
	}
}

static pid_t start_command(char *arg[], int ispipe, int haspipe, int lastpipe[2])
{
	pid_t pid;
	int newpipe[2];

	// if pipe, pipe
	// fork
	// execute command
	// cleanup pipe
	// setup next pipe

	if (ispipe)
		pipe(newpipe);
	pid = fork();
	if (pid < 0)
		die("failed to fork: %s\n", strerror(errno));
	else if (pid == 0)
	{
		// child

	}
	// parent

	return pid;
}

static command *do_pipeline(command *c)
{
	int ispipe;
	int haspipe;
	int lastpipe[2];

	ispipe = 0;
	haspipe = 0;
	lastpipe[0] = -1;
	lastpipe[1] = -1;
	while (c)
	{
		ispipe = (c->op == OP_PIPE);
		c->pid = start_command(c->argv, ispipe, haspipe, lastpipe);
		haspipe = ispipe;
		if (haspipe && c->next)
			c = c->next;
		else if (haspipe && !c->next)
			die("multiline pipeline is not implemented\n");
		else
			break ;
	}
	return NULL;
}

void run_list(command *c)
{
	pid_t pid;
	int status;

	while (c)
	{
		for (int i = 0; i < c->argc; i++)
			printf("argv: %s\n", c->argv[i]);

		// Check for builtin

		// Do pipeline
		c = do_pipeline(c);

		// Wait pipeline
		pid = waitpid(c->pid, &status, 0);
		assert(pid == c->pid);
		if (WIFEXITED(status))
			exit_status = WEXITSTATUS(status);
		else
			die("child %d exited abnormally", pid);

		// Reap zombie processes
		while (wait(NULL) > 0);

		c = c->next;
	}
}

int main(int argc, char *argv[])
{
	command *c;

	// Command allocation
	c = command_alloc();
	if (!c)
		return 1;

	// Parse
	for (int i = 1; i < argc; i++)
		command_append_arg(c, argv[i]);

	// Run list
	if (c->argc)
		run_list(c);

	// Command clear
	command_clear(&c);
	return exit_status;
}

