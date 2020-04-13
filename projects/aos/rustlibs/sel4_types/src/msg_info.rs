use super::primitives::*;
#[repr(C)]
pub struct seL4_MessageInfo {
    words: [u64; 1],
}

impl From<MsgInfo> for seL4_MessageInfo {
    fn from(msg: MsgInfo) -> Self {
        let mut word = 0u64;

        word |= (msg.label as u64 & 0xf_ffff_ffff_ffff_u64) << 12;
        word |= (msg.caps_unwrapped as u64 & 0x7u64) << 9;
        word |= (msg.extra_caps as u64 & 0x3u64) << 7;
        word |= msg.length as u64 & 0x7fu64;
        seL4_MessageInfo {words: [word]}
    }
}

impl From<&MsgInfo> for seL4_MessageInfo {
    fn from(msg: &MsgInfo) -> Self {
        let mut word = 0u64;

        word |= (msg.label as u64 & 0xf_ffff_ffff_ffff_u64) << 12;
        word |= (msg.caps_unwrapped as u64 & 0x7u64) << 9;
        word |= (msg.extra_caps as u64 & 0x3u64) << 7;
        word |= msg.length as u64 & 0x7fu64;
        seL4_MessageInfo {words: [word]}
    }
}

extern "C" {
    fn seL4_Send(ep_cptr: CPtr, msg: seL4_MessageInfo);
    pub fn seL4_Recv(endpoint: CPtr, sender: *mut Word) -> seL4_MessageInfo;
    fn seL4_GetMR(idx: i32) -> Word;
    fn seL4_SetMR(idx: i32, val: Word);
    pub fn seL4_MessageInfo_new(_: Uint64, _: Uint64, _: Uint64, _: Uint64) -> seL4_MessageInfo;

    // 'ere be monsters. Relies on correct interpretation of seL4's msg buff size
    // fn sos_get_ipc_msg_buf() -> *const Word;
}

impl seL4_MessageInfo {
    pub fn label(&self) -> u64 {(self.words[0] & 0xffff_ffff_ffff_f000_u64) >> 12}
    pub fn caps_unwrapped(&self) -> u8 {((self.words[0] & 0xe00u64) >> 9) as u8}
    pub fn extra_caps(&self) -> u8 {((self.words[0] & 0x180u64) >> 7) as u8}
    pub fn length(&self) -> u8 {(self.words[0] & 0x7fu64) as u8}
}

#[derive(Debug)]
pub struct MsgInfo {
    pub label: u64,
    pub caps_unwrapped: u8,
    pub extra_caps: u8,
    pub length: u8
}

impl MsgInfo {

    pub fn new(label: Word, caps_unwrapped: Word, extra_caps: Word, length: Word) -> Self {
        // TODO pull out the safety checks from Trustworthies impl
        // TODO automate the safety checks provided by kernel build system
        // SAFETY arguments do not violate the bit-field specifications of seL4
        unsafe { &seL4_MessageInfo_new(label, caps_unwrapped, extra_caps, length) }.into()
    }

    /// Receive messages
    /// end_point: must be a an endpoint or notification cap
    /// Blocks until a message or notification can be delivered.
    ///
    /// Results in a capability fault if
    ///  * end_point is not a valid capability
    ///  * end_point is not a cap on an endpoint or notification object
    ///  * end_point does not have recieve permissions
    /// TODO: handle capability faults
    ///
    /// Returns a rust-wrapped seL4_MessageInfo_t
    /// Returns an "identification badge" to indicate source of message
    ///  See seL4 manual 6.2.1
    pub fn recieve(end_point: CPtr) -> (MsgInfo, Word) {
        let mut badge = 0;
        let res = unsafe { &seL4_Recv(end_point, &mut badge) };
        (res.into(), badge)
    }

    /// A one-way message
    /// if the capability is an andpoint, blocks until it's delivered
    /// Causes reciever to unblock
    pub fn send(self, reply: CPtr) {
        unsafe { seL4_Send(reply, self.into()) };
    }

    // pub fn get_msg_buf<'a>(&self) -> Result<&'a[Word], &'static str> {
    //     if self.length as usize > MSG_MAX_LEN {
    //         Err("problem! tried getting a msg buf, but MsgInfo length is oversized")
    //     } else {
    //         Ok(Self::msg_buf(self.length as usize))
    //     }
    // }

    pub fn set_mr(self, idx: u8, val: Word) -> Result<Self, Self> {
        if idx < self.length {
            unsafe { seL4_SetMR(idx.into(), val) };
            Ok(self)
        } else {
            Err(self)
        }
    }

    pub fn get_mr(&self, idx: u8) -> Result<Word, &'static str> {
        if idx < self.length {
            Ok(unsafe { seL4_GetMR(idx.into()) })
        } else {
            Err("asked for an out-of-bounds MR")
        }
    }

    // fn msg_buf<'a>(len: usize) -> &'a [Word] {
    //     use core::slice::from_raw_parts;
    //     let msg_array = unsafe { from_raw_parts(sos_get_ipc_msg_buf(), MSG_MAX_LEN) };
    //     &msg_array[0..len]
    // }
}

impl From<&seL4_MessageInfo> for MsgInfo {
    fn from(sel4_msg: &seL4_MessageInfo) -> Self {
        MsgInfo {
            label: sel4_msg.label(),
            caps_unwrapped: sel4_msg.caps_unwrapped(),
            extra_caps: sel4_msg.extra_caps(),
            length: sel4_msg.length(),
        }
    }
}

impl core::fmt::Debug for seL4_MessageInfo {
    fn fmt(&self, f: &mut core::fmt::Formatter<'_>) -> core::fmt::Result {
        write!(f, "{:?}", MsgInfo::from(self))
    }
}
