#ifndef MLIST_H
#define MLIST_H

#include <stddef.h>
#include <stdbool.h>

#define MLIST_MALLOC(p, n)             \
  do {                                 \
    if (((p) = malloc(n)) == NULL) {   \
      printf("malloc failed");         \
      exit(-1);                        \
    }                                  \
  } while(false)

#define MLIST_FREE(p)                  \
  do {                                 \
    free(p);                           \
    (p) = NULL;                        \
  } while(false)

typedef struct mlist_t {
  void *mem_block;
  struct mlist_t *next;
  struct mlist_t *begin;
} mlist_t;

mlist_t *mlist_create(void);

void *mlist_alloc(mlist_t **p, size_t size);

void mlist_free(mlist_t *p);

#endif
