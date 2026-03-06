"""
═══════════════════════════════════════════════════════════
STEP 5 — Full GUI (Tank + Arm)

WHAT THIS DOES:
  This is the FINAL working GUI. It controls the complete robot:
    - Tank drive with burst timing (left panel)
    - All four arm joints with burst timing (right panel)
    - Tank speed slider (25–200ms bursts)
    - Arm step slider (1–5 degrees per step)

  This does exactly the same thing as final_gui/gui.py,
  just written with individual named functions instead of
  factory functions and loops.

WHAT YOU LEARN:
  - Wiring arm buttons to real serial commands
  - Arm burst pattern (send start → wait 50ms → send stop)
  - Step size slider that sends commands to the Arduino

HOW TO TEST:
  1. Arduino has step6_robot_combined.ino uploaded
  2. Run:  python3 step5_full_gui.py
  3. Tank buttons move the robot (short bursts)
  4. Arm buttons move each joint (short bursts)
  5. Drag "Arm step" slider to 5 → arm moves faster per click
  6. Close window → everything stops safely
═══════════════════════════════════════════════════════════
"""

from guizero import App, Box, Text, PushButton, Slider
import serial
import time


# ── CONNECT TO THE ARDUINO ────────────────────────────────
ser = serial.Serial("/dev/ttyACM0", 9600, timeout=1)
time.sleep(2)

# Send one character to the Arduino.
def send(char):
    ser.write(char.encode())


# ── CREATE THE WINDOW ─────────────────────────────────────
app = App(title="Op Gas Valve - Robot Controller", width=800, height=600)


# ══════════════════════════════════════════════════════════
# SETTINGS BAR (bottom of window)
# ══════════════════════════════════════════════════════════
# Created before panels so tkinter reserves space at the bottom first.
settings_bar = Box(app, align="bottom", width="fill")

# -- Tank speed slider --
# Controls how long each tank burst lasts (in milliseconds).
tank_row = Box(settings_bar, width="fill", align="top")
Text(tank_row, text="Tank speed", align="left", width=10)
tank_slider = Slider(tank_row, start=25, end=200, align="left")
tank_slider.value = 100    # default: 100ms per burst
Text(tank_row, text="ms", align="left")

# ═══ NEW IN THIS STEP ═════════════════════════════════════
# -- Arm step slider --
# Controls how many degrees the servo moves per tick.
# The Arduino receives '1' through '5' and changes its step size.
arm_row = Box(settings_bar, width="fill", align="top")
Text(arm_row, text="Arm step", align="left", width=10)
arm_step_slider = Slider(arm_row, start=1, end=5, align="left")
arm_step_slider.value = 2    # default: 2 degrees per tick
Text(arm_row, text="deg", align="left")

# This function runs every time the slider value changes.
# It sends the new step size to the Arduino.
def on_arm_step_change(value):
    send(str(int(float(value))))    # convert slider value to a digit character

# Connect the function to the slider.
arm_step_slider.update_command = on_arm_step_change
# ═════════════════════════════════════════════════════════


# ══════════════════════════════════════════════════════════
# TANK BURST FUNCTIONS
# ══════════════════════════════════════════════════════════
# Same pattern as Step 3:
#   fire() sends start command + schedules stop after X ms
#   stop() sends the stop command

tank_timer = [None]

# -- Forward --
def forward_stop():
    send("O")
    tank_timer[0] = None

def forward_fire():
    if tank_timer[0] is not None:
        app.cancel(tank_timer[0])
    send("M")
    tank_timer[0] = app.after(int(tank_slider.value), forward_stop)

# -- Left --
def left_stop():
    send("O")
    tank_timer[0] = None

def left_fire():
    if tank_timer[0] is not None:
        app.cancel(tank_timer[0])
    send("K")
    tank_timer[0] = app.after(int(tank_slider.value), left_stop)

# -- Right --
def right_stop():
    send("O")
    tank_timer[0] = None

def right_fire():
    if tank_timer[0] is not None:
        app.cancel(tank_timer[0])
    send("L")
    tank_timer[0] = app.after(int(tank_slider.value), right_stop)

# -- Backward --
def backward_stop():
    send("O")
    tank_timer[0] = None

def backward_fire():
    if tank_timer[0] is not None:
        app.cancel(tank_timer[0])
    send("N")
    tank_timer[0] = app.after(int(tank_slider.value), backward_stop)

# -- Stop button (emergency brake) --
def stop_click():
    if tank_timer[0] is not None:
        app.cancel(tank_timer[0])
        tank_timer[0] = None
    send("O")


# ═══ NEW IN THIS STEP ═════════════════════════════════════
# ══════════════════════════════════════════════════════════
# ARM BURST FUNCTIONS
# ══════════════════════════════════════════════════════════
# Same burst pattern, but for arm joints.
# Each joint gets its own fire/stop pair.
# Arm bursts are fixed at 50ms (not slider-controlled).
#
# How it works:
#   1. wrist_up_fire() sends 'A' (start wrist moving up)
#   2. After 50ms, wrist_up_stop() sends 'a' (stop wrist)
#   3. Result: the wrist moves just a tiny bit per click

arm_timers = {}    # tracks active timers per joint

# -- Wrist --
def wrist_up_stop():
    send("a")                              # 'a' = stop wrist
    arm_timers.pop("wrist", None)

def wrist_up_fire():
    if "wrist" in arm_timers:
        app.cancel(arm_timers["wrist"])
    send("A")                              # 'A' = wrist up
    arm_timers["wrist"] = app.after(50, wrist_up_stop)

def wrist_down_stop():
    send("a")                              # 'a' = stop wrist
    arm_timers.pop("wrist", None)

