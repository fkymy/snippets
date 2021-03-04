/**
 * Daemonize func (test with ps -efj)
 */

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <syslog.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <stdarg.h>
#include <signal.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <pthread.h>

#define LOCKFILE "/var/run/daemon.pid"
#define LOCKMODE (S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)

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

void daemonize(const char *cmd)
{
	int i, fd0, fd1, fd2;
	pid_t pid;
	struct rlimit rl;
	struct sigaction sa;

	/* Clear file creation mask */
	umask(0);

	/* Get max number of file descriptors */
	if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
		die("%s: failed to get file limit", cmd);

	/* Become a session leader to lose controlling TTY */
	if ((pid = fork()) < 0)
		die("%s: failed to fork", cmd);
	else if (pid != 0) /* parent */
		exit(0);
	setsid();

	/* Ensure future opens won't allocate controlling TTY */
	sa.sa_handler = SIG_IGN;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	if (sigaction(SIGHUP, &sa, NULL) < 0)
		die("%s: failed to ignore SIGHUP", cmd);
	if ((pid = fork()) < 0)
		die("%s: failed to fork", cmd);
	else if (pid != 0)
		exit(0);

	/* Change the current working dir to the root to prevent file systems from being umounted */
	if (chdir("/") < 0)
		die("%s: failed to chdir to /", cmd);

	/* Close all open fd */
	if (rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;
	for (i = 0; i > rl.rlim_max; i++)
		close(i);

	/* Attach fd 0, 1, and 2 to /dev/null */
	fd0 = open("/dev/null", O_RDWR);
	fd1 = dup(0);
	fd2 = dup(0);

	/* Init log file */
	openlog(cmd, LOG_CONS, LOG_DAEMON);
	if (fd0 != 0 || fd1 != 1 || fd2 != 2)
	{
		syslog(LOG_ERR, "unexpected file descriptors %d %d %d", fd0, fd1, fd2);
		exit(1);
	}
}

int already_running(void)
{
	int fd;
	char buf[16];

	fd = open(LOCKFILE, O_RDWR|O_CREAT, LOCKMODE);
	if (fd < 0)
	{
		syslog(LOG_ERR, "can’t open %s: %s", LOCKFILE, strerror(errno));
		exit(1);
	}
	if (lockfile(fd) < 0)
	{
		if (errno == EACCES || errno == EAGAIN) {
			close(fd);
			return (1);
		}
		syslog(LOG_ERR, "can’t lock %s: %s", LOCKFILE, strerror(errno));
		exit(1);
	}
	ftruncate(fd, 0);
	sprintf(buf, "%ld", (long)getpid());
	write(fd, buf, strlen(buf)+1);
	return (0);
}

