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

#include <types.h>

#define CURRENTEL_EL2           (2 << 2)
word_t is_hyp_mode(void)
{
    uint32_t val;
    asm volatile("mrs %0, CurrentEL" : "=r"(val) :: "cc");
    return (val == CURRENTEL_EL2);
}

uint32_t read_cpuid_id(void)
{
    uint32_t val;
    asm volatile("mrs %0, midr_el1" : "=r"(val) :: "cc");
    return val;
}
