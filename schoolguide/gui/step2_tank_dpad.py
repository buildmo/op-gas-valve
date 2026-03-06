"""
═══════════════════════════════════════════════════════════
STEP 2 — Tank D-Pad

WHAT THIS DOES:
  Five buttons arranged like a game controller d-pad:
       Forward
  Left   Stop   Right
      Backward

  Each button sends a single letter to the Arduino.

WHAT YOU LEARN:
  - Grid layout (arranging buttons in rows and columns)
  - One named function per button (no guessing what does what)
  - Cleanup on close (send "stop" when the window closes)

HOW TO TEST:
  1. Arduino has step2_two_motors.ino uploaded
  2. Run:  python3 step2_tank_dpad.py
  3. Click Forward — robot drives forward and keeps going
  4. Click Stop — robot stops
  5. Close the window — robot also stops (cleanup!)
═══════════════════════════════════════════════════════════
"""

from guizero import App, Box, PushButton
import serial
import time


# ── CONNECT TO THE ARDUINO ────────────────────────────────
ser = serial.Serial("/dev/ttyACM0", 9600, timeout=1)
time.sleep(2)

def send(char):
    ser.write(char.encode())


# ── CREATE THE WINDOW ─────────────────────────────────────
app = App(title="Robot Controller", width=400, height=400)


# ═══ NEW IN THIS STEP ═════════════════════════════════════

# ── D-PAD LAYOUT ──────────────────────────────────────────
# A "grid" layout lets us place buttons at specific positions.
# Grid coordinates work like a spreadsheet:
#   [column, row]  →  [0,0] is top-left
#
#          col0    col1    col2
#   row0           Fwd
#   row1   Left    Stop    Right
#   row2           Bwd
dpad = Box(app, layout="grid")


# ── ONE FUNCTION PER BUTTON ──────────────────────────────
# Each button has its own clearly-named function.
# No mystery about what each one does.

def forward_click():
    send("M")     # M = forward (both motors forward)

def left_click():
    send("K")     # K = left (spin counter-clockwise)

def stop_click():
    send("O")     # O = stop (all motors off)

def right_click():
    send("L")     # L = right (spin clockwise)

def backward_click():
    send("N")     # N = backward (both motors backward)


# ── CREATE THE BUTTONS ────────────────────────────────────
# Each PushButton specifies:
#   text    = label shown on the button
#   grid    = [column, row] position in the grid
#   width   = button width in characters
#   height  = button height in lines
#   command = function to call when clicked

PushButton(dpad, text="Forward",  grid=[1, 0], width=12, height=3, command=forward_click)
PushButton(dpad, text="Left",     grid=[0, 1], width=12, height=3, command=left_click)

# The stop button is red so it stands out as the emergency stop.
stop_btn = PushButton(dpad, text="Stop", grid=[1, 1], width=12, height=3, command=stop_click)
stop_btn.bg = "#ff6666"    # light red background

PushButton(dpad, text="Right",    grid=[2, 1], width=12, height=3, command=right_click)
PushButton(dpad, text="Backward", grid=[1, 2], width=12, height=3, command=backward_click)


# ── CLEANUP WHEN THE WINDOW CLOSES ───────────────────────
# This function runs automatically when you click the X button.
# It sends "stop" to the Arduino so the robot doesn't keep driving.
def on_close():
    send("O")       # stop the motors
    ser.close()     # close the serial connection cleanly
    app.destroy()   # close the window

# Tell the app to run on_close when the window is closed.
app.when_closed = on_close

# ═════════════════════════════════════════════════════════

app.display()
