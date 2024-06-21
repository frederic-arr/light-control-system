use core::ptr::addr_of;

#[derive(Debug, Clone, Copy)]
pub struct Intersection {
    state: State,
    state_tick: u32,
    sync_tick: u32,
    request: Option<Request>,
}

#[derive(Debug, Clone, Copy)]
enum Phase {
    Clear,
    Allow,
}

#[derive(Debug, Clone, Copy)]
enum State {
    Transition(Phase, Cycle, Cycle),
    Active(Cycle),
}

#[derive(Debug, Clone, Copy)]
enum Cycle {
    Blocked,
    Manual,

    /// Vernier->Genève, Genève->Vernier, Genève->Meyrin
    CycleA0,
    /// Vernier->Genève, Vernier->Meyrin
    CycleA1,
    /// Genève->Meyrin, Meyrin->Genève
    CycleA2,
    /// Vernier->Genève, Genève->Vernier, Pedestrian
    CycleB0,
}

#[derive(Debug, Clone, Copy, PartialEq, Eq)]
enum Request {
    Pedestrian,
    Block,
    Unblock,
    Maintenance,
}

#[derive(Debug, Clone, Copy)]
#[repr(C)]
pub enum Sprite {
    Wait,
    Ready,
    Go,
    Stop,
    Off,
    Warn,
}

fn alternate(tick: u32, cycle: u32, sprite: Sprite) -> Sprite {
    if tick % (cycle * 2) < cycle {
        sprite
    } else {
        Sprite::Off
    }
}

impl Intersection {
    pub fn new() -> Self {
        use Cycle::*;
        use State::*;

        Self {
            state: Active(Blocked),
            request: Some(Request::Unblock),
            state_tick: 0,
            sync_tick: 0,
        }
    }

    pub fn has_pedestrian_request(&self) -> bool {
        match self.request {
            Some(Request::Pedestrian) => true,
            _ => false,
        }
    }

    pub fn request_pedestrian(&mut self) {
        use Cycle::*;
        use State::*;

        match (self.state, self.request) {
            (Transition(_, _, CycleB0) | Active(CycleB0), None) => {}
            (Active(Blocked | Manual), _) => {}
            (Transition(_, _, Blocked | Manual), _) => {}
            (_, None) => self.request = Some(Request::Pedestrian),
            _ => {}
        }
    }

    pub fn request_unblock(&mut self) {
        match (self.state, self.request) {
            (_, None) => self.request = Some(Request::Unblock),
            _ => {}
        }
    }

    pub fn request_block(&mut self) {
        self.request = Some(Request::Block);
    }

    pub fn request_maintenance(&mut self) {
        match (self.state, self.request) {
            (_, None | Some(Request::Pedestrian)) => self.request = Some(Request::Maintenance),
            _ => {}
        }
    }

    pub fn sprites(&self) -> *const [Sprite; 8] {
        use Cycle::*;
        use Phase::*;
        use Sprite::*;
        use State::*;

        static mut DATA: [Sprite; 8] = [Wait; 8];

        let warn = alternate(self.sync_tick, 750, Warn);
        let data = match self.state {
            Active(Manual) => [warn, warn, warn, warn, warn, Off, Off, warn],
            Active(Blocked) | Transition(Clear, Manual, Blocked) => {
                [Wait, Wait, Wait, Wait, Wait, Wait, Wait, warn]
            }
            Transition(Clear, Blocked, CycleA0) => [Wait, Wait, Wait, Wait, Wait, Wait, Wait, warn],
            Transition(Allow, Blocked, CycleA0) => {
                [Ready, Wait, Ready, Ready, Wait, Wait, Wait, warn]
            }
            Transition(Clear, CycleA1, Blocked) => [Stop, Stop, Wait, Wait, Wait, Wait, Wait, warn],

            Active(CycleA0) => [Go, Wait, Go, Go, Wait, Wait, Wait, warn],
            Transition(Clear, CycleA0, CycleA1) => [Go, Wait, Stop, Stop, Wait, Wait, Wait, warn],
            Transition(Allow, CycleA0, CycleA1) => [Go, Ready, Wait, Wait, Wait, Wait, Wait, warn],
            Active(CycleA1) => [Go, Go, Wait, Wait, Wait, Wait, Wait, warn],
            Transition(Clear, CycleA1, CycleA2) => [Stop, Stop, Wait, Wait, Wait, Wait, Wait, warn],
            Transition(Allow, CycleA1, CycleA2) => [Wait, Wait, Wait, Ready, Ready, Wait, Go, warn],
            Active(CycleA2) => [Wait, Wait, Wait, Go, Go, Wait, Go, warn],

            Transition(Clear, CycleA2, CycleA0) => [
                Wait,
                Wait,
                Wait,
                Go,
                Stop,
                Wait,
                alternate(self.sync_tick, 500, Go),
                warn,
            ],
            Transition(Allow, CycleA2, CycleA0) => [Ready, Wait, Ready, Go, Wait, Wait, Wait, warn],

            Transition(Clear, CycleA2, CycleB0) => [
                Wait,
                Wait,
                Wait,
                Stop,
                Stop,
                Wait,
                alternate(self.sync_tick, 500, Go),
                warn,
            ],
            Transition(Allow, CycleA2, CycleB0) => [Ready, Wait, Ready, Wait, Wait, Go, Wait, warn],

            Active(CycleB0) => [Go, Wait, Go, Wait, Wait, Go, Wait, warn],
            Transition(Clear, CycleB0, CycleA0) => [
                Go,
                Wait,
                Go,
                Wait,
                Wait,
                alternate(self.sync_tick, 500, Go),
                Wait,
                warn,
            ],
            Transition(Allow, CycleB0, CycleA0) => [Go, Wait, Go, Ready, Wait, Wait, Wait, warn],

            _ => unreachable!("Invalid state: {:?}", self.state),
        };

        unsafe {
            DATA.copy_from_slice(&data);
            addr_of!(DATA)
        }
    }

