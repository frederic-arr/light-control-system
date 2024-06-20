#![no_std]

mod tlm;

#[cfg(not(test))]
use panic_halt as _;

use core::cell::{OnceCell, RefCell};
use cortex_m::interrupt::Mutex;
use embedded_alloc::Heap;
use tlm::{Intersection, TrafficLight};

type GlobalData<T> = Mutex<RefCell<OnceCell<T>>>;

macro_rules! forward {
    ($data:ident.$method:ident($($arg:expr),*)) => {
        cortex_m::interrupt::free(|cs| $data.borrow(cs).borrow_mut().get_mut().unwrap().$method($($arg),*))
    };
}

#[global_allocator]
static HEAP: Heap = Heap::empty();

static INTERSECTION: GlobalData<Intersection> = Mutex::new(RefCell::new(OnceCell::new()));

#[no_mangle]
pub unsafe extern "C" fn tlm_init() {
    use core::mem::MaybeUninit;
    const HEAP_SIZE: usize = 1024;
    static mut HEAP_MEM: [MaybeUninit<u8>; HEAP_SIZE] = [MaybeUninit::uninit(); HEAP_SIZE];
    HEAP.init(HEAP_MEM.as_ptr() as usize, HEAP_SIZE);
    cortex_m::interrupt::free(|cs| {
        INTERSECTION
            .borrow(cs)
            .borrow()
            .set(Intersection::new())
            .unwrap();
    });
}

#[no_mangle]
pub unsafe extern "C" fn tlm_intersection_tick(by: u32) {
    forward!(INTERSECTION.tick(by))
}

#[no_mangle]
pub unsafe extern "C" fn tlm_intersection_get_lights() -> *const TrafficLight {
    forward!(INTERSECTION.lights()).as_ptr()
}
