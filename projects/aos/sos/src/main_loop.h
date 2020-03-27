#pragma once

// #include UNUSED
#include <utils/attribute.h>
// #include _Word
#include <sel4/types.h>
#include <cspace/cspace.h>
#include <serial/serial.h>
#include "ut.h"

#define IRQ_EP_BADGE         BIT(seL4_BadgeBits - 1ul)

void handle_syscall(seL4_Word badge, uint32_t num_args, struct serial *serial, cspace_t *cspace);
NORETURN void syscall_loop(seL4_CPtr ep, struct serial *serial, cspace_t *cspace);
