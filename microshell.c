#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <stdarg.h>
#include <errno.h>
#include <string.h>

#define TOKEN_WORD 0
#define TOKEN_REDIRECTION 1
#define TOKEN_PIPE 2
#define TOKEN_AND 3
#define TOKEN_OR 4
#define TOKEN_SEPARATOR 5
#define TOKEN_OTHER -1

#define OP_PIPE 2
#define OP_AND 3
#define OP_OR 4
#define OP_SEPARATOR 5
#define OP_OTHER -1

extern char **environ;

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
	exit(1);
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

command *command_alloc()
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

void command_append_arg(command *c, char *word) {
	c->argv = (char**)realloc(c->argv, sizeof(char*) * (c->argc + 2));
	c->argv[c->argc] = word;
	c->argv[c->argc + 1] = NULL;
	c->argc++;
}

void command_free(command *c)
{
    free(c->argv);
    free(c);
}

void command_clear(command **c)
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

pid_t start_command(char *argv[], int ispipe, int haspipe, int lastpipe[2])
{
	pid_t pid;
	int newpipe[2];

	if (ispipe)
	{
		int r = pipe(newpipe);
		assert(r == 0);
	}
	pid = fork();
	if (pid == 0)
	{
		if (haspipe)
		{
			close(lastpipe[1]);
			dup2(lastpipe[0], 0);
			close(lastpipe[0]);
		}
		if (ispipe)
		{
			close(newpipe[0]);
			dup2(newpipe[1], 1);
			close(newpipe[1]);
		}
		execve(argv[0], argv, environ);
		_exit(1);
	}
	if (haspipe)
	{
		close(lastpipe[0]);
		close(lastpipe[1]);
	}
	if (ispipe)
	{
		lastpipe[0] = newpipe[0];
		lastpipe[1] = newpipe[1];
	}
	return pid;
}

command *do_pipeline(command *c)
{
	int ispipe = 0;
	int haspipe = 0;
	int lastpipe[2] = { -1, -1 };

	while (c)
	{
		ispipe = (c->op == '|');
		c->pid = start_command(c->argv, ispipe, haspipe, lastpipe);
		if (c->pid == -1)
			break ;
		haspipe = ispipe;
		if (haspipe && c->next)
			c = c->next;
		else if (haspipe && !c->next)
			die("multiline pipeline is not implemented\n");
		else
			break ;
	}
	return c;
}

void run_list(command *c)
{
	pid_t pid;

	while (c)
	{
		c = do_pipeline(c);
		if (c->pid != -1)
		{
			pid = waitpid(c->pid, NULL, 0);
			assert(pid == c->pid);
		}

		// Reap zombie processes
		while (wait(NULL) > 0);

		c = c->next;
	}
}

void eval(char *argv[], command *c)
{
	command *new;

	for (int i = 0; argv[i]; i++)
	{
		if (strcmp(argv[i], "|") == 0 || strcmp(argv[i], ";") == 0)
		{
			if (c->op || !c->argc)
				die("syntax error\n");
			c->op = argv[i][0];
		}
		else
		{
			if (c->op)
			{
				new = command_alloc();
				if (!new)
					die("failed to allocate command");
				c->next = new;
				c = new;
			}
			command_append_arg(c, argv[i]);
		}
	}
}

int main(int argc, char *argv[])
{
	command *c;

	c = command_alloc();
	if (!c)
		return 1;

	eval(argv + 1, c);

	if (c->argc)
		run_list(c);

	command_clear(&c);
	return 0;
}
