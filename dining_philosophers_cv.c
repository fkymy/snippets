#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

typedef struct	s_params
{
	int id;
	pthread_mutex_t *left;
	pthread_mutex_t *right;
	int *permits;
	pthread_cond_t *cv;
	pthread_mutex_t *m;
}				t_params;

static const int kNumPhilosophers = 5;
static const int kNumForks = kNumPhilosophers;
static const int kNumMeals = 3;

static void wait_for_permission(int *permits, pthread_cond_t *cv, pthread_mutex_t *m) {
	pthread_mutex_lock(m);
	while (permits == 0)
		pthread_cond_wait(cv, m);
	(*permits)--;
	pthread_mutex_unlock(m);
}

static void grant_permission(int *permits, pthread_cond_t *cv, pthread_mutex_t *m) {
	pthread_mutex_lock(m);
	(*permits)++;
	if (*permits == 1)
		pthread_cond_broadcast(cv);
	pthread_mutex_unlock(m);
}

static void think(int id) {
	printf("%d is thinking\n", id);
	sleep(1);
}

static void eat(int id, pthread_mutex_t *left, pthread_mutex_t *right, int *permits, pthread_cond_t *cv, pthread_mutex_t *m) {
	wait_for_permission(permits, cv, m);
	pthread_mutex_lock(left);
	pthread_mutex_lock(right);
	printf("%d is eating\n", id);
	sleep(1);
	grant_permission(permits, cv, m);
	pthread_mutex_unlock(left);
	pthread_mutex_unlock(right);
}

static void *philosopher(void *args) {
	t_params *p;

	p = (t_params *)args;
	for (int i = 0; i < kNumMeals; i++) {
		think(p->id);
		eat(p->id, p->left, p->right, p->permits, p->cv, p->m);
	}
	return NULL;
}

int main(int argc, char *argv[]) {
	pthread_mutex_t forks[kNumPhilosophers], m;
	pthread_cond_t cv;
	pthread_t philosophers[kNumPhilosophers];
	t_params params[kNumPhilosophers];

	int permits = kNumPhilosophers - 1;

	for (int i = 0; i < kNumPhilosophers; i++) {
		pthread_mutex_init(&forks[i], NULL);
		params[i].id = i;
		params[i].left = &forks[i];
		params[i].right = &forks[(i + 1) % kNumPhilosophers];
		params[i].permits = &permits;
		params[i].cv = &cv;
		params[i].m = &m;
		pthread_create(&philosophers[i], NULL, philosopher, (void *)(params + i));
	}
	for (int j = 0; j < kNumPhilosophers; j++) {
		pthread_join(philosophers[j], NULL);
	}
	return 0;
}

