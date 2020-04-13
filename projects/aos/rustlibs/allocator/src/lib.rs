#![no_std]
#![feature(alloc_error_handler)]

use core::ffi::c_void;
extern crate panic;

#[alloc_error_handler]
fn default_handler(layout: core::alloc::Layout) -> ! {
    panic!("memory allocation of {} bytes failed", layout.size())
}

extern "C" {
    fn malloc(size: isize) -> *mut u8;
    fn free(ptr: *mut c_void);
}

// #[no_mangle]
// pub extern fn __rust_allocate(size: usize, _align: usize) -> *mut u8 {
//     unsafe { malloc(size as isize) as *mut u8 }
// }

// #[no_mangle]
// pub extern fn __rust_deallocate(ptr: *mut u8, _old_size: usize, _align: usize) {
//     unsafe { free(ptr as *mut c_void) }
// }

// #[no_mangle]
// pub extern fn __rust_reallocate(ptr: *mut u8, _old_size: usize, size: usize,
//                                 _align: usize) -> *mut u8 {
//     unsafe {realloc(ptr as *mut c_void, size as isize) as *mut u8 }
// }

// #[no_mangle]
// pub extern fn __rust_reallocate_inplace(_ptr: *mut u8, old_size: usize,
//                                         _size: usize, _align: usize) -> usize {
//     // ???
// }

// #[no_mangle]
// pub extern fn __rust_usable_size(size: usize, _align: usize) -> usize {
//     // ???
// }


use core::alloc::{GlobalAlloc, Layout};

struct MyAllocator;

unsafe impl GlobalAlloc for MyAllocator {
    unsafe fn alloc(&self, layout: Layout) -> *mut u8 {

        // given that usize as isize can panic due to usize::MAX > isize::MAX
        // we must ensure that we return a malloc failure, rather than panicing when
        // layout.size() gives a isize > usize::MAX. We need to do this, as the
        // provided libc API asks for size_t (as opposed to usize_t)
        if layout.size() > isize::MAX as usize {
            return core::ptr::null_mut::<u8>()
        }
        malloc(layout.size() as isize) as *mut u8
    }
    unsafe fn dealloc(&self, ptr: *mut u8, _layout: Layout) { free(ptr as *mut c_void); }
}

#[global_allocator]
static A: MyAllocator = MyAllocator;
