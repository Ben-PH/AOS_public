/*
 * Copyright 2014, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#ifndef __ARCH_API_TYPES
#define __ARCH_API_TYPES

#include <sel4/objecttype.h>
#include <config.h>
#include <sel4/sel4_arch/objecttype.h>
#include <sel4/arch/objecttype.h>

#define pageType PageObject4K

enum asidConstants {
    asidInvalid = 0
};

#define asidMax (BIT(asidLowBits + asidHighBits) - 1)

typedef word_t asid_t;

#endif
