/*
 * Copyright 2019, Data61
 * Commonwealth Scientific and Industrial Research Organisation (CSIRO)
 * ABN 41 687 119 230.
 *
 * This software may be distributed and modified according to the terms of
 * the GNU General Public License version 2. Note that NO WARRANTY is provided.
 * See "LICENSE_GPLv2.txt" for details.
 *
 * @TAG(DATA61_GPL)
 */
/****************************************************************************
 *
 *      $Id:  $
 *
 *      Description: Simple milestone 0 test.
 *
 *      Author:         Godfrey van der Linden
 *      Original Author:    Ben Leslie
 *
 ****************************************************************************/

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sel4/sel4.h>
#include <syscalls.h>

#include "ttyout.h"

size_t sos_debug_print(const void *vData, size_t count)
{
#ifdef CONFIG_DEBUG_BUILD
  size_t i;
  const char *realdata = vData;
  for (i = 0; i < count; i++) {
    seL4_DebugPutChar(realdata[i]);
  }
#endif
  return count;
}

void tty_main();
// Block a thread forever
// we do this by making an unimplemented system call.

void thread_block(void)
{
    /* construct some info about the IPC message tty_test will send
     * to sos -- it's 1 word long */
    seL4_MessageInfo_t tag = seL4_MessageInfo_new(0, 0, 0, 1);
    /* Set the first word in the message to 0 */
    seL4_SetMR(0, 1);
    /* Now send the ipc -- call will send the ipc, then block until a reply
     * message is received */
    seL4_Call(SYSCALL_ENDPOINT_SLOT, tag);
    /* Currently SOS does not reply -- so we never come back here */
}

int main(void)
{
    sosapi_init_syscall_table();

    /* initialise communication */
    ttyout_init();
    sos_debug_print("checking to see if i can printf\n", 32);

    printf("foobar\n");
    printf("500chars00000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000054321X0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000054321X0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000054321X0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000054321X00000000000000000054321X");
    fflush(NULL);
    thread_block();

    return 0;
}
