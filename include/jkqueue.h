/**
 * \file
 * \author Julia Kaeppel
 * \brief Queue to manage JKThreads.
 */
#ifndef JKQUEUE_H
#define JKQUEUE_H

#include <stddef.h>

#include "jkerrno.h"
#include "jkthread.h"

/**
 * \brief Linked-list node used internally by the JKQueue struct.
 */
struct JKQueueNode {
    /** Pointer to the next node. */
    struct JKQueueNode *next;
    /** Pointer to the previous node. */
    struct JKQueueNode *prev;
    /** Pointer to the thread held by this node. */
    struct JKThread *thread;
};

/**
 * \brief Queue to manage JKThreads.
 */
struct JKQueue {
    /** Node containing the active thread. */
    struct JKQueueNode *active;
};

/**
 * \brief Initializes a JKQueue.
 * \param[out] queue The queue to initialize.
 * \param[in] thread The main thread.
 * \return 0 on success, or an error code on failure.
 */
int jkqueue_init(struct JKQueue *queue, struct JKThread *thread);

/**
 * \brief Pushes a new thread onto a JKQueue.
 * 
 * This action doesn't act like a normal queue push. Rather, the new thread is
 * placed right after the active node.
 * \param[out] queue The queue to push the thread onto.
 * \param[in] thread The thread to push onto the queue.
 * \return 0 on success, or an error code on failure.
 */
int jkqueue_push(struct JKQueue *queue, struct JKThread *thread);

/**
 * \brief Pops a thread off of a JKQueue.
 * \param[in,out] queue The queue to pop the thread off of.
 * \return A pointer to the thread popped off of the queue, or NULL if only the
 * main thread remains on the queue.
 */
struct JKThread *jkqueue_pop(struct JKQueue *queue);

/**
 * \brief Advances the active thread in a JKQueue.
 * \param[out] queue The queue to advance the active thread of.
 * \return A pointer to the new active thread.
 */
struct JKThread *jkqueue_next(struct JKQueue *queue);

#endif
