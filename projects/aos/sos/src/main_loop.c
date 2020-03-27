#include "main_loop.h"
#include "irq.h"
#include <aos/sel4_zf_logif.h>
#include <aos/debug.h>


/*
 * A dummy starting syscall
 */
#define SOS_SYSCALL0 0

// Will be initialised when we bring in syscall loop


void handle_syscall(UNUSED seL4_Word badge, UNUSED uint32_t num_args, UNUSED struct serial *serial, cspace_t *cspace)
{
    ZF_LOGE("in handle syscall with cspace %p\n", cspace);
    /* allocate a slot for the reply cap */
    seL4_CPtr reply = cspace_alloc_slot(cspace);
    ZF_LOGE("got reply\n");
    /* get the first word of the message, which in the SOS protocol is the number
     * of the SOS "syscall". */
    seL4_Word syscall_number = seL4_GetMR(0);
    ZF_LOGE("got syscall number\n");
    /* Save the reply capability of the caller. If we didn't do this,
     * we coud just use seL4_Reply to respond directly to the reply capability.
     * However if SOS were to block (seL4_Recv) to receive another message, then
     * the existing reply capability would be deleted. So we save the reply capability
     * here, as in future you will want to reply to it later. Note that after
     * saving the reply capability, seL4_Reply cannot be used, as the reply capability
     * is moved from the internal slot in the TCB to our cspace, and the internal
     * slot is now empty. */
    seL4_Error err = cspace_save_reply_cap(cspace, reply);
    ZF_LOGF_IFERR(err, "Failed to save reply");

    /* Process system call */
    switch (syscall_number) {
    case SOS_SYSCALL0:
        ZF_LOGV("syscall: thread example made syscall 0!\n");
        /* construct a reply message of length 1 */
        seL4_MessageInfo_t reply_msg = seL4_MessageInfo_new(0, 0, 0, 1);
        /* Set the first (and only) word in the message to 0 */
        seL4_SetMR(0, 0);
        /* Send the reply to the saved reply capability. */
        seL4_Send(reply, reply_msg);
        /* Free the slot we allocated for the reply - it is now empty, as the reply
         * capability was consumed by the send. */
        cspace_free_slot(cspace, reply);
        break;
    case 1:
        ZF_LOGE("got syscall 1, which we are intentionally not replying to");
        break;

    case 2:
        // TODO: implement with a function in a binary for public use
        ZF_LOGE("syscall2: with this impemented, rusts .a should work!");
        break;

    default:
        ZF_LOGE("Unknown syscall %lu\n", syscall_number);
        /* don't reply to an unknown syscall */
    }
}

NORETURN void syscall_loop(seL4_CPtr ep, struct serial *serial, cspace_t *cspace)
{
    while (1) {
        seL4_Word badge = 0;
        /* Block on ep, waiting for an IPC sent over ep, or
         * a notification from our bound notification object */
        seL4_MessageInfo_t message = seL4_Recv(ep, &badge);
        /* Awake! We got a message - check the label and badge to
         * see what the message is about */
        seL4_Word label = seL4_MessageInfo_get_label(message);

        if (badge & IRQ_EP_BADGE) {
            /* It's a notification from our bound notification
             * object! */
            sos_handle_irq_notification(&badge);
        } else if (label == seL4_Fault_NullFault) {
            /* It's not a fault or an interrupt, it must be an IPC
             * message from tty_test! */
          ZF_LOGE("just about to handle syscall\n");
          handle_syscall(badge, seL4_MessageInfo_get_length(message) - 1, serial, cspace);
        } else {
            /* some kind of fault */
            
            /* debug_print_fault(message, TTY_NAME); */
            /* /\* dump registers too *\/ */
            /* debug_dump_registers(tty_test_process.tcb); */

            ZF_LOGF("The SOS skeleton does not know how to handle faults!");
        }
    }            debug_print_fault(message, TTY_NAME);
            /* dump registers too */
            debug_dump_registers(tty_test_process.tcb);

}
