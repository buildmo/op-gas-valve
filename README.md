# Operation Gas Valve — Facilitator Code Repository

Backup code for the OGV 3-day robotics challenge. This repo contains **milestone handout files** organized by team, plus the **final production code** for the completed robot.

**You should never need to give teams the final code directly.** The milestones are designed to be handed out incrementally when a team falls behind a hard deadline, so they can catch up without skipping the learning.

## Table of Contents

- [What's in this repo](#whats-in-this-repo)
- [Milestone handout schedule](#milestone-handout-schedule)
  - [Tank team](#tank-team-milestonestank)
  - [Arm team](#arm-team-milestonesarm)
  - [Pilot team](#pilot-team-milestonespilot)
  - [Combined](#combined-milestonescombined)
- [How to use this during the event](#how-to-use-this-during-the-event)
  - [Before the event](#before-the-event)
  - [During the event](#during-the-event)
  - [Serial command reference](#serial-command-reference)
- [Hardware assumptions](#hardware-assumptions)
- [Software setup](#software-setup)
- [File reference](#file-reference)

---

## What's in this repo

```
pi-fleet/
├── milestones/              Handout code — organized by team
│   ├── tank/                Arduino sketches for the Tank team
│   ├── arm/                 Arduino sketches for the Arm team
│   ├── pilot/               Python GUI scripts for the Pilot team
│   └── combined/            Merged Arduino sketch (Tank + Arm)
│
├── final_arduino/           Complete Arduino sketch (don't hand out)
├── final_gui/               Complete Python GUI (don't hand out)
│
├── setup.sh                 Installs all software on the Pi
├── test_milestones.sh       Verifies all code compiles / parses
├── SETUP.md                 Software installation guide
└── ARCHITECTURE.md          System architecture (give to Integration team)
```

- **`milestones/`** — What you hand to teams. Each team folder has numbered files in the order you'd give them out.
- **`final_arduino/`** and **`final_gui/`** — The finished, working robot code. Keep this as your facilitator reference. Only use it if everything goes sideways on Day 3.
- **`test_milestones.sh`** — Run this before the event to verify every file compiles and parses cleanly on your Pi.

---

## Milestone handout schedule

Each milestone builds on the previous one. Only hand out a file if the team hasn't reached that level on their own by the deadline.

### Tank team (`milestones/tank/`)

| File | When to hand out | What it does |
|---|---|---|
| `0_one_motor/` | Day 1 HR3 — if H-bridge wiring isn't working | Simplest possible: one motor, three commands (F/B/S). Proves wiring is correct before adding the second motor. |
| `1_motor_test/` | Day 1 HR5 — if motors aren't moving | Both motors with Serial Monitor commands (M/N/K/L/O) and printed feedback. Teams can verify wiring and motor direction. |
| `2_serial_control/` | **Day 2 HR3 (hard deadline)** — if tank isn't driving via code | Motor control with serial buffer drain. Ready for GUI integration. |

### Arm team (`milestones/arm/`)

| File | When to hand out | What it does |
|---|---|---|
| `0_one_servo/` | Day 1 HR3 — if servo wiring isn't working | Simplest possible: one servo on pin 10, three commands (+/-/0). Proves servo power and wiring are correct before adding more. |
| `1_servo_test/` | Day 1 HR5 — if servos aren't responding | Interactive servo tester. Select servo 1-4, nudge +/- 5 degrees via Serial Monitor. Verifies wiring and power. |
| `2_serial_control/` | **Day 2 HR3 (hard deadline)** — if arm isn't moving via code | Servo control via serial commands (A/B, C/D, G/H, I/J), angle limits per joint, smooth 2-degree stepping. Ready for GUI integration. |

### Pilot team (`milestones/pilot/`)

| File | When to hand out | What it does |
|---|---|---|
| `0_one_button_gui.py` | Day 1 HR3 — if serial connection isn't working | Simplest possible: one Forward button sending 'M' over serial. Proves Pi-to-Arduino communication works. |
| `1_tank_gui.py` | Day 1 HR5 — if GUI framework isn't up | Minimal tank-only D-pad. Click a button, sends a character over serial. Enough to test tank driving. |
| `1b_gui_framework.py` | Day 1 HR7 — if pilot team finishes tank early | Full window layout with tank D-pad (live) and arm panel (placeholder). Arm buttons print to console but don't send serial yet. Gets the UI structure ready for Day 2. |
| `2_tank_burst_gui.py` | **Day 2 HR3 (hard deadline)** — if burst control isn't working | Tank GUI with burst timers (auto-stop after each click) and speed slider (25-200ms). Much smoother than raw button presses. |
| `3_full_gui.py` | Day 2 HR5 — if full integration is behind | Complete GUI. Tank D-pad + arm controls (+/- for each joint) + separate speed sliders + cleanup on close. This is the mission-day GUI. |

### Combined (`milestones/combined/`)

| File | When to hand out | What it does |
|---|---|---|
| `robot_controller/` | Day 2 HR5 — when Tank + Arm code needs to merge | Single Arduino sketch with motors + servos. Hand to both teams when they need to combine their code onto one Arduino. |

---

## How to use this during the event

### Before the event

1. **Clone this repo** onto your facilitator laptop:
   ```
   git clone git@github.com:buildmo/pi-fleet.git
   ```

2. **Run the test script** on a Pi with the Arduino connected to verify everything compiles:
   ```bash
   bash test_milestones.sh
   ```
   All items should show green checkmarks. To also upload each sketch to the Arduino:
   ```bash
   bash test_milestones.sh --upload
   ```
   Override board or port if needed:
   ```bash
   BOARD=arduino:avr:mega:cpu=atmega2560 PORT=/dev/ttyUSB0 bash test_milestones.sh
   ```

3. **Have the milestone files ready** on a USB stick or shared drive so you can hand them out quickly during the event.

### During the event

1. **Check team progress** against the daily minimum standards in the Facilitator Guide every 30 minutes during build phases.

2. **If a team misses a deadline**, open their folder in `milestones/` and hand them the next numbered file. Tell them:
   - *"Here's working code for [component]. Read through it, understand what it does, upload it, and verify it works. Then move on to the next task."*

3. **Don't hand out files preemptively.** Let teams struggle — the learning is in the struggle. Only intervene at the deadlines listed above.

4. **Day 2 HR3 is the critical deadline.** If Tank or Arm aren't code-controlled by HR3, hand out `2_serial_control/` immediately so integration isn't blocked.

5. **Day 2 HR5 is merge time.** If teams can't combine their Arduino code, hand out `milestones/combined/robot_controller/`.

### Serial command reference

All milestone files use the same serial protocol so they're interchangeable:

| Command | Action | Command | Action |
|---|---|---|---|
| `M` | Tank forward | `A` / `B` | Wrist up / down |
| `N` | Tank backward | `C` / `D` | Elbow up / down |
| `K` | Tank left | `G` / `H` | Arm up / down |
| `L` | Tank right | `I` / `J` | Grip open / close |
| `O` | Tank stop | `a` `c` `g` `i` | Stop individual joint |
| | | `1`-`5` | Set arm step size (degrees) |

---

## Hardware assumptions

These files assume the following pin assignments. If teams wire differently, they'll need to update the pin constants at the top of each sketch.

**Motors (H-bridge L298N):**
| Pin | Function |
|---|---|
| 6 | EN_A — Left motor PWM |
| 4 | IN1 — Left motor direction |
| 3 | IN2 — Left motor direction |
| 7 | IN3 — Right motor direction |
| 8 | IN4 — Right motor direction |
| 5 | EN_B — Right motor PWM |

**Servos:**
| Pin | Joint | Range | Start |
|---|---|---|---|
| 10 | Wrist | 45-135 | 90 |
| 12 | Elbow | 0-90 | 45 |
| 9 | Arm | 70-135 | 90 |
| 11 | Grip | 0-180 | 90 |

**Serial:** 9600 baud over USB (`/dev/ttyACM0` by default).

---

## Software setup

See **[SETUP.md](SETUP.md)** for full installation instructions, or run the setup script on each Pi:

```bash
bash setup.sh
```

This installs Python packages (guizero, pyserial, tkinter) and arduino-cli. See [SETUP.md](SETUP.md) for Arduino IDE desktop app instructions and manual install steps.

---

## File reference

| Path | Description |
|---|---|
| `milestones/tank/0_one_motor/` | Base — Single motor test (F/B/S) |
| `milestones/tank/1_motor_test/` | Day 1 — H-bridge test with Serial Monitor feedback |
| `milestones/tank/2_serial_control/` | Day 2 — Motor control for GUI integration |
| `milestones/arm/0_one_servo/` | Base — Single servo test (+/-/0) |
| `milestones/arm/1_servo_test/` | Day 1 — Interactive servo tester (select + nudge) |
| `milestones/arm/2_serial_control/` | Day 2 — Servo control with limits and stepping |
| `milestones/pilot/0_one_button_gui.py` | Base — One button forward test |
| `milestones/pilot/1_tank_gui.py` | Day 1 — Minimal tank D-pad |
| `milestones/pilot/1b_gui_framework.py` | Day 1 — Full layout with arm placeholders |
| `milestones/pilot/2_tank_burst_gui.py` | Day 2 — Tank GUI with burst timers |
| `milestones/pilot/3_full_gui.py` | Day 2 — Complete tank + arm GUI |
| `milestones/combined/robot_controller/` | Day 2 — Merged Arduino sketch |
| `final_arduino/robot_controller/` | Complete Arduino sketch (facilitator reference) |
| `final_gui/gui.py` | Complete GUI (facilitator reference) |
| `final_gui/main.py` | Config loader + GUI launcher |
| `test_milestones.sh` | Compile + syntax check for all files |
