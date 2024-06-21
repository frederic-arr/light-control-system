#![no_std]

mod tlm;

extern crate alloc;

#[cfg(not(test))]
use panic_halt as _;

use core::cell::{OnceCell, RefCell};
use cortex_m::interrupt::Mutex;
use embedded_alloc::Heap;
use tlm::{Intersection, Sprite};

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
pub unsafe extern "C" fn tlm_intersection_new() -> alloc::boxed::Box<Intersection> {
    alloc::boxed::Box::new(Intersection::new())
}

#[no_mangle]
pub unsafe extern "C" fn tlm_intersection_tick(intersection: &mut Intersection, by: u32) {
    intersection.tick(by);
}

#[no_mangle]
pub unsafe extern "C" fn tlm_intersection_get_lights(
    intersection: &Intersection,
) -> *const [Sprite; 8] {
    intersection.sprites()
}

#[no_mangle]
pub unsafe extern "C" fn tlm_intersection_request_pedestrian(intersection: &mut Intersection) {
    intersection.request_pedestrian();
}

#[no_mangle]
pub unsafe extern "C" fn tlm_intersection_has_pedestrian_request(
    intersection: &Intersection,
) -> bool {
    intersection.has_pedestrian_request()
}

#[no_mangle]
pub unsafe extern "C" fn tlm_intersection_request_unblock(intersection: &mut Intersection) {
    intersection.request_unblock();
}

#[no_mangle]
pub unsafe extern "C" fn tlm_intersection_request_block(intersection: &mut Intersection) {
    intersection.request_block();
}
