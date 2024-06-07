#![no_std]

extern crate alloc;

use core::cell::{OnceCell, RefCell};

use alloc::vec;
use alloc::vec::Vec;

#[cfg(not(test))]
use panic_halt as _;

use cortex_m as _;
use cortex_m::interrupt::Mutex;
use embedded_alloc::Heap;

type GlobalData<T> = Mutex<RefCell<OnceCell<T>>>;

macro_rules! forward {
    ($data:ident.$method:ident($($arg:expr),*)) => {
        cortex_m::interrupt::free(|cs| $data.borrow(cs).borrow_mut().get_mut().unwrap().$method($($arg),*))
    };
}

#[global_allocator]
static HEAP: Heap = Heap::empty();

static APP_DATA: GlobalData<AppData> = Mutex::new(RefCell::new(OnceCell::new()));

#[derive(Debug)]
pub struct AppData {
    buffer: Vec<u8>,
    state: u8,
}

#[no_mangle]
pub unsafe extern "C" fn tlm_init() {
    use core::mem::MaybeUninit;
    const HEAP_SIZE: usize = 1024;
    static mut HEAP_MEM: [MaybeUninit<u8>; HEAP_SIZE] = [MaybeUninit::uninit(); HEAP_SIZE];
    HEAP.init(HEAP_MEM.as_ptr() as usize, HEAP_SIZE);
    cortex_m::interrupt::free(|cs| {
        APP_DATA
            .borrow(cs)
            .borrow()
            .set(AppData {
                buffer: vec![1, 2, 3, 4, 5],
                state: 0,
            })
            .unwrap();
    });
}

#[no_mangle]
pub unsafe extern "C" fn tlm_app_data_next() -> u8 {
    forward!(APP_DATA.next())
}

impl AppData {
    fn next(&mut self) -> u8 {
        let element = self.buffer[self.state as usize];
        self.state += 1;
        element
    }
}
