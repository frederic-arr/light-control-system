# Light Control System
## Introduction
Project for the course "Embedded Systems" at the University of Applied Sciences Northwestern Switzerland in Geneva (HEPIA). The project consists of a control system for a light system. The control system is implemented in Rust and runs on a microcontroller which is programmed in C. The microcontroller is connected to various devices:
- A display (`ILI9341`) to show the current state of the system
- A capacitive touch sensor (`FT6206`) to interact to emulate the pedestrian request button
- A color sensor (`APDS-9960`) to detect the day/night cycle and put the intersection in the appropriate mode

The board itself is an `LPC1769` microcontroller from NXP.

## Quick Start
The provided release should come with all the pre-compiled libraries needed for the final compilation step. If you want to compile the project from scratch, you will need to install [Rust](https://www.rust-lang.org/tools/install). After installing the required tools, you can run the following commands to compile the project:
```bash
cargo build --release
```	

*Note: You may need to install the required target, which can be done by running `rustup target add thumbv8m.main-none-eabi`.*
*Note 2: You may need to switch to the nightly toolchain by running `rustup default nightly`.*

## Code Structure
- `tlm/` contains the Rust library to manage the control system
  - `src/` contains the source code for the library
    - `lib.rs` is the entry point for the library which exposes the public API with the C FFI
    - `tlm.rs` contains the implementation of the control system. The implementation can be used in pure Rust or exposed to C using the FFI
- `src/` contains the C code for the microcontroller
  - `dev/` contains the device-specific code (named according to the model or the controller)
  - `sprites/` contains the sprites used in the project
  - `sys/` contains the system-specific code (e.g. the startup code)
  - `main_project.c` is the entry point for the C code
  - `tl.h` contains the C FFI interface as well as some utility functions to interact with the control system

## General Purpose Direct Memory Access (GPDMA)
The GPDMA code is partially implemented. Due to the lack of connection to the LE/BE pin, it is however not possible to send data to the display with the correct colors.
Furthermore, the GPDMA code is not yet integrated with the project. The integration (LE/BE pin aside) should be quite straightforward, as the GPDMA protocol for single transfers is already implemented and tested. For multiple transfers, a linked list of "commands" can be kept and consumed on each GPDMA Interrupt to send the next command.

## APDS-9960 (Digital Proximity, Ambient Light, RGB and Gesture Sensor)
Optimally, this device should be interrupt-driven. Unfortunately, the required interrupt pin cannot be used due to technical limitations of the microcontroller (the device pin is connected to P1.27, while the only P0.X and P2.X can recieve interrupts). The device is therefore polled in the main loop which is sufficient for the current use case.

## Sprites
The sprites were created using Adobe XD and exported as PNG files. The sprites are then converted to C code using GIMP.
You can find the original XD files under `sprites.xd` in the root directory.