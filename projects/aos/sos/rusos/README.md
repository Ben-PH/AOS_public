

Main event loop of the OS personality. The top of the loop is a straight up block on `msg = seL4_Recv($root_server-endpoint, &badge)`. End point is what we block on, waiting on IPC/NTFN.

Currently works the FFI by recieving a c_void. Improvements will include typing

Currently `extern "C"` is used, but that can be put out into a crate, and give us a safe wrapper.

1. On wake up, is seemingly analagous to OS161 when you're at the top of a function that's called at the end of context-switching assembly.
2. Following that analogy, we first need to learn our current context:
 * OS161 you have to load things from co-processor, which is set by interupt
 * seL4, instead of the co-processor, we use `msg = seL4_Recv($root-server-endpoint, &badge)`
3. check `badge` variable, set by the `Recv`, against `EP_BADGE` to check if notification/IPC.
 * this was setup in our call to `sos_init_irq_dispatch()`
 1. for notification, we branch off to `sos_handle_irq_notification(&badge)`
 2. for IPC: we assume it's a syscall (unless there's a fault).
  * specifications of `sos_init_irq_dispatch()` mean `badge & !IRQ_EP_BADGE` uniquely identifies the set of IRQs
  * this means we pass on badge so we know where we came from
 3. branch off to `handle_syscall(badge, $msg_legth)`
 4. If there WAS a fault, we just dump.
 5. end-of-loop back to the beginning to sleep on `Recv`
