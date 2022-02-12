/**
 * \file
 * \author Julia Kaeppel
 */
#include "jkthread.h"

#include <assert.h>
#include <stdlib.h>
#include <string.h>

enum {
    /** Size of all registers, in bytes. */
    REGS_SIZE = 72 * 8,
    /** Width of a register, in bytes. */
    REG_WIDTH = 8,

    /* Register offsets */

    /* RAX */
    RAX_OFFSET = 0 * REG_WIDTH,
    /* RBX */
    RBX_OFFSET = 1 * REG_WIDTH,
    /* RCX */
    RCX_OFFSET = 2 * REG_WIDTH,
    /* RDX */
    RDX_OFFSET = 3 * REG_WIDTH,
    /* RSI */
    RSI_OFFSET = 4 * REG_WIDTH,
    /* RDI */
    RDI_OFFSET = 5 * REG_WIDTH,
    /* RSP */
    RSP_OFFSET = 6 * REG_WIDTH,
    /* RBP */
    RBP_OFFSET = 7 * REG_WIDTH,
    /* RIP */
    RIP_OFFSET = 8 * REG_WIDTH
};

/**
 * \brief Save current register state to a JKThread object.
 * \param[out] thread The object to save the register state to.
 */
static void save_regs(struct JKThread *thread);

/**
 * \brief Restore registers from a saved state in a JKThead object.
 * \param[in] thread The object to restore the register state from.
 */
static void restore_regs(const struct JKThread *thread);

int jkthread_create(struct JKThread *thread, void *(*start_routine)(void *),
    void *arg)
{
    int code = 0;

    assert(thread != NULL);
    assert(start_routine != NULL);

    /* Initialize pointer values to NULL in case of failure */

    /* Create register storage */
    thread->regs = malloc(REGS_SIZE);
    if (thread->regs == NULL) {
        code = JKTHREAD_ALLOC_FAIL;
        goto cleanup;
    }

    /* Create stack */
    thread->stack = malloc(JKTHREAD_STACK_SIZE);
    if (thread->stack == NULL) {
        code = JKTHREAD_STACK_LIMIT;
        goto cleanup;
    }

    /* Set stack and base pointer values to the beginning of the stack */
    memcpy(&thread->regs[RSP_OFFSET], &thread->stack, REG_WIDTH);
    memcpy(&thread->regs[RBP_OFFSET], &thread->stack, REG_WIDTH);

    (void)start_routine;
    (void)arg;
    save_regs(thread);
    restore_regs(thread);

    /* Clean up in case of error */
cleanup:
    if (code) {
        free(thread->regs);
        free(thread->stack);
    }

    return code;
}

static void save_regs(struct JKThread *thread)
{
    char rip[REG_WIDTH];

    __asm__("movq %%rax, %[rax]\n"
            "movq %%rbx, %[rbx]\n"
            "movq %%rcx, %[rcx]\n"
            "movq %%rdx, %[rdx]\n"
            "movq %%rsi, %[rsi]\n"
            "movq %%rdi, %[rdi]\n"
            "movq %%rsp, %[rsp]\n"
            "movq %%rbp, %[rbp]\n"
            "leaq (%%rip), %[rip]\n"
        : [rax] "=m" (thread->regs[RAX_OFFSET]),
          [rbx] "=m" (thread->regs[RBX_OFFSET]),
          [rcx] "=m" (thread->regs[RCX_OFFSET]),
          [rdx] "=m" (thread->regs[RDX_OFFSET]),
          [rsi] "=m" (thread->regs[RSI_OFFSET]),
          [rdi] "=m" (thread->regs[RDI_OFFSET]),
          [rsp] "=m" (thread->regs[RSP_OFFSET]),
          [rbp] "=m" (thread->regs[RBP_OFFSET]),
          [rip] "=r" (rip) /* dest of leaq must be a register */
    );

    /* Save rip value */
    memcpy(&thread->regs[RIP_OFFSET], &rip, REG_WIDTH);
}

static void restore_regs(const struct JKThread *thread)
{
    __asm__("movq %[rax], %%rax\n"
            "movq %[rbx], %%rbx\n"
            "movq %[rcx], %%rcx\n"
            "movq %[rdx], %%rdx\n"
            "movq %[rsi], %%rsi\n"
            "movq %[rdi], %%rdi\n"
            "movq %[rsp], %%rsp\n"
            "movq %[rbp], %%rbp\n"
            "jmp %[rip]\n"
        : /* No input operands */
        : [rax] "m" (thread->regs[RAX_OFFSET]),
          [rbx] "m" (thread->regs[RBX_OFFSET]),
          [rcx] "m" (thread->regs[RCX_OFFSET]),
          [rdx] "m" (thread->regs[RDX_OFFSET]),
          [rsi] "m" (thread->regs[RSI_OFFSET]),
          [rdi] "m" (thread->regs[RDI_OFFSET]),
          [rsp] "m" (thread->regs[RSP_OFFSET]),
          [rbp] "m" (thread->regs[RBP_OFFSET]),
          [rip] "m" (thread->regs[RIP_OFFSET])
    );
}
