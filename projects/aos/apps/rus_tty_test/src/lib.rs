#![no_std]
#![feature(lang_items)]

mod debug_print;

#[lang = "eh_personality"] extern fn eh_personality() {}
#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {loop{}}
#[repr(C)]
pub struct seL4_MessageInfo {
    pub words: [u64; 1usize],
}

extern "C" {
    fn seL4_SetMR(_: u64, _: u64);
    fn seL4_MessageInfo_new(_: u64, _: u64, _: u64, _: u64) -> seL4_MessageInfo;
    fn seL4_Call(_: u64, _: seL4_MessageInfo);
}

fn thread_block() {
    /* construct some info about the IPC message tty_test will send
     * to sos -- it's 1 word long */
    let tag = unsafe { seL4_MessageInfo_new(0, 0, 0, 1) };
    /* Set the first word in the message to 0 */
    unsafe { seL4_SetMR(0, 1); }
    /* Now send the ipc -- call will send the ipc, then block until a reply
     * message is received */
    unsafe { seL4_Call(1, tag); }
    /* Currently SOS does not reply -- so we never come back here */
}

#[no_mangle]
extern "C" fn tty_main()  {
    dbg_println!("what _print?!?!?!");
    // dummy_printf("task:\tHello world, I'm\trus_tty_test!\n");
    thread_block();

}
