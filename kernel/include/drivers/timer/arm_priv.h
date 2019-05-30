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

#ifndef __DRIVERS_TIMER_ARM_PRIV_H_
#define __DRIVERS_TIMER_ARM_PRIV_H_

#define TMR_INTS_EVENT       BIT(0)

/* 32 bit down counter */
struct timer {
    uint32_t load;
    uint32_t count;
    uint32_t ctrl;
    uint32_t ints;
};
typedef volatile struct timer timer_t;
extern timer_t *const priv_timer;

static inline void resetTimer(void)
{
    priv_timer->ints = TMR_INTS_EVENT;
}

#endif /* __DRIVERS_TIMER_ARM_PRIV_H_ */
