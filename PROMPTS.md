# AI Prompt Playbook for Facilitators

> **How to use this document:** When a team misses a milestone deadline, find the matching phase below and hand them the prompt to paste into Claude. They'll learn by prompting AI and understanding the generated output — don't just hand them raw code.

## Table of Contents

- [Phase 0: Base (Day 1 HR3)](#phase-0-base-day-1-hr3)
  - [Tank Team — One Motor Test](#tank-team--one-motor-test)
  - [Arm Team — One Servo Test](#arm-team--one-servo-test)
  - [Pilot Team — One Button GUI](#pilot-team--one-button-gui)
- [Phase 1: Day 1 HR5](#phase-1-day-1-hr5)
  - [Tank Team — Two Motor Direction Test](#tank-team--two-motor-direction-test)
  - [Arm Team — Four Servo Tester](#arm-team--four-servo-tester)
  - [Pilot Team — Tank D-Pad GUI](#pilot-team--tank-d-pad-gui)
- [Phase 1b: Day 1 HR7](#phase-1b-day-1-hr7)
  - [Pilot Team — Add Arm Placeholder Buttons](#pilot-team--add-arm-placeholder-buttons)
- [Phase 2: Day 2 HR3 (Hard Deadline)](#phase-2-day-2-hr3-hard-deadline)
  - [Tank Team — Serial-Ready Tank Code](#tank-team--serial-ready-tank-code)
  - [Arm Team — Serial-Ready Arm Code](#arm-team--serial-ready-arm-code)
  - [Pilot Team — Tank Burst GUI](#pilot-team--tank-burst-gui)
- [Phase 3: Day 2 HR5 (Integration)](#phase-3-day-2-hr5-integration)
  - [Combined — Merge Tank + Arm Arduino Code](#combined--merge-tank--arm-arduino-code)
  - [Pilot Team — Full GUI with Tank + Arm + Sliders](#pilot-team--full-gui-with-tank--arm--sliders)
- [Quick Reference: Which Prompt to Give](#quick-reference-which-prompt-to-give)

---

## Phase 0: Base (Day 1 HR3)

These prompts get each team's first proof-of-life working.

---

### Tank Team — One Motor Test

> **Hand this out if:** H-bridge wiring is done but they can't get a motor spinning by HR3.

```
Write an Arduino sketch that controls ONE DC motor through an L298N H-bridge.

Hardware:
- Arduino Uno
- L298N H-bridge module
- Left motor only (right motor not wired yet)
- Pins: EN_A = 6 (PWM enable), IN1 = 4 (direction), IN2 = 3 (direction)
- Motor speed: PWM value 200 (out of 255)

Serial interface:
- 9600 baud
- Read single-character commands from Serial Monitor
- 'F' = Forward (IN1=LOW, IN2=HIGH, analogWrite EN_A to 200)
- 'B' = Backward (IN1=HIGH, IN2=LOW, analogWrite EN_A to 200)
- 'S' = Stop (IN1=LOW, IN2=LOW, analogWrite EN_A to 0)
- Print the current state to Serial Monitor after each command (e.g. ">> Forward")

Structure:
- setup(): set pin modes, start Serial at 9600, print a ready message
- loop(): check Serial.available(), read one char, switch on F/B/S

Keep it simple — no classes, no libraries beyond Serial. This is a basic wiring test.
```

**After generating, tell them to test:**
1. Upload sketch, open Serial Monitor at 9600 baud
2. Type `F` — motor spins one direction
3. Type `B` — motor spins the other direction
4. Type `S` — motor stops
5. If motor doesn't spin, check EN_A jumper is removed and wired to pin 6

---

### Arm Team — One Servo Test

> **Hand this out if:** Servo wiring is done but they can't get a servo moving by HR3.

```
Write an Arduino sketch that controls ONE servo motor for testing.

Hardware:
- Arduino Uno
- SG90 servo on pin 10
- IMPORTANT: Power the servo from the buck converter 5V, NOT from the Arduino 5V pin (Arduino can't supply enough current)

Serial interface:
- 9600 baud
- Read single-character commands from Serial Monitor
- '+' = Move up 5 degrees (cap at 180)
- '-' = Move down 5 degrees (cap at 0)
- '0' = Center to 90 degrees
- Print the current angle after each command

Requirements:
- Use the Servo library (#include <Servo.h>)
- Start at 90 degrees (center position)
- Constrain angle between 0 and 180
- Print "Angle: XX" after each move so they can see the position

Keep it simple — one servo, three commands, angle tracking.
```

**After generating, tell them to test:**
1. Upload sketch, open Serial Monitor at 9600 baud
2. Type `+` several times — servo sweeps one direction
3. Type `-` several times — servo sweeps back
4. Type `0` — servo returns to center
5. If servo jitters or doesn't move, check power supply (NOT from Arduino 5V)

---

### Pilot Team — One Button GUI

> **Hand this out if:** Pi is set up but they haven't gotten serial communication working by HR3.

```
Write a Python GUI that sends a single serial command to an Arduino when you click a button.

Requirements:
- Use the guizero library (NOT tkinter directly)
- Import: from guizero import App, PushButton
- Import: import serial
- Serial port: /dev/ttyACM0 at 9600 baud
- After opening serial, wait 2 seconds (time.sleep(2)) for Arduino to reset

GUI:
- App title: "One Button Test", size 300x200
- One PushButton labeled "Forward"
- When clicked, send the byte b"M" over serial

Structure:
- Open serial connection at top level
- Create App and PushButton
- Button command calls a function that does ser.write(b"M")
- app.display() at the end

Keep it minimal — this is just to prove the Pi can talk to the Arduino over USB serial.
```

**After generating, tell them to test:**
1. Plug Arduino (running tank motor test) into Pi via USB
2. Run `python3 one_button_gui.py`
3. Click "Forward" — motor should spin
4. If nothing happens: check `ls /dev/ttyACM*` to verify port exists

---

## Phase 1: Day 1 HR5

These prompts expand each subsystem to full capability.

---

### Tank Team — Two Motor Direction Test

> **Hand this out if:** One motor works but they need both motors with directional control by HR5.

```
Write an Arduino sketch that controls TWO DC motors through an L298N H-bridge with directional commands.

Hardware:
- Arduino Uno, L298N H-bridge
- Left motor: EN_A = 6 (PWM), IN1 = 4, IN2 = 3
- Right motor: EN_B = 5 (PWM), IN3 = 7, IN4 = 8
- Left motor speed: 200, Right motor speed: 220 (right is slightly faster to compensate for drift)

Serial commands (9600 baud):
- 'M' = Forward — both motors forward (Left: IN1=LOW, IN2=HIGH; Right: IN3=LOW, IN4=HIGH)
- 'N' = Backward — both motors backward (Left: IN1=HIGH, IN2=LOW; Right: IN3=HIGH, IN4=LOW)
- 'K' = Turn left — left motor forward, right motor backward
- 'L' = Turn right — left motor backward, right motor forward
- 'O' = Stop — all direction pins LOW, both PWM to 0

Structure:
- Define named constants for all pins and speeds at the top
- setup(): set all 6 pins as OUTPUT, start Serial at 9600
- loop(): check Serial.available(), read char, switch on M/N/K/L/O
- Create helper functions: driveForward(), driveBackward(), driveLeft(), driveRight(), stopMotors()
- Print feedback like ">> Forward" after each command

Keep the helper functions clean — each one sets all 4 direction pins and both PWM values.
```

**After generating, tell them to test:**
1. Upload, open Serial Monitor
2. `M` — both wheels forward, `N` — both backward
3. `K` — spins left, `L` — spins right, `O` — stop
4. If it veers when going "straight", adjust the two speed constants

---

### Arm Team — Four Servo Tester

> **Hand this out if:** One servo works but they need to test all four joints by HR5.

```
Write an Arduino sketch that lets you interactively test 4 servo motors.

Hardware:
- Arduino Uno with Servo library
- 4 servos (all powered from buck converter, NOT Arduino 5V):
  - Servo 1: Wrist on pin 10
  - Servo 2: Elbow on pin 12
  - Servo 3: Arm on pin 9
  - Servo 4: Grip on pin 11

Serial interface (9600 baud):
- '1' = Select servo 1 (Wrist)
- '2' = Select servo 2 (Elbow)
- '3' = Select servo 3 (Arm)
- '4' = Select servo 4 (Grip)
- '+' = Move selected servo up 5 degrees (cap at 180)
- '-' = Move selected servo down 5 degrees (cap at 0)
- '0' = Center ALL servos to 90 degrees

Requirements:
- Use arrays to store servo objects, angles, pin numbers, and joint names
- Track each servo's angle independently (all start at 90)
- When a servo is selected, print: "Selected: [name] (pin [num]) at [angle] deg"
- When moved, print: "[name]: [angle] deg"
- Constrain all angles between 0 and 180

This is a testing tool — the team will use it to find the safe range of motion for each joint before setting final limits.
```

**After generating, tell them to test:**
1. Upload, open Serial Monitor
2. Type `1` to select Wrist, then `+`/`-` to sweep it — note where it binds mechanically
3. Repeat for `2` (Elbow), `3` (Arm), `4` (Grip)
4. Type `0` to re-center everything
5. Write down the safe min/max angle for each joint — you'll need these on Day 2

---

### Pilot Team — Tank D-Pad GUI

> **Hand this out if:** One-button test works but they need a proper driving interface by HR5.

```
Write a Python GUI with a D-pad layout for controlling a tank robot over serial.

Requirements:
- Use guizero library (from guizero import App, PushButton, Box)
- Serial: /dev/ttyACM0 at 9600 baud
- Wait 2 seconds after connecting for Arduino reset

GUI layout:
- App title: "Tank Controller", size 400x400
- D-pad using a Box with grid layout:
  - Row 0, Col 1: "Forward" button → sends 'M'
  - Row 1, Col 0: "Left" button → sends 'K'
  - Row 1, Col 1: "Stop" button → sends 'O' (give it a red background like #ff6666)
  - Row 1, Col 2: "Right" button → sends 'L'
  - Row 2, Col 1: "Backward" button → sends 'N'

Helper function:
- send(char): encodes the character and writes to serial (ser.write(char.encode()))

Cleanup:
- When the window closes (app.when_closed), send 'O' (stop motors), close serial, then destroy the app

Keep it clean — one send() helper, grid layout for the D-pad, proper cleanup on exit.
```

**After generating, tell them to test:**
1. Connect Pi to Arduino running the two-motor sketch
2. Run the GUI — click each direction button
3. Forward/Backward/Left/Right should all work, Stop should halt immediately
4. Close the window — motors should stop (cleanup handler)

---

## Phase 1b: Day 1 HR7

---

### Pilot Team — Add Arm Placeholder Buttons

> **Hand this out if:** Tank D-pad is working and pilot team has time to prepare the arm layout before Day 2.

```
Expand a tank D-pad GUI to add a side-by-side arm control panel with PLACEHOLDER buttons.

Context: The arm team is still wiring servos on Day 1. We don't have the arm serial protocol yet. Build the layout now so we can wire it up on Day 2.

Requirements:
- Use guizero (from guizero import App, Box, PushButton, Text)
- Serial: /dev/ttyACM0 at 9600 baud
- Wait 2 seconds after connecting for Arduino reset

GUI layout:
- App title: "Op Gas Valve - Robot Controller", size 800x600
- Two side-by-side panels using a grid layout on the app:
  - [0,0]: Tank panel (WORKING — sends real serial commands)
  - [1,0]: Arm panel (PLACEHOLDER — just prints to console)
- Both panels should have a visible border (e.g. #999999)

Tank panel (column 0):
- Title text: "TANK"
- D-pad grid: Forward='M', Left='K', Stop='O' (red #ff6666), Right='L', Backward='N'
- These send real serial commands via ser.write()

Arm panel (column 1):
- Title text: "ARM"
- 4 rows, one per joint: Wrist, Elbow, Arm, Grip
- Each row has a "+" button and a "-" button
- Buttons call a placeholder function that prints: "[ARM PLACEHOLDER] {joint} {direction}"
- Use a factory function that returns a lambda for each button (to capture joint name and direction)
- NO serial commands sent from arm buttons yet

Cleanup:
- on_close: send 'O', close serial, destroy app

The arm panel is purely layout — it will be connected to real commands on Day 2.
```

**After generating, tell them to test:**
1. Run the GUI — you should see tank controls on the left, arm controls on the right
2. Tank buttons should still drive the robot
3. Arm +/- buttons should print placeholder messages to the terminal (not send serial)
4. This layout will be upgraded with real arm commands on Day 2

---

## Phase 2: Day 2 HR3 (Hard Deadline)

These are the integration-ready versions. If a team doesn't have these by HR3, hand them the prompt.

---

### Tank Team — Serial-Ready Tank Code

> **Hand this out if:** Tank team doesn't have clean, GUI-ready motor code by Day 2 HR3.

```
Write an Arduino sketch for tank motor control that is optimized for GUI integration (not Serial Monitor).

Hardware:
- Arduino Uno, L298N H-bridge
- Left motor: EN_A = 6, IN1 = 4, IN2 = 3 (speed: 200)
- Right motor: EN_B = 5, IN3 = 7, IN4 = 8 (speed: 220)

Serial commands (9600 baud): M=forward, N=backward, K=left, L=right, O=stop

CRITICAL REQUIREMENT — Serial buffer drain:
Instead of reading one byte at a time, drain the entire serial buffer and only act on the LAST byte:
```
char cmd = 0;
while (Serial.available() > 0) {
  cmd = Serial.read();
}
if (cmd) {
  handleCommand(cmd);
}
```
This prevents lag when the GUI sends commands faster than the Arduino processes them.

Other requirements:
- Do NOT use Serial.println() for feedback — it clutters the serial line when a GUI is connected
- Use a handleCommand(char cmd) function with a switch statement
- Same helper functions as before: driveForward(), driveBackward(), driveLeft(), driveRight(), stopMotors()

Two changes from the test version: (1) buffer drain loop, (2) no print statements.
```

**After generating, tell them to test:**
1. Upload and test with Serial Monitor — commands should still work but no feedback text
2. Send multiple characters quickly (e.g. `MMMMM`) — robot should respond to only the latest
3. This is ready for the GUI to connect to

---

### Arm Team — Serial-Ready Arm Code

> **Hand this out if:** Arm team doesn't have direction-based servo control with limits by Day 2 HR3.

```
Write an Arduino sketch for 4-servo arm control with smooth stepping, angle limits, and direction-based commands.

Hardware:
- Arduino Uno with Servo library
- 4 servos (powered from buck converter, NOT Arduino):

| Index | Joint  | Pin | Min Angle | Max Angle | Start Angle |
|-------|--------|-----|-----------|-----------|-------------|
| 0     | Wrist  | 10  | 45        | 135       | 90          |
| 1     | Elbow  | 12  | 0         | 90        | 45          |
| 2     | Arm    | 9   | 70        | 135       | 90          |
| 3     | Grip   | 11  | 0         | 180       | 90          |

Movement model:
- Each servo has a direction: -1 (moving down), 0 (idle), +1 (moving up)
- A tickServos() function runs every 20ms (use millis(), not delay)
- Each tick, every servo with a non-zero direction moves by stepDeg degrees toward its limit
- Default stepDeg = 2 degrees per tick
- Constrain angles within each servo's individual min/max limits

Serial commands (9600 baud):
- START moving (uppercase):
  A = Wrist up (+1), B = Wrist down (-1)
  C = Elbow up (+1), D = Elbow down (-1)
  G = Arm up (+1),   H = Arm down (-1)
  I = Grip open (+1), J = Grip close (-1)
- STOP individual joints (lowercase):
  a = Wrist stop, c = Elbow stop, g = Arm stop, i = Grip stop
- Step size (digits):
  '1' through '5' = set stepDeg to 1-5 degrees per tick

Serial buffer drain (same pattern as tank):
```
char cmd = 0;
while (Serial.available() > 0) {
  cmd = Serial.read();
}
if (cmd) { handleCommand(cmd); }
```

Data structures — use arrays indexed 0-3:
- Servo objects, pin numbers, min angles, max angles, current angles, directions
- Track lastServoTick with millis() for the 20ms interval

No Serial.println() — this will be driven by a GUI.
```

**After generating, tell them to test:**
1. Upload, open Serial Monitor
2. Send `A` — wrist should start moving up continuously
3. Send `a` — wrist should stop
4. Send `5` then `C` — elbow should move faster (5 degrees per tick)
5. Verify each servo stops at its limit (e.g. wrist stops at 135)

---

### Pilot Team — Tank Burst GUI

> **Hand this out if:** Pilot team doesn't have burst timing and speed control by Day 2 HR3.

```
Write a Python GUI for tank control with burst timers and a speed slider.

Context: Instead of "hold to drive" (which doesn't work well over remote desktop), each button click sends a START command, waits a configurable duration, then automatically sends STOP.

Requirements:
- Use guizero (from guizero import App, Box, PushButton, Slider, Text)
- Serial: /dev/ttyACM0 at 9600 baud
- Wait 2 seconds after connecting for Arduino reset

Burst timer mechanism:
- Create a timers dict (timers = {}) to track active timers
- burst(start_char, stop_char) is a factory that returns a fire() function:
  - fire(): sends start_char, then schedules stop() after slider.value milliseconds
  - stop(): sends stop_char, removes timer from dict
  - If a timer already exists for the same key, cancel it before starting new one
  - Use app.after(delay_ms, callback) for scheduling (this is guizero's timer method)

GUI layout:
- App title: "Tank Controller", size 500x450
- Bottom bar (pack this FIRST so Tkinter reserves space): speed slider
  - Slider: range 25 to 200, default 100, label "Tank Speed (ms)"
  - This controls how long each burst lasts
- D-pad (centered above slider):
  - "Forward" → burst("M", "O")
  - "Left" → burst("K", "O")
  - "Stop" → sends "O" directly (emergency stop, also cancels any active timer)
  - "Right" → burst("L", "O")
  - "Backward" → burst("N", "O")
  - Stop button has red background (#ff6666)

Cleanup:
- on_close: send 'O', close serial, destroy app

The slider value is read live each time a burst fires — moving the slider changes the next burst duration immediately.
```

**After generating, tell them to test:**
1. Connect to Arduino running serial-ready tank code
2. Click Forward — robot moves briefly then auto-stops
3. Move slider to 200 — clicks make it move longer; move to 25 — very short bursts
4. Click Stop — immediate halt regardless of timers
5. Close window — motors stop

---

## Phase 3: Day 2 HR5 (Integration)

---

### Combined — Merge Tank + Arm Arduino Code

> **Hand this out if:** Integration team needs help merging the two Arduino sketches onto one board.

```
Merge a tank motor controller and a servo arm controller into a single Arduino sketch that handles both subsystems.

Tank hardware:
- L298N H-bridge
- Left motor: EN_A = 6, IN1 = 4, IN2 = 3 (speed: 200)
- Right motor: EN_B = 5, IN3 = 7, IN4 = 8 (speed: 220)
- Commands: M=forward, N=backward, K=left, L=right, O=stop

Arm hardware:
- 4 servos via Servo library (powered from buck converter):
  - Wrist: pin 10, range 45-135°, start 90°
  - Elbow: pin 12, range 0-90°, start 45°
  - Arm: pin 9, range 70-135°, start 90°
  - Grip: pin 11, range 0-180°, start 90°
- Direction-based movement: each servo has dir = -1/0/+1
- tickServos() runs every 20ms, moves each active servo by stepDeg
- Default stepDeg = 2
- Commands:
  - A/B = Wrist up/down, a = Wrist stop
  - C/D = Elbow up/down, c = Elbow stop
  - G/H = Arm up/down, g = Arm stop
  - I/J = Grip open/close, i = Grip stop
  - '1'-'5' = Set step size

How to merge:
1. Combine all pin definitions and global variables at the top
2. setup(): initialize all motor pins as OUTPUT + attach all servos + start Serial at 9600
3. loop():
   - Serial buffer drain (read all available bytes, act on last)
   - handleCommand() with a single switch statement routing to BOTH tank and arm functions
   - Call tickServos() every 20ms using millis()
4. Keep tank drive functions (driveForward, etc.) and arm tick function as separate helpers
5. No Serial.println() — GUI-driven only

The key insight: both subsystems share the same serial stream. One handleCommand() switch routes tank letters (M/N/K/L/O) to motor functions and arm letters (A-J, a/c/g/i, 1-5) to servo functions.
```

**After generating, tell them to test:**
1. Upload to the single Arduino
2. Send tank commands (M/N/K/L/O) — motors should work exactly as before
3. Send arm commands (A/a, C/c, etc.) — servos should work exactly as before
4. Send commands in rapid succession — buffer drain ensures no lag
5. Both subsystems run independently on the same board

---

### Pilot Team — Full GUI with Tank + Arm + Sliders

> **Hand this out if:** Pilot team needs help building the complete control interface.

```
Write a complete Python GUI that controls both tank motors and arm servos over a single serial connection.

Requirements:
- Use guizero (from guizero import App, Box, PushButton, Slider, Text)
- Serial: /dev/ttyACM0 at 9600 baud
- Wait 2 seconds after connecting for Arduino reset

GUI layout (800x600, title "Op Gas Valve - Robot Controller"):

1. Bottom settings bar (pack FIRST for Tkinter layout):
   - Tank speed slider: range 25-200ms, default 100
   - Arm step slider: range 1-5, default 2
   - Arm step slider has an update command: when changed, send the digit ('1'-'5') to Arduino

2. Main area — two side-by-side panels:
   Left panel [0,0]: TANK
   - D-pad with burst timers (same as burst GUI):
     Forward='M', Back='N', Left='K', Right='L', Stop='O'
   - Burst duration comes from tank speed slider
   - Stop button cancels active timer and sends 'O'

   Right panel [1,0]: ARM
   - 4 joint rows: Wrist, Elbow, Arm, Grip
   - Each joint has "+" and "-" buttons
   - Button commands use FIXED 50ms burst duration (not the tank slider):
     Wrist: + sends 'A' then 'a' after 50ms, - sends 'B' then 'a' after 50ms
     Elbow: + sends 'C' then 'c', - sends 'D' then 'c'
     Arm:   + sends 'G' then 'g', - sends 'H' then 'g'
     Grip:  + sends 'I' then 'i', - sends 'J' then 'i'

Burst timer mechanism:
- make_burst(start_char, stop_char, duration_source) factory function:
  - duration_source can be a Slider (reads .value) or a plain int (like 50)
  - Returns fire() function that sends start, schedules stop after duration
  - Track timers in a dict, cancel existing timer before starting new one
- Tank buttons use the tank speed slider as duration_source
- Arm buttons use fixed integer 50 as duration_source

Cleanup on close:
- Send 'O' (stop motors)
- Send 'a', 'c', 'g', 'i' (stop all servo joints)
- Close serial, destroy app

Both panels share one serial connection. The arm step slider updates the Arduino's step size in real time.
```

**After generating, tell them to test:**
1. Connect to Arduino running the merged robot_controller sketch
2. Tank D-pad — drives the robot with configurable burst duration
3. Arm buttons — each joint moves in small increments
4. Move arm step slider to 5 — arm movements become bigger per click
5. Move tank speed slider to 200 — tank bursts last longer
6. Close window — everything stops cleanly (motors + servos)

---

## Quick Reference: Which Prompt to Give

| Deadline | Team | Prompt Section |
|----------|------|---------------|
| Day 1 HR3 | Tank | Phase 0 — One Motor Test |
| Day 1 HR3 | Arm | Phase 0 — One Servo Test |
| Day 1 HR3 | Pilot | Phase 0 — One Button GUI |
| Day 1 HR5 | Tank | Phase 1 — Two Motor Direction Test |
| Day 1 HR5 | Arm | Phase 1 — Four Servo Tester |
| Day 1 HR5 | Pilot | Phase 1 — Tank D-Pad GUI |
| Day 1 HR7 | Pilot | Phase 1b — Arm Placeholder Buttons |
| Day 2 HR3 | Tank | Phase 2 — Serial-Ready Tank Code |
| Day 2 HR3 | Arm | Phase 2 — Serial-Ready Arm Code |
| Day 2 HR3 | Pilot | Phase 2 — Tank Burst GUI |
| Day 2 HR5 | Combined | Phase 3 — Merge Tank + Arm |
| Day 2 HR5 | Pilot | Phase 3 — Full GUI |
