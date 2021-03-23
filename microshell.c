#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

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
    /* for (int i = 0; i < c->argc; i++) */
    /*     free(c->argv[i]); */
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

void run_list(command *c)
{
	while (c)
	{
		for (int i = 0; i < c->argc; i++)
			printf("argv: %s\n", c->argv[i]);
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
	return 0;
}

