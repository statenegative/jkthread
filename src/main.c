/**
 * \file
 * \author Julia Kaeppel
 * \brief Main file.
 */
#include <stdio.h>

#include "jkthread.h"

static void *entry_point(void *arg);

/**
 * \brief Main function.
 * \return Program exit code.
 */
int main(void)
{
    struct JKThread thread;
    jkthread_create(&thread, entry_point, NULL);
    return 0;
}

static void *entry_point(void *arg)
{
    printf("Something is working, at least!\n");
    return arg;
}
