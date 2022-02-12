/**
 * \file
 * \author Julia Kaeppel
 * \brief User-level threads.
 */
#ifndef JKTHREAD_H
#define JKTHREAD_H

#include "jkerrno.h"

enum {
    /** Default stack size of 2MB. */
    JKTHREAD_STACK_SIZE = 2000000,
    /** Width of a register, in bytes. */
    JKTHREAD_REG_WIDTH = 8
};

/**
 * \brief Holds saved register values.
 */
struct JKThreadRegisters {
    /** RBX register. */
    char rbx[JKTHREAD_REG_WIDTH];
    /** RSP register. */
    char rsp[JKTHREAD_REG_WIDTH];
    /** RBP register. */
    char rbp[JKTHREAD_REG_WIDTH];
    /** R12 register. */
    char r12[JKTHREAD_REG_WIDTH];
    /** R13 register. */
    char r13[JKTHREAD_REG_WIDTH];
    /** R14 register. */
    char r14[JKTHREAD_REG_WIDTH];
    /** R15 register. */
    char r15[JKTHREAD_REG_WIDTH];
    /** RIP register. */
    char rip[JKTHREAD_REG_WIDTH];
};

/**
 * \brief User-level thread struct.
 */
struct JKThread {
    struct JKThreadRegisters registers;
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

/**
 * \brief Performs a context switch.
 */
void jkthread_switch();

#endif
