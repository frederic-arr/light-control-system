#![no_std]

#[cfg(not(test))]
use panic_abort as _;

#[no_mangle]
pub extern "C" fn calling_rust_from_mcu(left: u8, right: u8) -> u8 {
    left + right
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn it_works() {
        let result = calling_rust_from_mcu(2, 2);
        assert_eq!(result, 4);
    }
}
