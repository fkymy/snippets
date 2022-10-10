#include <pthread.h>

struct semaphore {
	size_t value;
	pthread_cond_t *cv;
	pthread_mutex_t *m;
};

void sem_wait(struct semaphore *s) {
	pthread_mutex_lock(s->m);
	while (s->value == 0)
		pthread_cond_wait(s->cv, s->m);
	s->value--;
	pthread_mutex_unlock(s->m);
}

void sem_signal(struct semaphore *s) {
	pthread_mutex_lock(s->m);
	s->value++;
	if (s->value == 1)
		pthread_cond_broadcast(s->cv);
	pthread_mutex_unlock(s->m);
}
