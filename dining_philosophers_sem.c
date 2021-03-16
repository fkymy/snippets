#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <semaphore.h>

typedef struct	s_params
{
	int id;
	pthread_mutex_t *left;
	pthread_mutex_t *right;
	sem_t *permits;
}				t_params;

static const int kNumPhilosophers = 5;
static const int kNumForks = kNumPhilosophers;
static const int kNumMeals = 3;

static void think(int id) {
	printf("%d is thinking\n", id);
	sleep(1);
}

static void eat(int id, pthread_mutex_t *left, pthread_mutex_t *right, sem_t *permits) {
	sem_wait(permits);
	pthread_mutex_lock(left);
	pthread_mutex_lock(right);
	printf("%d is eating\n", id);
	sleep(1);
	sem_post(permits);
	pthread_mutex_unlock(left);
	pthread_mutex_unlock(right);
}

static void *philosopher(void *args) {
	t_params *p;

	p = (t_params *)args;
	for (int i = 0; i < kNumMeals; i++) {
		think(p->id);
		eat(p->id, p->left, p->right, p->permits);
	}
	return NULL;
}

int main(int argc, char *argv[]) {
	pthread_mutex_t forks[kNumPhilosophers];
	pthread_t philosophers[kNumPhilosophers];
	t_params params[kNumPhilosophers];
	sem_t *permits;

	permits = sem_open("/permits", O_CREAT|O_EXCL, S_IRWXU, kNumPhilosophers - 1);
	sem_unlink("/permits");

	for (int i = 0; i < kNumPhilosophers; i++) {
		pthread_mutex_init(&forks[i], NULL);
		params[i].id = i;
		params[i].left = &forks[i];
		params[i].right = &forks[(i + 1) % kNumPhilosophers];
		params[i].permits = permits;
		pthread_create(&philosophers[i], NULL, philosopher, (void *)(params + i));
	}
	for (int j = 0; j < kNumPhilosophers; j++) {
		pthread_join(philosophers[j], NULL);
	}
	sem_close(permits);
	return 0;
}

