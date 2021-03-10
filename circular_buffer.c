#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <pthread.h>

#define BUFFER_SIZE 64
#define min(a,b) ({\
		__typeof__ (a) _a = (a); \
		__typeof__ (b) _b = (b); \
		_a < _b ? _a : _b; })

struct bbuffer {
	char buf[BUFFER_SIZE];
	size_t capacity;
	size_t pos;
	size_t len;
	pthread_mutex_t	lock;
	pthread_cond_t nonfull;
	pthread_cond_t nonempty;
	int write_closed;
};

void thread_A(struct bbuffer *b);
void thread_B(struct bbuffer *b);

void bbuffer_close_write(struct bbuffer *b)
{
	pthread_mutex_lock(&b->lock);
	b->write_closed = 1;
	pthread_cond_broadcast(&b->nonempty);
	pthread_mutex_unlock(&b->lock);
}

ssize_t bbuffer_write(struct bbuffer *b, const char *buf, size_t size)
{
	pthread_mutex_lock(&b->lock);
	assert(!b->write_closed);
	while (b->len == b->capacity)
		pthread_cond_wait(&b->nonfull, &b->lock);
	size_t pos = 0;
	while (pos < size && b->len < b->capacity) {
		size_t bindex = (b->pos + b->len) % b->capacity;
		b->buf[bindex] = buf[pos];
		b->len++;
		pos++;
	}
	if (pos > 0)
		pthread_cond_broadcast(&b->nonempty);
	pthread_mutex_unlock(&b->lock);
	if (pos == 0 && size > 0) {
		return -1; /* try again */
	} else {
		return pos;
	}
}

ssize_t bbuffer_read(struct bbuffer *b, char *buf, size_t size)
{
	pthread_mutex_lock(&b->lock);
	while (b->len == 0 && !b->write_closed)
		pthread_cond_wait(&b->nonempty, &b->lock);
	if (b->write_closed)
		printf("write closed!\n");
	size_t pos = 0;
	while (pos < size && b->len > 0) {
		buf[pos] = b->buf[b->pos];
		b->pos = (b->pos + 1) % b->capacity;
		b->len--;
		pos++;
	}
	printf("pos: %zu\n", pos);
	if (pos > 0)
		pthread_cond_broadcast(&b->nonfull);
	pthread_mutex_unlock(&b->lock);
	if (pos == 0 && size > 0 && !b->write_closed) {
		return -1; /* try again */
	} else {
		return pos;
	}
}

// make these atomic
size_t nwrites;
size_t nreads;

int main()
{
	struct bbuffer b = { 0 };
	pthread_t t1, t2;

	b.capacity = BUFFER_SIZE;
	pthread_mutex_init(&b.lock, NULL);
	pthread_cond_init(&b.nonfull, NULL);
	pthread_cond_init(&b.nonempty, NULL);
	pthread_create(&t1, NULL, (void *)thread_A, (void *)&b);
	pthread_create(&t2, NULL, (void *)thread_B, (void *)&b);
	pthread_join(t1, NULL);
	pthread_join(t2, NULL);
	fprintf(stderr, "%zu reads, %zu writes\n", nreads, nwrites);
	return 0;
}

/* Writer thread function */
void thread_A(struct bbuffer *b)
{
	const char msg[] = "Hello world!\n";
	const size_t msg_len = strlen(msg);
	for (int i = 0; i < 1000000; i++) {
		size_t pos = 0;
		while (pos < msg_len) {
			ssize_t nw = bbuffer_write(b, &msg[pos], msg_len - pos);
			++nwrites;
			if (nw > -1) {
				pos += nw;
			}
		}
	}
	bbuffer_close_write(b);
}

/* Reader thread function */
void thread_B(struct bbuffer *b)
{
	char buf[BUFFER_SIZE];
	ssize_t nr;
	while ((nr = bbuffer_read(b, buf, sizeof(buf))) != 0) {
		nreads++;
		if (nr > -1) {
			write(1, buf, nr);
		}
	}
}
