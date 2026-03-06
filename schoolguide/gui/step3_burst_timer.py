"""
═══════════════════════════════════════════════════════════
STEP 3 — Burst Timer

WHAT THIS DOES:
  Instead of clicking "Forward" and the robot driving forever,
  each click now sends a SHORT BURST — the robot moves for a
  fraction of a second, then automatically stops.

  Think of it like tapping the gas pedal instead of holding it down.

  A slider lets you control how long each burst lasts
  (25ms = tiny tap, 200ms = longer push).

WHAT YOU LEARN:
  - Timed actions: "do this, then stop after X milliseconds"
  - Sliders for adjustable settings
  - Fire/stop pattern (start → wait → stop)

HOW TO TEST:
  1. Arduino has step2_two_motors.ino uploaded
  2. Run:  python3 step3_burst_timer.py
  3. Click Forward — robot moves briefly then stops by itself!
  4. Drag the slider left (25ms) — very short bursts
  5. Drag the slider right (200ms) — longer bursts
═══════════════════════════════════════════════════════════
"""

from guizero import App, Box, Text, PushButton, Slider
import serial
import time


# ── CONNECT TO THE ARDUINO ────────────────────────────────
ser = serial.Serial("/dev/ttyACM0", 9600, timeout=1)
time.sleep(2)

def send(char):
    ser.write(char.encode())


# ── CREATE THE WINDOW ─────────────────────────────────────
app = App(title="Robot Controller", width=400, height=500)


# ═══ NEW IN THIS STEP ═════════════════════════════════════

# ── TANK SPEED SLIDER ────────────────────────────────────
# This slider controls how many milliseconds each burst lasts.
# Placed at the bottom of the window (align="bottom").
speed_row = Box(app, width="fill", align="bottom")
Text(speed_row, text="Tank speed", align="left", width=10)
tank_slider = Slider(speed_row, start=25, end=200, align="left")
tank_slider.value = 100    # default: 100 milliseconds
Text(speed_row, text="ms", align="left")


# ── BURST TIMER TRACKING ─────────────────────────────────
# We use a list [None] instead of a plain variable because
# Python needs a mutable container for inner functions to modify.
# When a timer is active, this holds its ID so we can cancel it.
tank_timer = [None]


# ── FORWARD BURST ─────────────────────────────────────────
# How a burst works:
#   1. forward_fire() sends "M" (start moving)
#   2. Schedules forward_stop() to run after X milliseconds
#   3. forward_stop() sends "O" (stop moving)
#
# If you click again before the timer fires, we cancel the
# old timer and start a new one — this prevents confusion.

def forward_stop():
    send("O")                 # tell Arduino to stop
    tank_timer[0] = None      # clear the timer

def forward_fire():
    if tank_timer[0] is not None:       # cancel any running timer
        app.cancel(tank_timer[0])
    send("M")                           # start driving forward
    ms = int(tank_slider.value)         # read the slider value
    tank_timer[0] = app.after(ms, forward_stop)  # schedule the stop


# ── LEFT BURST ────────────────────────────────────────────
def left_stop():
    send("O")
    tank_timer[0] = None

def left_fire():
    if tank_timer[0] is not None:
        app.cancel(tank_timer[0])
    send("K")
    ms = int(tank_slider.value)
    tank_timer[0] = app.after(ms, left_stop)


# ── RIGHT BURST ───────────────────────────────────────────
def right_stop():
    send("O")
    tank_timer[0] = None

def right_fire():
    if tank_timer[0] is not None:
        app.cancel(tank_timer[0])
    send("L")
    ms = int(tank_slider.value)
    tank_timer[0] = app.after(ms, right_stop)


# ── BACKWARD BURST ────────────────────────────────────────
def backward_stop():
    send("O")
    tank_timer[0] = None

def backward_fire():
    if tank_timer[0] is not None:
        app.cancel(tank_timer[0])
    send("N")
    ms = int(tank_slider.value)
    tank_timer[0] = app.after(ms, backward_stop)


# ── STOP BUTTON ───────────────────────────────────────────
# The stop button cancels any active timer AND sends stop immediately.
# This is the emergency brake.
def stop_click():
    if tank_timer[0] is not None:
        app.cancel(tank_timer[0])
        tank_timer[0] = None
    send("O")

# ═════════════════════════════════════════════════════════


# ── D-PAD LAYOUT ──────────────────────────────────────────
# Same grid as Step 2, but now each button calls a _fire function
# instead of sending the command directly.
dpad = Box(app, layout="grid")

PushButton(dpad, text="Forward",  grid=[1, 0], width=12, height=3, command=forward_fire)
PushButton(dpad, text="Left",     grid=[0, 1], width=12, height=3, command=left_fire)

stop_btn = PushButton(dpad, text="Stop", grid=[1, 1], width=12, height=3, command=stop_click)
stop_btn.bg = "#ff6666"

PushButton(dpad, text="Right",    grid=[2, 1], width=12, height=3, command=right_fire)
PushButton(dpad, text="Backward", grid=[1, 2], width=12, height=3, command=backward_fire)


# ── CLEANUP ───────────────────────────────────────────────
def on_close():
    send("O")
    ser.close()
    app.destroy()

app.when_closed = on_close

app.display()