def wrist_down_fire():
    if "wrist" in arm_timers:
        app.cancel(arm_timers["wrist"])
    send("B")                              # 'B' = wrist down
    arm_timers["wrist"] = app.after(50, wrist_down_stop)

# -- Elbow --
def elbow_up_stop():
    send("c")                              # 'c' = stop elbow
    arm_timers.pop("elbow", None)

def elbow_up_fire():
    if "elbow" in arm_timers:
        app.cancel(arm_timers["elbow"])
    send("C")                              # 'C' = elbow up
    arm_timers["elbow"] = app.after(50, elbow_up_stop)

def elbow_down_stop():
    send("c")                              # 'c' = stop elbow
    arm_timers.pop("elbow", None)

def elbow_down_fire():
    if "elbow" in arm_timers:
        app.cancel(arm_timers["elbow"])
    send("D")                              # 'D' = elbow down
    arm_timers["elbow"] = app.after(50, elbow_down_stop)

# -- Arm --
def arm_up_stop():
    send("g")                              # 'g' = stop arm
    arm_timers.pop("arm", None)

def arm_up_fire():
    if "arm" in arm_timers:
        app.cancel(arm_timers["arm"])
    send("G")                              # 'G' = arm up
    arm_timers["arm"] = app.after(50, arm_up_stop)

def arm_down_stop():
    send("g")                              # 'g' = stop arm
    arm_timers.pop("arm", None)

def arm_down_fire():
    if "arm" in arm_timers:
        app.cancel(arm_timers["arm"])
    send("H")                              # 'H' = arm down
    arm_timers["arm"] = app.after(50, arm_down_stop)

# -- Grip --
def grip_open_stop():
    send("i")                              # 'i' = stop grip
    arm_timers.pop("grip", None)

def grip_open_fire():
    if "grip" in arm_timers:
        app.cancel(arm_timers["grip"])
    send("I")                              # 'I' = grip open
    arm_timers["grip"] = app.after(50, grip_open_stop)

def grip_close_stop():
    send("i")                              # 'i' = stop grip
    arm_timers.pop("grip", None)

def grip_close_fire():
    if "grip" in arm_timers:
        app.cancel(arm_timers["grip"])
    send("J")                              # 'J' = grip close
    arm_timers["grip"] = app.after(50, grip_close_stop)

# ═════════════════════════════════════════════════════════


# ══════════════════════════════════════════════════════════
# LAYOUT — Two panels side by side
# ══════════════════════════════════════════════════════════
panels = Box(app, align="top", width="fill", height="fill", layout="grid")


# ── DRIVE PANEL (left side) ──────────────────────────────
tank_box = Box(panels, grid=[0, 0], border=True)
tank_box.set_border(1, "#999999")
Text(tank_box, text="DRIVE", size=14, font="sans-serif", bold=True)

dpad = Box(tank_box, layout="grid")

PushButton(dpad, text="Forward",  grid=[1, 0], width=12, height=3, command=forward_fire)
PushButton(dpad, text="Left",     grid=[0, 1], width=12, height=3, command=left_fire)

stop_btn = PushButton(dpad, text="Stop", grid=[1, 1], width=12, height=3, command=stop_click)
stop_btn.bg = "#ff6666"

PushButton(dpad, text="Right",    grid=[2, 1], width=12, height=3, command=right_fire)
PushButton(dpad, text="Backward", grid=[1, 2], width=12, height=3, command=backward_fire)


# ── ARM PANEL (right side) ───────────────────────────────
arm_box = Box(panels, grid=[1, 0], border=True)
arm_box.set_border(1, "#999999")
Text(arm_box, text="ARM", size=14, font="sans-serif", bold=True)

arm_grid = Box(arm_box, layout="grid")

# ═══ NEW IN THIS STEP ═══ (wired to real serial commands!)

# Wrist row: + sends 'A' (up), - sends 'B' (down)
Text(arm_grid, text="Wrist", grid=[0, 0], width=8, align="left")
PushButton(arm_grid, text="+", grid=[1, 0], width=6, height=3, command=wrist_up_fire)
PushButton(arm_grid, text="-", grid=[2, 0], width=6, height=3, command=wrist_down_fire)

# Elbow row: + sends 'C' (up), - sends 'D' (down)
Text(arm_grid, text="Elbow", grid=[0, 1], width=8, align="left")
PushButton(arm_grid, text="+", grid=[1, 1], width=6, height=3, command=elbow_up_fire)
PushButton(arm_grid, text="-", grid=[2, 1], width=6, height=3, command=elbow_down_fire)

# Arm row: + sends 'G' (up), - sends 'H' (down)
Text(arm_grid, text="Arm", grid=[0, 2], width=8, align="left")
PushButton(arm_grid, text="+", grid=[1, 2], width=6, height=3, command=arm_up_fire)
PushButton(arm_grid, text="-", grid=[2, 2], width=6, height=3, command=arm_down_fire)

# Grip row: + sends 'I' (open), - sends 'J' (close)
Text(arm_grid, text="Grip", grid=[0, 3], width=8, align="left")
PushButton(arm_grid, text="+", grid=[1, 3], width=6, height=3, command=grip_open_fire)
PushButton(arm_grid, text="-", grid=[2, 3], width=6, height=3, command=grip_close_fire)

# ═════════════════════════════════════════════════════════


# ══════════════════════════════════════════════════════════
# CLEANUP — runs when the window is closed
# ══════════════════════════════════════════════════════════
# Stop EVERYTHING: motors + all four joints.
# Then close the serial connection and the window.
def on_close():
    send("O")     # stop motors
    send("a")     # stop wrist
    send("c")     # stop elbow
    send("g")     # stop arm
    send("i")     # stop grip
    ser.close()   # close serial connection
    app.destroy() # close window

app.when_closed = on_close

app.display()
