#![no_std]
#![feature(lang_items)]

mod debug_print;

#[lang = "eh_personality"] extern fn eh_personality() {}
#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {loop{}}


#[no_mangle]
extern "C" fn tty_main() -> u32 {
    dbg_println!("what _print?!?!?!");
    1
}
