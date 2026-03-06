# School Guide — Lego-Block Coding Progression

Build a robot controller step by step. Each step adds one new concept with clearly marked `// ═══ NEW IN THIS STEP ═══` blocks. No arrays, no factory functions, no abstraction layers — just code that reads like English.

## Two Tracks

| Arduino Track (upload to Arduino) | GUI Track (run on Raspberry Pi) |
|---|---|
| step1 — One motor | step1 — One button |
| step2 — Two motors (tank drive) | step2 — Tank d-pad |
| step3 — One servo (wrist) | step3 — Burst timer |
| step4 — Four servos | step4 — Arm layout |
| step5 — Arm letter commands | step5 — Full GUI |
| step6 — Full robot combined | |

## Which Arduino pairs with which GUI?

| To test... | Upload this Arduino sketch | Run this GUI |
|---|---|---|
| Tank driving | step2_two_motors.ino | step1, step2, or step3 |
| Arm servos (manual) | step3 or step4 or step5 | Serial Monitor only |
| Full robot | **step6_robot_combined.ino** | **step5_full_gui.py** |

## Arduino Steps

### Step 1 — One Motor
**What it does:** Drive one motor forward, backward, or stop via Serial Monitor.
**What's new:** Pin setup, `Serial.begin()`, reading serial commands, `analogWrite` for speed.
**Commands:** `F` = forward, `B` = backward, `S` = stop
**Test:** Open Serial Monitor (9600 baud), send `F` — motor spins. Send `S` — motor stops.

### Step 2 — Two Motors (Tank Drive)
**What it does:** Drive both motors together for forward/backward/left/right.
**What's new:** Second motor pins (IN3, IN4, EN_B), named drive functions.
**Commands:** `M` = forward, `N` = backward, `K` = left, `L` = right, `O` = stop
**Test:** Send `M` — both motors forward. Send `K` — robot spins left.

### Step 3 — One Servo (Wrist)
**What it does:** Move the wrist servo up/down with angle tracking.
**What's new:** `#include <Servo.h>`, `Servo` object, angle limits (45°–135°).
**Commands:** `+` = nudge up 5°, `-` = nudge down 5°, `0` = center (90°)
**Test:** Send `+` several times — servo sweeps up. Send `0` — returns to center.

### Step 4 — Four Servos
**What it does:** Control all four arm joints by selecting + nudging.
**What's new:** Three more servo objects (elbow, arm, grip), each with own limits and start angle.
**Commands:** `1`–`4` = select servo, `+`/`-` = nudge, `0` = center all
**Test:** Send `2` then `+` — elbow moves up. Send `0` — all servos center.

### Step 5 — Arm Letter Commands
**What it does:** Continuous movement with direction variables and smooth millis() stepping.
**What's new:** Direction variables (wristDir, elbowDir, etc.), `millis()` timer, step size control.
**Commands:** `A`/`B`/`a` = wrist up/down/stop, `C`/`D`/`c` = elbow, `G`/`H`/`g` = arm, `I`/`J`/`i` = grip, `1`–`5` = step degrees
**Test:** Send `A` — wrist moves continuously. Send `a` — wrist stops. Send `5` then `A` — wrist moves faster.

### Step 6 — Full Robot Combined
**What it does:** Tank drive + arm control in one sketch. This is the final robot code.
**What's new:** Motor functions merged in, serial buffer drain, no more Serial.println.
**Commands:** All tank commands (M/N/K/L/O) + all arm commands (A/B/a, C/D/c, G/H/g, I/J/i, 1–5)
**Test:** Pair with `step5_full_gui.py` — full robot control from the GUI.

## GUI Steps

### Step 1 — One Button
**What it does:** A single "Forward" button that sends `M` to the Arduino.
**What's new:** `guizero` App, `serial.Serial`, `PushButton` with a command function.
**Test:** Click Forward — robot drives forward. (Close window to stop.)

### Step 2 — Tank D-Pad
**What it does:** 5-button d-pad for all tank directions + stop.
**What's new:** Grid layout, one named function per button, `on_close` cleanup.
**Test:** Click each direction — robot responds. Close window — robot stops.

### Step 3 — Burst Timer
**What it does:** Each click sends a short burst (start → wait → stop) instead of staying on forever.
**What's new:** `app.after()` timers, per-direction fire/stop functions, tank speed slider (25–200ms).
**Test:** Click Forward — robot moves briefly then stops. Drag slider left — shorter bursts.

### Step 4 — Arm Layout (Placeholders)
**What it does:** Side-by-side panels with tank controls (working) and arm buttons (print to terminal).
**What's new:** `Box` with `layout="grid"` for side-by-side panels, arm button placeholders.
**Test:** Tank buttons still work. Arm buttons print to terminal (not wired to serial yet).

### Step 5 — Full GUI
**What it does:** Complete robot control — tank burst + arm burst + sliders. Same behavior as `final_gui`.
**What's new:** Arm burst functions wired to real serial commands (50ms burst), arm step slider (1–5 deg).
**Test:** Pair with `step6_robot_combined.ino` — everything works together.

## Hardware Checklist

### Pins
| Component | Pin |
|---|---|
| Left motor PWM (EN_A) | 6 |
| Left motor dir (IN1) | 4 |
| Left motor dir (IN2) | 3 |
| Right motor PWM (EN_B) | 5 |
| Right motor dir (IN3) | 7 |
| Right motor dir (IN4) | 8 |
| Wrist servo | 10 |
| Elbow servo | 12 |
| Arm servo | 9 |
| Grip servo | 11 |

### Servo Limits
| Joint | Min | Max | Start |
|---|---|---|---|
| Wrist | 45° | 135° | 90° |
| Elbow | 0° | 90° | 45° |
| Arm | 70° | 135° | 90° |
| Grip | 0° | 180° | 90° |

### Power
- Motors: external power to L298N (do NOT power from Arduino 5V)
- Servos: external 5V supply recommended for 4 servos
- Arduino: USB from Raspberry Pi

### Serial
- Baud rate: 9600
- Port: `/dev/ttyACM0` (default on Raspberry Pi)
- Motor speeds: left=200, right=220 (PWM 0–255)
