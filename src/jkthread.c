/**
 * \file
 * \author Julia Kaeppel
 */
#include "jkthread.h"

#include <assert.h>
#include <malloc.h>
#include <stddef.h>
#include <string.h>

#include "jkqueue.h"
#include "jkswitch.h"

/** Queue of threads. */
static struct JKQueue g_queue = { NULL };
/** Main thread. */
static struct JKThread g_thread;

int jkthread_create(struct JKThread *thread, void *(*start_routine)(void *),
    void *arg)
{
    int code = 0;
    void *sp;

    assert(thread != NULL);
    assert(start_routine != NULL);
    
    /* Create queue if it doesn't yet exist */
    if (g_queue.active == NULL) {
        /* Save main thread */
        jkthread_save_regs(&g_thread.registers);
        code = jkqueue_init(&g_queue, &g_thread);
        if (code) {
            goto cleanup;
        }
    }

    /* Create stack, aligned to a 16-byte boundary */
    thread->stack = memalign(16, JKTHREAD_STACK_SIZE);
    if (thread->stack == NULL) {
        code = JKTHREAD_STACK_LIMIT;
        goto cleanup;
    }

    /* Set stack and base pointers, setting up the 128-byte red zone */
    sp = &thread->stack[JKTHREAD_STACK_SIZE - 128];
    memcpy(thread->registers.rsp, &sp, JKTHREAD_REG_WIDTH);
    memcpy(thread->registers.rbp, &sp, JKTHREAD_REG_WIDTH);
    /* Set RIP value to start_routine */
    memcpy(thread->registers.rip, &start_routine, JKTHREAD_REG_WIDTH);

    /* Add thread to queue */
    jkqueue_push(&g_queue, thread);

    /* Context switch */
    /* Save current register state */
    jkthread_save_regs(&g_queue.active->thread->registers);
    /* Advance to the new thread */
    jkqueue_next(&g_queue);
    /* restore new thread's registers */
    jkthread_restore_regs(&g_queue.active->thread->registers,
        g_queue.active->prev->thread->registers.rip, arg);

    /* Clean up in case of error */
cleanup:
    if (code) {
        free(thread->stack);
    }

    return code;
}

void jkthread_switch()
{
    /* Save current register state */
    jkthread_save_regs(&g_queue.active->thread->registers);
    /* Advance to the next thread */
    jkqueue_next(&g_queue);
    /* Restore next thread's registers */
    jkthread_restore_regs(&g_queue.active->thread->registers,
        g_queue.active->prev->thread->registers.rip, NULL);
}
