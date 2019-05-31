/*
 * Copyright 2017, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(DATA61_GPL)
 */

/* Default implementations of required utility functions. Override these under
 * plat-* if there is a more appropriate implementation for a given platform.
 */

#include <printf.h>

void __attribute__((weak)) abort(void)
{
    printf("abort() called.\n");

    while (1);
}