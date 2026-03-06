"""
═══════════════════════════════════════════════════════════
STEP 4 — Arm Layout (Placeholders)

WHAT THIS DOES:
  Splits the window into two side-by-side panels:
    Left panel:  Tank drive controls (working — same as Step 3)
    Right panel: Arm controls (PLACEHOLDERS — just print to terminal)

  The arm buttons don't control the robot yet. They just
  print a message so you can verify the layout looks right
  before wiring them up in the next step.

WHAT YOU LEARN:
  - Side-by-side panel layout
  - Building a placeholder UI (test the layout first, wire it later)
  - How the arm buttons will be organized (Wrist/Elbow/Arm/Grip, each with +/-)

HOW TO TEST:
  1. Arduino has step2_two_motors.ino uploaded
  2. Run:  python3 step4_arm_layout.py
  3. Tank buttons work as before (bursts)
  4. Click any arm + or - button → see a message in the terminal
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
# Same size and title as the final version.
app = App(title="Op Gas Valve - Robot Controller", width=800, height=600)


# ── SETTINGS BAR (bottom of window) ──────────────────────
# Created first so tkinter reserves space at the bottom.
settings_bar = Box(app, align="bottom", width="fill")

tank_row = Box(settings_bar, width="fill", align="top")
Text(tank_row, text="Tank speed", align="left", width=10)
tank_slider = Slider(tank_row, start=25, end=200, align="left")
tank_slider.value = 100
Text(tank_row, text="ms", align="left")


# ── TANK BURST FUNCTIONS (same as Step 3) ────────────────
tank_timer = [None]

def forward_stop():
    send("O")
    tank_timer[0] = None

def forward_fire():
    if tank_timer[0] is not None:
        app.cancel(tank_timer[0])
    send("M")
    tank_timer[0] = app.after(int(tank_slider.value), forward_stop)

def left_stop():
    send("O")
    tank_timer[0] = None

def left_fire():
    if tank_timer[0] is not None:
        app.cancel(tank_timer[0])
    send("K")
    tank_timer[0] = app.after(int(tank_slider.value), left_stop)

def right_stop():
    send("O")
    tank_timer[0] = None

def right_fire():
    if tank_timer[0] is not None:
        app.cancel(tank_timer[0])
    send("L")
    tank_timer[0] = app.after(int(tank_slider.value), right_stop)

def backward_stop():
    send("O")
    tank_timer[0] = None

def backward_fire():
    if tank_timer[0] is not None:
        app.cancel(tank_timer[0])
    send("N")
    tank_timer[0] = app.after(int(tank_slider.value), backward_stop)

def stop_click():
    if tank_timer[0] is not None:
        app.cancel(tank_timer[0])
        tank_timer[0] = None
    send("O")


# ═══ NEW IN THIS STEP ═════════════════════════════════════

# ── SIDE-BY-SIDE PANELS ──────────────────────────────────
# A grid layout at the top level puts two panels next to each other:
#   [0, 0] = left panel (tank)
#   [1, 0] = right panel (arm)
panels = Box(app, align="top", width="fill", height="fill", layout="grid")


# ── DRIVE PANEL (LEFT SIDE) ──────────────────────────────
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


# ── ARM PANEL (RIGHT SIDE) — PLACEHOLDERS ────────────────
# These buttons DON'T send serial commands yet.
# They just print a message to the terminal so you can see
# that the layout works before wiring them up in Step 5.
arm_box = Box(panels, grid=[1, 0], border=True)
arm_box.set_border(1, "#999999")
Text(arm_box, text="ARM", size=14, font="sans-serif", bold=True)

arm_grid = Box(arm_box, layout="grid")

# -- Wrist row --
def wrist_up_placeholder():
    print("Wrist UP clicked")

def wrist_down_placeholder():
    print("Wrist DOWN clicked")

Text(arm_grid, text="Wrist", grid=[0, 0], width=8, align="left")
PushButton(arm_grid, text="+", grid=[1, 0], width=6, height=3, command=wrist_up_placeholder)
PushButton(arm_grid, text="-", grid=[2, 0], width=6, height=3, command=wrist_down_placeholder)

# -- Elbow row --
def elbow_up_placeholder():
    print("Elbow UP clicked")

def elbow_down_placeholder():
    print("Elbow DOWN clicked")

Text(arm_grid, text="Elbow", grid=[0, 1], width=8, align="left")
PushButton(arm_grid, text="+", grid=[1, 1], width=6, height=3, command=elbow_up_placeholder)
PushButton(arm_grid, text="-", grid=[2, 1], width=6, height=3, command=elbow_down_placeholder)

# -- Arm row --
def arm_up_placeholder():
    print("Arm UP clicked")

def arm_down_placeholder():
    print("Arm DOWN clicked")

Text(arm_grid, text="Arm", grid=[0, 2], width=8, align="left")
PushButton(arm_grid, text="+", grid=[1, 2], width=6, height=3, command=arm_up_placeholder)
PushButton(arm_grid, text="-", grid=[2, 2], width=6, height=3, command=arm_down_placeholder)

# -- Grip row --
def grip_open_placeholder():
    print("Grip OPEN clicked")

def grip_close_placeholder():
    print("Grip CLOSE clicked")

Text(arm_grid, text="Grip", grid=[0, 3], width=8, align="left")
PushButton(arm_grid, text="+", grid=[1, 3], width=6, height=3, command=grip_open_placeholder)
PushButton(arm_grid, text="-", grid=[2, 3], width=6, height=3, command=grip_close_placeholder)

# ═════════════════════════════════════════════════════════


# ── CLEANUP ───────────────────────────────────────────────
def on_close():
    send("O")       # stop motors
    ser.close()     # close serial
    app.destroy()   # close window

app.when_closed = on_close

app.display()
