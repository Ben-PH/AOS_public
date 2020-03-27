#![no_std]
#![feature(lang_items)]

pub const IRQ_EP_BADGE: u64 = 1 << 63;

#[lang = "eh_personality"] extern fn eh_personality() {}
#[panic_handler]
fn panic(_info: &core::panic::PanicInfo) -> ! {loop{}}

#[repr(C)]
pub struct seL4_MessageInfo {
    pub words: [u64; 1usize],
}

// cspace and serial are opaque structs passed in on entry to syscall_loop
#[repr(C)] pub struct CSpace { private: [u8; 0] }
#[repr(C)] pub struct Serial { private: [u8; 0] }

extern "C" {
    fn seL4_Recv(endpoint: u64, badge: &mut u64) -> seL4_MessageInfo;
    fn seL4_MessageInfo_get_label(msg: &seL4_MessageInfo) -> u64;
    fn seL4_MessageInfo_get_length(msg: &seL4_MessageInfo) -> u64;
    fn seL4_MessageInfo_new(_: u64, _: u64, _: u64, _: u64) -> seL4_MessageInfo;
    fn sos_handle_irq_notification(badge: &u64);
    fn debug_dump();
    fn cspace_alloc_slot(cspace: *mut CSpace) -> u64;
    fn cspace_free_slot(cspace: *mut CSpace, slot: u64) -> u64;
    fn seL4_GetMR(idx: u64) -> u64;
    fn seL4_SetMR(idx: u64, val: u64);
    fn seL4_Send(ep_cptr: u64, msg: seL4_MessageInfo);
    fn cspace_save_reply_cap(cspace: *mut CSpace, reply: u64);
    fn serial_send(serial: *mut Serial, bytes: *const u8, len: usize) -> usize;
}

pub const seL4_Fault_NullFault: u64 = 0;

#[no_mangle]
extern "C" fn syscall_loop(endpoint: u64, serial: *mut Serial, cspace: *mut CSpace) -> ! {
    loop {
        let mut badge = 0;

        let msg = unsafe { seL4_Recv(endpoint, &mut badge) };

        let label = unsafe { seL4_MessageInfo_get_label(&msg) };

        if badge & IRQ_EP_BADGE != 0 {
            unsafe { sos_handle_irq_notification(&badge) }
        } else if label == seL4_Fault_NullFault {
            handle_syscall(badge, unsafe { seL4_MessageInfo_get_length(&msg) as u32 } - 1, serial, cspace);
        } else {
            unsafe { debug_dump(); }
        }
    }
}

fn handle_syscall(_badge: u64, num_args: u32, serial: *mut Serial, cspace: *mut CSpace) {
    let reply = unsafe { cspace_alloc_slot(cspace) };
    let syscall_no = unsafe { seL4_GetMR(0) };
    unsafe { cspace_save_reply_cap(cspace, reply)}

    match syscall_no {
        0 => unsafe {// run syscall 0

            let reply_msg = seL4_MessageInfo_new(0, 0, 0, 1);
            seL4_SetMR(0, 0);
            seL4_Send(reply, reply_msg);
            cspace_free_slot(cspace, reply);

        },
        1 => {}, // intentionally not replying to syscall 1
        2 => {
            let foo = "rusts .a build uses non-shared-mem IPC for sos_write impl";
            unsafe { serial_send(serial, foo.as_ptr(), foo.bytes().len()) };
            let reply_msg = unsafe { seL4_MessageInfo_new(0,0,0,1) };
            unsafe { seL4_SetMR(0, 0) };
            unsafe { seL4_Send(reply, reply_msg) };
            unsafe { cspace_free_slot(cspace, reply) };
        }, // print out with serial send
        _ => unsafe { debug_dump(); }
    }
}
