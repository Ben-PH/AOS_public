#![no_std]
pub struct DbgWriter;

extern "C" { fn seL4_DebugPutChar(ch: u8); }
fn tty_debug_print(string: &str) -> usize {
    string
        .bytes()
        .for_each(|i| unsafe {seL4_DebugPutChar(i)});

    string.len()
}
impl DbgWriter {
    fn write_string(s: &str) -> core::fmt::Result {
        tty_debug_print(s);
        Ok(())
    }
}
impl core::fmt::Write for DbgWriter {
    fn write_str(&mut self, s: &str) -> core::fmt::Result {
        Self::write_string(s)
    }
}

#[doc(hidden)]
pub fn _print(args: core::fmt::Arguments) {
    use core::fmt::Write;
    DbgWriter.write_fmt(args).unwrap();
}

#[macro_export]
macro_rules! dbg_print {
    ($($arg:tt)*) => ($crate::_print(format_args!($($arg)*)));
}

#[macro_export]
macro_rules! dbg_println {
    () => (dbg_print!("\n"));
    ($($arg:tt)*) => (dbg_print!("{}\n", format_args!($($arg)*)));
}

