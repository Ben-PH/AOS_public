# Rust implemented tty_test for UNSW Adv. OS

Intro goes here? Context is 2019 project

## From initial entry to user-land, through `main`, and onto event loop

Roughly:

1. kernel-to-uland with `_sel4_start`
2. which calls `_start`
3. which calls `main`
4. which starts the first process
5. then goes into the main event loop: `syscall_loop`

tty_test is the first milestone in the course, and serves as a "hello world" implementation for seL4 using the provided libraries. As initially provided, the boot-sequence goes like this:

1. The seL4 kernel finishes initialisation, context switches into userlands `_sel4_start` in `projects/aos/sos/crt/sel4_crt0.S`
NOTE: The project does this with `tools/seL4/cmake-tool/helpers/rootserver.cmake` which is used by cmakes `DeclareRootServer` will show that the 
2. `_sel4_start` does a whole heap of root-server specific setup, architecture specific. Summarised as follows

 * Setup a stack for ourselves.
 * Construct bootinfo environment variable
 * Construct the boot_tcb_cptr environment variable
 * _First call out to C_ to get the value of seL4_CapInitThreadTCB
 * Setup stack frame ready for jumping to _start
 * null terminate auxv TODO: understand why
 * give vsyscall location TODO: understand how this ties in with libc runtime
 * Pass in information on our simulated TLS ELF headers, after constructing the missing pieces TODO: ???
 * Give default page size
 * Null terminate envp
 * Give initial tcb location TODO: understand how this ties in with scheduler
 * Give bootinfo location
 * Set the environment to seL4 TODO: what purpose does this serve? is this envp?
 * Null terminate argument vector
 * Give a name in the for of `prog_name`
 * Push argument count
 * **Now go to actual** `_start`. NOTE: This is the `_start` from provided elf-loader tooling. Not kernel
 
3. in `_start`, we have our `la s0, main \n jr s0`, putting us in root-servers `main()`
4. _first_ thing done, is `init_muslc()`. 
5. allow for frame unwinding (`backtrace()`)
6. setup cspace, ut manager, and dma
7. allow for uart output NOTE: In debug, this is shared with kernel
8. expand the stack and run on that
9. initialise the IRQ dispatch system 
10. initialise the frame table with the root-servers cspace, and its VSpace
11. runs tests on this cspace, dma, and frame-tabling.
12. map in the timer device. TODO: writeup as a case study for mapping in device
13. init the network stack. TODO: .md writup on what the args do
14. init the serial device
15. `start_first_process()` a name (initially `"tty_test"`), and `ipc_ep` capability, which was initialised in `sos_ipc_init(endpointCP, notificactionCP)`
16. jumps straight into `syscall_loop` with the same `ipc_ep` as its argument.
17. also gives the pointer to the serial device to allow serial out