    pub fn tick(&mut self, by: u32) {
        use Cycle::*;
        use Phase::*;
        use State::*;

        self.state_tick += by;
        self.sync_tick += by;

        let tick = self.state_tick;
        match self.state {
            // Main road for 12s
            Active(CycleA0) if tick >= 13_000 => self.next(),

            // Secondary road for 6s
            Active(CycleA1 | CycleA2) if tick >= 6_000 => self.next(),

            // Pedestrian crossing for 16s
            Active(CycleB0) if tick >= 16_000 => self.next(),

            // Main road transition for 4s
            Transition(Clear, CycleA0, _) if tick >= 6_000 => self.next(),

            // Pedestrian transition for 5s
            Transition(Clear, CycleB0, _) if tick >= 5_000 => self.next(),

            // Pedestrian secondary for 8s
            Transition(Clear, CycleA2, _) if tick >= 8_000 => self.next(),

            // Secondary road transition for 4s to clear and 1.5s to allow
            Transition(Clear, _, _) if tick >= 4_000 => self.next(),
            Transition(Allow, _, _) if tick >= 1_500 => self.next(),

            Active(Blocked) if tick >= 10_000 && self.request == Some(Request::Unblock) => {
                self.next()
            }

            Active(Manual) if tick >= 10_000 && self.request == Some(Request::Unblock) => {
                self.next()
            }

            Active(Blocked) if tick >= 10_000 && self.request == Some(Request::Maintenance) => {
                self.next()
            }

            _ => {}
        }
    }

    pub fn next(&mut self) {
        use Cycle::*;
        use Phase::*;
        use State::*;

        self.state_tick = 0;
        (self.request, self.state) = match (self.request, self.state) {
            (Some(Request::Unblock), Active(Blocked)) => {
                (None, Transition(Allow, Blocked, CycleA0))
            }
            (r @ Some(Request::Unblock), Active(Manual)) => (r, Transition(Clear, Manual, Blocked)),
            (Some(Request::Maintenance), Active(Blocked)) => (None, Active(Manual)),
            (Some(Request::Block), Active(CycleA1)) => (None, Transition(Clear, CycleA1, Blocked)),
            (r @ Some(Request::Maintenance), Active(CycleA1)) => {
                (r, Transition(Clear, CycleA1, Blocked))
            }
            (r @ Some(Request::Pedestrian), Active(CycleA2)) => {
                (r, Transition(Clear, CycleA2, CycleB0))
            }
            (Some(Request::Pedestrian), Transition(Clear, CycleA2, CycleB0)) => {
                (None, Transition(Allow, CycleA2, CycleB0))
            }

            (r, Active(CycleA0)) => (r, Transition(Clear, CycleA0, CycleA1)),
            (r, Active(CycleA1)) => (r, Transition(Clear, CycleA1, CycleA2)),
            (r, Active(CycleA2)) => (r, Transition(Clear, CycleA2, CycleA0)),
            (r, Active(CycleB0)) => (r, Transition(Clear, CycleB0, CycleA0)),
            (r, Transition(Clear, _, Blocked)) => (r, Active(Blocked)),
            (r, Transition(Clear, a, b)) => (r, Transition(Allow, a, b)),
            (r, Transition(Allow, _, b)) => (r, Active(b)),
            _ => (self.request, self.state),
        };
    }
}
