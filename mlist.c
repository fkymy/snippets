#include <stdio.h>
#include <stdlib.h>
#include "mlist.h"

static mlist_t *mlist_extend(mlist_t *p);

mlist_t *mlist_create (void)
{
    mlist_t *p;

    MLIST_MALLOC(p, sizeof(mlist_t));
    p->mem_block = NULL;
    p->next  = NULL;
    p->begin = p;
    return p;
}

void *mlist_alloc (mlist_t **p, size_t size)
{
    mlist_t *lp = *p;
    MLIST_MALLOC(lp->mem_block, size);
    *p = mlist_extend(*p);
    return lp->mem_block;
}

void mlist_free (mlist_t *p)
{
    mlist_t *curr, *last;

    curr = p->begin;
    while (curr != NULL)
    {
        MLIST_FREE(curr->mem_block);
        last = curr;
        curr = curr->next;
        MLIST_FREE(last);
    }
}

mlist_t *mlist_extend(mlist_t *p)
{
  p->next        = mlist_create();
  p->next->begin = p->begin;
  return p->next;
}
