/*
 * Copyright 2016, General Dynamics C4 Systems
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(GD_GPL)
 */

#ifndef __KERNEL_ARM_TRAPS_H
#define __KERNEL_ARM_TRAPS_H

#include <config.h>
#include <util.h>

static inline void arch_c_entry_hook(void)
{
#ifndef CONFIG_ARCH_ARM_V6
    /* If using the TPIDRURW register for the IPC buffer then
     * there's no pointer in saving whatever the user put into
     * it since we will overwrite it anyway next time we
     * load the thread */
    if (!config_set(CONFIG_IPC_BUF_TPIDRURW)) {
        setRegister(NODE_STATE(ksCurThread), TPIDRURW, readTPIDRURW());
    }
#endif
#ifdef CONFIG_ARM_HYPERVISOR_SUPPORT
    if (NODE_STATE(ksCurThread)->tcbArch.tcbVCPU) {
        /* Although the TPIDRURO register is user read only it *is* writeable
         * by a thread running in supervisor mode and so in that case we need to
         * save it here as it actually could have changed */
#ifndef CONFIG_ARCH_AARCH64
        setRegister(NODE_STATE(ksCurThread), TLS_BASE, readTPIDRURO());
#endif
        /* in the case where we are using TPIDRURW as the IPC buffer we still
         * want to save the value of TPIDRURW (even though we skipped it just
         * above) as a supervisor mode thread will not understand or appreciate
         * seL4s desire that it be used as the IPC buffer and so we should respect
         * its changes */
        if (config_set(CONFIG_IPC_BUF_TPIDRURW)) {
            setRegister(NODE_STATE(ksCurThread), TPIDRURW, readTPIDRURW());
        }
    }
#endif
}

static inline void arch_c_exit_hook(void)
{
}

void VISIBLE NORETURN restore_user_context(void);

void c_handle_syscall(word_t cptr, word_t msgInfo, syscall_t syscall)
VISIBLE SECTION(".vectors.text");

void c_handle_interrupt(void)
VISIBLE SECTION(".vectors.text");

void c_handle_undefined_instruction(void)
VISIBLE SECTION(".vectors.text");

void c_handle_data_fault(void)
VISIBLE SECTION(".vectors.text");

void c_handle_instruction_fault(void)
VISIBLE SECTION(".vectors.text");

#ifdef CONFIG_ARM_HYPERVISOR_SUPPORT
void c_handle_vcpu_fault(word_t hsr)
VISIBLE SECTION(".vectors.text");
#endif /* CONFIG_ARM_HYPERVISOR_SUPPORT */

#ifdef CONFIG_HAVE_FPU
void c_handle_enfp(void)
VISIBLE SECTION(".vectors.text");
#endif /* CONFIG_HAVE_FPU */

#endif /* __KERNEL_ARM_TRAPS_H */
