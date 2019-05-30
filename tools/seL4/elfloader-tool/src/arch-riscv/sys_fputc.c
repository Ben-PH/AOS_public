/*
 * Copyright 2018, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(DATA61_GPL)
 */
/*
 * Platform-specific putchar implementation.
 */

#include <types.h>
#include <printf.h>
#include "sbi.h"

int __fputc(int c, UNUSED FILE *stream)
{
    if (c == '\n') {
        sbi_console_putchar('\r');
    }
    sbi_console_putchar(c);
    return 0;
}
