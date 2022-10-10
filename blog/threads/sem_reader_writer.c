#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <semaphore.h>

// バッファの容量（バイト）
static const unsigned int num_buffer_slots = 8;

// 生産者・消費者スレッドがバッファをループする回数
static const unsigned int num_iterations = 3;

// データ処理にかかる時間のシミュレーション
static const unsigned int low_process_time_ms = 20;
static const unsigned int high_process_time_ms = 120;
static const unsigned int low_prepare_time_ms = 10;
static const unsigned int high_prepare_time_ms = 100;

// ログ出力のためのmutex
pthread_mutex_t print_lock;
// 乱数ジェネレータのためのmutex
pthread_mutex_t rand_lock;

static int get_rand(unsigned int low, unsigned int high)
{
	int num;

	pthread_mutex_lock(&rand_lock);
	num = (rand() % (high - low + 1)) + low;
	pthread_mutex_unlock(&rand_lock);
	return num;
}

static char prepare_data()
{
	usleep(1000 * get_rand(low_prepare_time_ms, high_prepare_time_ms));
	return get_rand('A', 'Z');
}

static void process_data(char ch)
{
	(void)ch;
	usleep(1000 * get_rand(low_process_time_ms, high_process_time_ms));
}

// バッファの内容を出力 |A|B|C...
static void print_buffer(char buffer[], size_t capacity)
{
	printf("BUFFER: |");
	for (size_t i = 0; i < capacity; i++)
		printf("%c|", buffer[i]);
	printf("\n");
}

struct threadinfo {
	char *buffer;
	sem_t *full;
	sem_t *empty;
	size_t capacity;
	size_t iterations;
};

void *write_to_buffer(void *arg)
{
	struct threadinfo *tip = arg;

	printf("Writer: Ready to write.\n");
	for (size_t i = 0; i < tip->iterations * tip->capacity; i++)
	{
		char ch = prepare_data();
		sem_wait(tip->empty);
		tip->buffer[i % tip->capacity] = ch;
		sem_post(tip->full);

		pthread_mutex_lock(&print_lock);
		printf("Writer: published data packet with character '%c'.\t\t", ch);
		print_buffer(tip->buffer, tip->capacity);
		pthread_mutex_unlock(&print_lock);
	}
	return NULL;
}

void *read_from_buffer(void *arg)
{
	struct threadinfo *tip = arg;

	printf("Reader: Ready to read.\n");
	for (size_t i = 0; i < tip->iterations * tip->capacity; i++)
	{
		sem_wait(tip->full);
		char ch = tip->buffer[i % tip->capacity];
		process_data(ch);
		tip->buffer[i % tip->capacity] = ' ';
		sem_post(tip->empty);

		pthread_mutex_lock(&print_lock);
		printf("Reader: consumed data packed with character '%c'.\t\t", ch);
		print_buffer(tip->buffer, tip->capacity);
		pthread_mutex_unlock(&print_lock);
	}
	return NULL;
}

int main(void)
{
	char buffer[num_buffer_slots];
	memset(buffer, ' ', sizeof(buffer));

	srand(time(0));
	pthread_mutex_init(&rand_lock, NULL);

	pthread_mutex_init(&print_lock, NULL);

	sem_t *full = sem_open("/full", O_CREAT|O_EXCL, S_IRWXU, 0);
	sem_t *empty = sem_open("/empty", O_CREAT|O_EXCL, S_IRWXU, sizeof(buffer));
	sem_unlink("/full");
	sem_unlink("/empty");

	pthread_t writer_thread;
	pthread_t reader_thread;

	struct threadinfo ti;
	ti.buffer = buffer;
	ti.full = full;
	ti.empty = empty;
	ti.capacity = sizeof(buffer);
	ti.iterations = num_iterations;
	pthread_create(&writer_thread, NULL, write_to_buffer, &ti);
	pthread_create(&reader_thread, NULL, read_from_buffer, &ti);

	pthread_join(writer_thread, NULL);
	pthread_join(reader_thread, NULL);
	sem_close(full);
	sem_close(empty);
	return 0;
}
