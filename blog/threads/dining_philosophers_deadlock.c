#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>

struct threadinfo
{
	int id;
	pthread_mutex_t *left;
	pthread_mutex_t *right;
};

static const int num_philosophers = 5;
static const int num_meals = 3;

static const unsigned int low_think_time = 100;
static const unsigned int high_think_time = 2000;
static const unsigned int low_eat_time = 25;
static const unsigned int high_eat_time = 50;

pthread_mutex_t rand_lock = PTHREAD_MUTEX_INITIALIZER;

static unsigned int get_rand(unsigned int low, unsigned int high)
{
	unsigned int num;

	pthread_mutex_lock(&rand_lock);
	num = (rand() % (high - low + 1)) + low;
	pthread_mutex_unlock(&rand_lock);
	return num;
}

static unsigned int get_think_time()
{
	return get_rand(low_think_time, high_think_time) * 1000;
}

static unsigned int get_eat_time()
{
	return get_rand(low_eat_time, high_eat_time) * 1000;
}

static void think(int id)
{
	printf("%d is thinking\n", id);
	usleep(get_think_time());
}

static void eat(int id, pthread_mutex_t *left, pthread_mutex_t *right)
{
	pthread_mutex_lock(left);
	sleep(1); // 人工的にデッドロックを再現したい
	pthread_mutex_lock(right);
	printf("%d is eating\n", id);
	usleep(get_eat_time());
	pthread_mutex_unlock(left);
	pthread_mutex_unlock(right);
}

static void *philosopher(void *arg)
{
	struct threadinfo *tip = arg;

	for (int i = 0; i < num_meals; i++)
	{
		think(tip->id);
		eat(tip->id, tip->left, tip->right);
	}
	return NULL;
}

int main(int argc, char *argv[])
{
	pthread_mutex_t forks[num_philosophers];
	pthread_t philosophers[num_philosophers];
	struct threadinfo ti[num_philosophers];

	srand(time(0));

	for (int i = 0; i < num_philosophers; i++)
	{
		pthread_mutex_init(&forks[i], NULL);
		ti[i].id = i;
		ti[i].left = &forks[i];
		ti[i].right = &forks[(i + 1) % num_philosophers];
		pthread_create(&philosophers[i], NULL, philosopher, (void *)&ti[i]);
	}
	for (int j = 0; j < num_philosophers; j++)
		pthread_join(philosophers[j], NULL);
	return 0;
}
