files that are not needed for direct linking but can be used as helper crates. For example, the debug_print provides `dbg_println!` macro, which is `print!` and `println!`, but especially using uart and `seL4_DebugPutChar(u8)`. 

Plan is to implement `println!` using serial send
