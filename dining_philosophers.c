#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

#define SECTONSEC 1000000000 /* sec to nanosec */
# define USECTONSEC 1000 /* microsec to nanosec */
# define MSECTOUSEC 1000 /* milisec to microsec */

typedef struct	s_params
{
	int id;
	pthread_mutex_t *left;
	pthread_mutex_t *right;
	int *permits;
	pthread_mutex_t *permits_lock;
}				t_params;

static const int kNumPhilosophers = 5;
static const int kNumForks = kNumPhilosophers;
static const int kNumMeals = 3;

void wait_for_permission(int *permits, pthread_mutex_t *permits_lock) {
	while (1)
	{
		pthread_mutex_lock(permits_lock);
		if (*permits > 0)
			break ;
		pthread_mutex_unlock(permits_lock);
		usleep(100);
	}
	(*permits)--;
	pthread_mutex_unlock(permits_lock);
}

void grant_permission(int *permits, pthread_mutex_t *permits_lock) {
	pthread_mutex_lock(permits_lock);
	(*permits)++;
	pthread_mutex_unlock(permits_lock);
}

static void think(int id) {
	printf("%d is thinking\n", id);
	sleep(1);
}

static void eat(int id, pthread_mutex_t *left, pthread_mutex_t *right, int *permits, pthread_mutex_t *permits_lock) {
	wait_for_permission(permits, permits_lock);
	pthread_mutex_lock(left);
	pthread_mutex_lock(right);
	printf("%d is eating\n", id);
	sleep(1);
	grant_permission(permits, permits_lock);
	pthread_mutex_unlock(left);
	pthread_mutex_unlock(right);
}

static void *philosopher(void *args) {
	t_params *p;

	p = (t_params *)args;
	for (int i = 0; i < kNumMeals; i++) {
		think(p->id);
		eat(p->id, p->left, p->right, p->permits, p->permits_lock);
	}
	return NULL;
}

int main(int argc, char *argv[]) {
	pthread_mutex_t forks[kNumPhilosophers];
	pthread_t philosophers[kNumPhilosophers];
	t_params params[kNumPhilosophers];

	int permits = kNumPhilosophers - 1;
	pthread_mutex_t permits_lock;
	pthread_mutex_init(&permits_lock, NULL);

	for (int i = 0; i < kNumPhilosophers; i++) {
		pthread_mutex_init(&forks[i], NULL);
		params[i].id = i;
		params[i].left = &forks[i];
		params[i].right = &forks[(i + 1) % kNumPhilosophers];
		params[i].permits = &permits;
		params[i].permits_lock = &permits_lock;
		pthread_create(&philosophers[i], NULL, philosopher, (void *)(params + i));
	}
	for (int j = 0; j < kNumPhilosophers; j++) {
		pthread_join(philosophers[j], NULL);
	}
	return 0;
}

