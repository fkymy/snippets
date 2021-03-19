#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <semaphore.h>

// The size of the buffer, in bytes
static const unsigned int kNumBufferSlots = 8;

// The number of times the reader and write should loop through the buffer.
static const unsigned int kNumIterations = 3;

// The bounds for how much the reader could sleep for when processing data
static const unsigned int kLowProcessTimeMS = 20;
static const unsigned int kHighProcessTimeMS = 120;

// The bounds for how much the writer could sleep for when preparing data
static const unsigned int kLowPrepareTimeMS = 10;
static const unsigned int kHighPrepareTimeMS = 100;

// Shared random time generator (in milliseconds) to simulate data processing
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
	usleep(1000 * get_rand(kLowPrepareTimeMS, kHighPrepareTimeMS));
	return get_rand('A', 'Z');
}

static void process_data(char ch)
{
	(void)ch;
	usleep(1000 * get_rand(kLowProcessTimeMS, kHighProcessTimeMS));
}

// prints the content of the given buffer in the format |A|B|C|...
// performs no concurrency/coordination

static void print_buffer(char buffer[], size_t size)
{
	printf("BUFFER: |");
	for (size_t i = 0; i < size; i++)
		printf("%c|", buffer[i]);
	printf("\n");
}

struct threadinfo {
	char *buffer;
	sem_t *full;
	sem_t *empty;
	size_t size;
	size_t iterations;
	pthread_mutex_t *write_lock;
};

void *write_to_buffer(void *arg)
{
	struct threadinfo *tip = arg;

	printf("Writer: Ready to write.\n");
	for (size_t i = 0; i < tip->iterations * tip->size; i++)
	{
		char ch = prepare_data();

		// wait for space before writing
		sem_wait(tip->empty);
		tip->buffer[i % tip->size] = ch;

		// signal that there is content to read
		sem_post(tip->full);

		pthread_mutex_lock(tip->write_lock);
		printf("Writer: published data packet with character '%c'.\t\t", ch);
		print_buffer(tip->buffer, tip->size);
		pthread_mutex_unlock(tip->write_lock);
	}
	return NULL;
}

void *read_from_buffer(void *arg)
{
	struct threadinfo *tip = arg;

	printf("Reader: Ready to read.\n");
	for (size_t i = 0; i < tip->iterations * tip->size; i++)
	{
		// wait for content before reading
		sem_wait(tip->full);

		// read and process data
		char ch = tip->buffer[i % tip->size];
		process_data(ch);
		tip->buffer[i % tip->size] = ' ';

		// signal that there is space to write more content
		sem_post(tip->empty);

		pthread_mutex_lock(tip->write_lock);
		printf("Reader: consumed data packed with character '%c'.\t\t", ch);
		print_buffer(tip->buffer, tip->size);
		pthread_mutex_unlock(tip->write_lock);
	}
	return NULL;
}

int main(void)
{
	char buffer[kNumBufferSlots];
	memset(buffer, ' ', sizeof(buffer));

	srand(time(0));
	pthread_mutex_init(&rand_lock, NULL);

	pthread_mutex_t write_lock;
	pthread_mutex_init(&write_lock, NULL);

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
	ti.size = sizeof(buffer);
	ti.iterations = kNumIterations;
	ti.write_lock = &write_lock;
	pthread_create(&writer_thread, NULL, write_to_buffer, &ti);
	pthread_create(&reader_thread, NULL, read_from_buffer, &ti);

	pthread_join(writer_thread, NULL);
	pthread_join(reader_thread, NULL);
	sem_close(full);
	sem_close(empty);
	return 0;
}
