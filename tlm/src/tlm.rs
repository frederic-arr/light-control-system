use core::ptr::addr_of;

#[derive(Debug, Clone, Copy)]
pub struct Intersection {
    state: State,
    state_tick: u32,
    sync_tick: u32,
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
    /// Vernier->Genève, Genève->Vernier, Genève->Meyrin
    CycleA0(bool),
    /// Vernier->Genève, Vernier->Meyrin
    CycleA1(bool),
    /// Genève->Meyrin, Meyrin->Genève
    CycleA2(bool),
    /// Vernier->Genève, Genève->Vernier, Pedestrian
    CycleB0,
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
            state: Active(CycleA0(false)),
            state_tick: 0,
            sync_tick: 0,
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
            Active(CycleA0(_)) => [Go, Wait, Go, Go, Wait, Wait, Wait, warn],
            Transition(Clear, CycleA0(_), CycleA1(_)) => {
                [Go, Wait, Stop, Stop, Wait, Wait, Wait, warn]
            }
            Transition(Allow, CycleA0(_), CycleA1(_)) => {
                [Go, Ready, Wait, Wait, Wait, Wait, Wait, warn]
            }
            Active(CycleA1(_)) => [Go, Go, Wait, Wait, Wait, Wait, Wait, warn],
            Transition(Clear, CycleA1(_), CycleA2(_)) => {
                [Stop, Stop, Wait, Wait, Wait, Wait, Wait, warn]
            }
            Transition(Allow, CycleA1(_), CycleA2(_)) => {
                [Wait, Wait, Wait, Ready, Ready, Wait, Go, warn]
            }
            Active(CycleA2(_)) => [Wait, Wait, Wait, Go, Go, Wait, Go, warn],

            Transition(Clear, CycleA2(_), CycleA0(_)) => [
                Wait,
                Wait,
                Wait,
                Go,
                Stop,
                Wait,
                alternate(self.sync_tick, 500, Go),
                warn,
            ],
            Transition(Allow, CycleA2(_), CycleA0(_)) => {
                [Ready, Wait, Ready, Go, Wait, Wait, Wait, warn]
            }

            Transition(Clear, CycleA2(_), CycleB0) => [
                Wait,
                Wait,
                Wait,
                Stop,
                Stop,
                Wait,
                alternate(self.sync_tick, 500, Go),
                warn,
            ],
            Transition(Allow, CycleA2(_), CycleB0) => {
                [Ready, Wait, Ready, Wait, Wait, Wait, Wait, warn]
            }

            Active(CycleB0) => [Go, Wait, Go, Wait, Wait, Go, Wait, warn],
            Transition(Clear, CycleB0, CycleA0(_)) => [
                Go,
                Wait,
                Go,
                Wait,
                Wait,
                alternate(self.sync_tick, 500, Go),
                Wait,
                warn,
            ],
            Transition(Allow, CycleB0, CycleA0(_)) => [Go, Wait, Go, Ready, Wait, Wait, Wait, warn],

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
            Active(CycleA0(_)) if tick >= 13_000 => self.next(),

            // Secondary road for 6s
            Active(CycleA1(_) | CycleA2(_)) if tick >= 6_000 => self.next(),

            // Pedestrian crossing for 16s
            Active(CycleB0) if tick >= 16_000 => self.next(),

            // Main road transition for 4s
            Transition(Clear, CycleA0(_), _) if tick >= 6_000 => self.next(),

            // Pedestrian transition for 5s
            Transition(Clear, CycleB0, _) if tick >= 5_000 => self.next(),

            // Pedestrian secondary for 8s
            Transition(Clear, CycleA2(_), _) if tick >= 8_000 => self.next(),

            // Secondary road transition for 4s to clear and 1.5s to allow
            Transition(Clear, _, _) if tick >= 4_000 => self.next(),
            Transition(Allow, _, _) if tick >= 1_500 => self.next(),

            _ => {}
        }
    }

    pub fn next(&mut self) {
        use Cycle::*;
        use Phase::*;
        use State::*;

        self.state_tick = 0;
        self.state = match self.state {
            Active(s @ CycleA0(v)) => Transition(Clear, s, CycleA1(v)),
            Active(s @ CycleA1(v)) => Transition(Clear, s, CycleA2(v)),
            Active(s @ CycleA2(false)) => Transition(Clear, s, CycleA0(false)),
            Active(s @ CycleA2(true)) => Transition(Clear, s, CycleB0),
            Active(s @ CycleB0) => Transition(Clear, s, CycleA0(false)),

            Transition(Clear, a, b) => Transition(Allow, a, b),
            Transition(Allow, _, b) => Active(b),
        }
    }
}
