#![no_std]
#![feature(lang_items)]

use debug_print::*;

#[lang = "eh_personality"] extern fn eh_personality() {}
#[panic_handler]
fn panic(info: &core::panic::PanicInfo) -> ! {
    dbg_println!("{:#?}", info);
    loop{}
}
