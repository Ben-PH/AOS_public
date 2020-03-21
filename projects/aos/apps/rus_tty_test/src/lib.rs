#![no_std]
#![feature(lang_items)]


// #[lang = "stack_exhausted"] extern fn stack_exhausted() {}
#[lang = "eh_personality"] extern fn eh_personality() {}
#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {loop{}}

extern "C" {
    fn seL4_DebugPutChar(c: u8);
}

fn tty_debug_print(string: &[u8]) -> usize {
    string.iter().for_each(|i| unsafe {seL4_DebugPutChar(*i)});
    string.len()
}

#[no_mangle]
extern "C" fn tty_main() -> u32 {
    tty_debug_print(b"rust says foobar!\n");
    1
}
