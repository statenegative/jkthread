/**
 * \file
 * \author Julia Kaeppel
 * \brief Functions for context-switching.
 */
#ifndef JKSWITCH_H
#define JKSWITCH_H

#include "jkthread.h"

/**
 * \brief Save current register state to a JKThread object.
 * \param[out] thread The object to save the register state to.
 */
void jkthread_save_regs(struct JKThread *thread);

/**
 * \brief Restore registers from a saved state in a JKThead object.
 * \param[in] thread The object to restore the register state from.
 * \param[in] rip Pointer to the location the calling thread's RIP register
 * is saved to.
 * \param[in] rdi Value to pass as an input parameter to the restored thread.
 */
void jkthread_restore_regs(const struct JKThread *thread,
    char rip[JKTHREAD_REG_WIDTH], char rdi[JKTHREAD_REG_WIDTH]);

#endif
