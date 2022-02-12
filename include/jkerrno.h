/**
 * \file
 * \author Julia Kaeppel
 * \brief Error codes for the JKThread library.
 */
#ifndef JKERRNO_H
#define JKERRNO_H

enum {
    /* Error codes */

    /** General allocation failed. */
    JKTHREAD_ALLOC_FAIL = 1,
    /** Stack allocation failed. */
    JKTHREAD_STACK_LIMIT = 2
};

#endif
