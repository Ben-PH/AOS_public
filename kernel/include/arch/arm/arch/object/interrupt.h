/*
 * Copyright 2014, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#ifndef __ARCH_OBJECT_INTERRUPT_H
#define __ARCH_OBJECT_INTERRUPT_H

#include <types.h>
#include <api/failures.h>
#include <object/structures.h>
#include <plat/machine.h>

exception_t Arch_decodeIRQControlInvocation(word_t invLabel, word_t length,
                                            cte_t *srcSlot, extra_caps_t excaps,
                                            word_t *buffer);

/* Handle a platform-reserved IRQ. */
static inline void handleReservedIRQ(irq_t irq)
{
#ifdef CONFIG_IRQ_REPORTING
    if (irq != KERNEL_UART_IRQ) {
        printf("Received reserved IRQ: %d\n", (int)irq);
    }
#endif

#ifdef CONFIG_ARM_ENABLE_PMU_OVERFLOW_INTERRUPT
    if (irq == KERNEL_PMU_IRQ) {
        handleOverflowIRQ();
        return;
    }
#endif /* CONFIG_ARM_ENABLE_PMU_OVERFLOW_INTERRUPT */

#ifdef CONFIG_ARM_HYPERVISOR_SUPPORT
    if (irq == INTERRUPT_VGIC_MAINTENANCE) {
        VGICMaintenance();
        return;
    }
#endif

#ifdef CONFIG_ARM_SMMU
    if (irq == INTERRUPT_SMMU) {
        plat_smmu_handle_interrupt();
        return;
    }
#endif

    if (irq == KERNEL_UART_IRQ) {
        handleUartIRQ();
        return;
    }
}


static inline exception_t Arch_checkIRQ(word_t irq_w)
{
    if (irq_w > maxIRQ) {
        current_syscall_error.type = seL4_RangeError;
        current_syscall_error.rangeErrorMin = 0;
        current_syscall_error.rangeErrorMax = maxIRQ;
        userError("Rejecting request for IRQ %u. IRQ is greater than maxIRQ.", (int)irq_w);
        return EXCEPTION_SYSCALL_ERROR;
    }
    return EXCEPTION_NONE;
}

#endif
