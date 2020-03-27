#![no_std]
#![feature(lang_items)]

use debug_print::*;

pub const IRQ_EP_BADGE: u64 = 0b1000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000_0000;

#[lang = "eh_personality"] extern fn eh_personality() {}
#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {loop{}}

#[repr(C)]
#[derive(Debug)]
pub struct seL4_MessageInfo {
    pub words: [u64; 1],
}

// cspace and serial are opaque structs passed in on entry to syscall_loop
#[derive(Debug)]
#[repr(C)] pub struct CSpace { private: [u8; 0] }

extern "C" {
    fn seL4_Recv(endpoint: u64, badge: *mut u64) -> seL4_MessageInfo;
    fn seL4_MessageInfo_ptr_get_label(msg: *mut seL4_MessageInfo) -> u64;
    fn seL4_MessageInfo_ptr_get_length(msg: *const seL4_MessageInfo) -> u64;
    fn seL4_MessageInfo_new(_: u64, _: u64, _: u64, _: u64) -> seL4_MessageInfo;
    fn sos_handle_irq_notification(badge: &u64);
    fn debug_dump(msg: seL4_MessageInfo);
    fn cspace_alloc_slot(cspace: *mut CSpace) -> u64;
    fn cspace_free_slot(cspace: *mut CSpace, slot: u64) -> u64;
    fn seL4_GetMR(idx: u64) -> u64;
    fn seL4_SetMR(idx: u64, val: u64);
    fn seL4_Send(ep_cptr: u64, msg: seL4_MessageInfo);
    fn cspace_save_reply_cap(cspace: *mut CSpace, reply: u64);
    fn serial_send(serial: *mut core::ffi::c_void, bytes: *const u8, len: usize) -> usize;
    fn sos_debug_print(chars: *const u8, count: isize) -> isize;
    fn abort();
}

#[allow(bad_style)]
pub const seL4_Fault_NullFault: u64 = 0;

fn ros_debug_print(msg: &[u8]) {
    unsafe { sos_debug_print(msg.as_ptr(), msg.len() as isize); }
}
#[no_mangle]
extern "C" fn syscall_loop(endpoint: u64, serial: *mut core::ffi::c_void, cspace: *mut CSpace) -> ! {
    loop {
        let mut badge = 0u64;
        let mut msg = unsafe { seL4_Recv(endpoint, &mut badge as *mut u64) };

        let label = unsafe { seL4_MessageInfo_ptr_get_label(&mut msg as *mut seL4_MessageInfo) };

        if badge & IRQ_EP_BADGE != 0 {
            unsafe { sos_handle_irq_notification(&badge) }
        } else if label == seL4_Fault_NullFault {
            let len = unsafe { seL4_MessageInfo_ptr_get_length(&msg as *const seL4_MessageInfo) };
            match handle_syscall(badge, len as u32 - 1, serial, cspace) {
                Ok(_) => {},
                _ => dbg_println!("bad syscall\n{:?}\n\
                                   badge: {:?}\n\
                                   serial: {:?}\n\
                                   cspace: {:?}", msg, badge, serial, cspace),
            };
        } else {
            dbg_println!("isnt irq, isnt no-fault");
            unsafe {
                debug_dump(msg);
                abort();

            }

        }
    }
}

fn handle_syscall(_badge: u64, num_args: u32, serial: *mut core::ffi::c_void, cspace: *mut CSpace) -> Result<(), ()> {
    let reply = unsafe { cspace_alloc_slot(cspace) };
    let syscall_no = unsafe { seL4_GetMR(0) };
    unsafe { cspace_save_reply_cap(cspace, reply)}
    dbg_print!("{:?}", syscall_no);

    match syscall_no {
        0 => unsafe {// run syscall 0

            let reply_msg = seL4_MessageInfo_new(0, 0, 0, 1);
            seL4_SetMR(0, 0);
            seL4_Send(reply, reply_msg);
            cspace_free_slot(cspace, reply);
            Ok(())

        },
        1 => {
            ros_debug_print(b"in dummy syscall 1\n");
            Ok(())}, // intentionally not replying to syscall 1
        2 => {
            serial_send(serial, "change rusos/lib.rs to complete implementation of tty_out\n".as_ptr(), 58);
            Ok(())
        }, // print out with serial send
        _ => Err(())
    }
}
