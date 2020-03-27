# SOS in the AOS project

This Readme is not for reference purposes, rather, provide you with context.


## general main.c

 * `alloc_retype` is a helper to `ut_alloc` and `cspace_alloc_slot`. The rusult of the `ut_alloc` gets put into the allocated slot
 * `extern char _cpio_archive[]` and `extern char _cpio_archive_end[]` are used in starting first process
  * used along with file-name of the app to get an `elf_base` with `cpio_get_file()`


## IRQ dispatch initialisation


function `sos_init_irq_diispatch` inside `irq.[hc]`

key points when called by root server

 * Is called immediately after `sos_ipc_init`, which sets its caps for ipc endpoint and notification objects
 * It's given a `cspace_t` pointer reffing to its global static
 * Takes a hardcoded value indexing into its IRQ control cap in its cspace
 * its ntfn was set during `sos_ipc_init`
 * we set flag high bit to 1 (think signed/unsigned high bit) as per `IRQ_EP_BADGE`
  * minted notifications have badges with these bits set.
  * think `0x8000_0000_0000_0000` & IRQ == `0x8000_0000_0000_0000` => got notification
 * How to look at the bits to identify where an IRQ comes from
  * think `0x8000_0000_0000_0000` ^ IRQ == uniquely ID the IRQ. "retail staff with a name badges"

Regarding implementation
 * **the irq_dispatch data struct is a file-global static-struct in `irq.c`**
  * cspace: TODO implication of putting a particular cspace in here
  * irq_control: TODO
  * notification: TODO: aos content discussing notifications
  * flag_bits: specification for unwrapping an IRQ into a Notification or IPC
  * ident: specification for value wrapped up in an IRQ
 * initialisation puts values into this struct
 * flag and ident bits must be disjoint


## tests

currently tests
 * passed-in cspace against
  * dummy 1 level cspace
  * dummy 2 level cspace
 * dma by
  * running `dma = sos_dma_malloc(4k page, 4k page)`
  * looping through the `dma.vaddr` and setting values, then...
  * checking the values set correctly
 * frame table by
  * looping through a frames array to give an allocated frame
   * asserting not allocated a `NULL_FRAME`
   * make a `vaddr[]` by passing the allocated frame into `frame_data()`
   * writing index first and last byte of `vaddr`
  * loop through the frame array again
   * `vaddr[]` into a frame
   * `assert` the bight was written as expected
  * loop through again to free all frames
  * check we get same frames when realloc by
   * making a `new_frames[]` and iterating over it. at each index
    * `alloc_frame()`
    * scan through the old frame-array to assert new allocated frame matches an old one
  * clean up

NOTE: dma test is basically "iterate an array and write to it then do exactly the same iteration to check that you read the same as what you just wrote"


## start\_first\_process

NOTE: An unsuccesful process start isn't cleaned up - a user spamming the system with fail-to-start processes will cause the OS to run outa memory

 * implemented within `main.c`
 * `main.c` holds a static struct that collects kernel objects and caps to describe a process: cspace, tcb, etc
  * we set `.vspace` by making a new one, putting into a slot and passing back the result
  * give `.vspace` to an asid pool
  * make a 1-level c-space and put it into `.cspace` with root-servers static `cspace` wrapping it
  * create an ipc buffer and put it into `.ipc_buffer`, we'll be mapping this in later
  * allocate a slot in `.cspace`,
   * subjected to `cspace_mint()`, provided by `libsel4cspace`
    * like `copy`, but we need the badge-setting feature of `mint`
    * connect the root-server and tty_test endpoints
  * make a tcp and configure it
   * [The seL4 tutorial](https://docs.sel4.systems/Tutorials/threads.html#configure-a-tcb) is where you want to go for this one.
   * we also name and priority-set it
  * set up the stack, and get sp, for this process using `init_process_stack`.
   * System V compatable to **start and init the C library** TODO: write-up on this
   * needs creating contexts cspace, vspace cap, and the elf file
   * map it in for the proc using `map_frame` with `.stack`, `.vspace`
   * the cap on this fram, we want it in sos addr space (alloc a cspace slot, copy it in, map into sos
   * set `main()` arguments for the new proc
   * clean up things root-server side
  * we can now `elf_load()` our file into `.vspace`
  * we can now map in `.ipc_buffer`
  * load up the program counter and sp into the user-context register set.
  * write these registers into the tcp and set it to continue.


## syscall\_loop()
see `sos/src/rusos/README.md` for more readme.
