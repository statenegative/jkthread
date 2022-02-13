/**
 * \file
 * \author Julia Kaeppel
 */
#include "jkqueue.h"

#include <assert.h>
#include <stdlib.h>

enum {
    /* Default number of threads held by the queue. */
    DEF_THREADS = 4
};

int jkqueue_init(struct JKQueue *queue, struct JKThread *thread)
{
    assert(queue != NULL);
    assert(thread != NULL);

    /* Allocate main thread node */
    queue->active = malloc(sizeof *queue->active);
    if (queue->active == NULL) {
        return JKTHREAD_ALLOC_FAIL;
    }

    /* Set node values */
    queue->active->next = queue->active;
    queue->active->prev = queue->active;
    queue->active->thread = thread;

    return 0;
}

int jkqueue_push(struct JKQueue *queue, struct JKThread *thread)
{
    assert(queue != NULL);
    assert(thread != NULL);

    /* Allocate new node */
    queue->active->next->prev = malloc(sizeof *queue->active->next->prev);
    if (queue->active->next->prev == NULL) {
        return JKTHREAD_ALLOC_FAIL;
    }

    /* Update prev node */
    queue->active->next->prev->next = queue->active->next;
    queue->active->next->prev->prev = queue->active;
    queue->active->next->prev->thread = thread;
    queue->active->next = queue->active->next->prev;

    return 0;
}

struct JKThread *jkqueue_pop(struct JKQueue *queue)
{
    struct JKQueueNode *node;

    assert(queue != NULL);

    /* Don't remove the main node if only it remains */
    if (queue->active == queue->active->next) {
        return NULL;
    }

    /* Save active node, which will be popped off the queue */
    node = queue->active;

    /* Update active node and preserve prev node */
    queue->active = queue->active->next;
    queue->active->prev = node->prev;

    /* Handle case where only the main thread will remain */
    if (queue->active == queue->active->prev) {
        queue->active->next = queue->active;
    }

    return node->thread;
}

struct JKThread *jkqueue_next(struct JKQueue *queue)
{
    assert(queue != NULL);

    queue->active = queue->active->next;
    return queue->active->thread;
}
