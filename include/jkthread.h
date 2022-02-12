/**
 * \file
 * \author Julia Kaeppel
 * \brief User-level threads.
 */
#ifndef JKTHREAD_H
#define JKTHREAD_H

enum {
    /** Default stack size of 2MB. */
    JKTHREAD_STACK_SIZE = 2000000,

    /* Error codes */

    /** General allocation failed. */
    JKTHREAD_ALLOC_FAIL = 1,
    /** Stack allocation failed. */
    JKTHREAD_STACK_LIMIT = 2
};

/**
 * \brief User-level thread struct.
 */
struct JKThread {
    /** Register data. */
    char *regs;
    /** Stack pointer. */
    char *stack;
};

/**
 * \brief Creates a JKThread.
 * \param[out] thread The thread to create.
 * \param[in] start_routine The entry point of the thread.
 * \param[in] arg The argument to pass to the entry point function.
 * \return 0 on success. Returns an error code on failure.
 */
int jkthread_create(struct JKThread *thread, void *(*start_routine)(void *),
    void *arg);

#endif
