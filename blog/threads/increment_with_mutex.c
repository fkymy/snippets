#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static void *thread_routine(void *arg)
{
	int *np = (int *)arg;

	for (int i = 0; i < 1000000; i++)
	{
		pthread_mutex_lock(&lock);
		*np += 1; /* 際どい部分 */
		pthread_mutex_unlock(&lock);
	}
	return NULL;
}

int main(void)
{
	pthread_t threads[4];

	int n = 0;

	for (size_t i = 0; i < 4; i++)
		pthread_create(&threads[i], NULL, thread_routine, &n);
	for (size_t j = 0; j < 4; j++)
		pthread_join(threads[j], NULL);
	printf("n: %d\n", n);
	return 0;
}
