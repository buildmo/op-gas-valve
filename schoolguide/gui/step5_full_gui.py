"""
STEP 5 — Full GUI (Tank + Arm)
This is the final working GUI — same behavior as final_gui
Test: pair with Arduino step6
"""

from guizero import App, Box, Text, PushButton, Slider
import serial
import time

# ── Connect to Arduino ──
ser = serial.Serial("/dev/ttyACM0", 9600, timeout=1)
time.sleep(2)

def send(char):
    ser.write(char.encode())

# ── GUI ──
app = App(title="Op Gas Valve - Robot Controller", width=800, height=600)

# ── Settings bar (bottom) ──
settings_bar = Box(app, align="bottom", width="fill")

tank_row = Box(settings_bar, width="fill", align="top")
Text(tank_row, text="Tank speed", align="left", width=10)
tank_slider = Slider(tank_row, start=25, end=200, align="left")
tank_slider.value = 100
Text(tank_row, text="ms", align="left")

# ═══ NEW IN THIS STEP ═══
arm_row = Box(settings_bar, width="fill", align="top")
Text(arm_row, text="Arm step", align="left", width=10)
arm_step_slider = Slider(arm_row, start=1, end=5, align="left")
arm_step_slider.value = 2
Text(arm_row, text="deg", align="left")

def on_arm_step_change(value):
    send(str(int(float(value))))

arm_step_slider.update_command = on_arm_step_change
# ═════════════════════════

# ── Burst timer tracking ──
tank_timer = [None]
arm_timers = {}

# ── Tank burst functions (explicit per direction) ──
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

# ═══ NEW IN THIS STEP ═══
# ── Arm burst functions (explicit per joint) ──
def wrist_up_stop():
    send("a")
    arm_timers.pop("wrist", None)

def wrist_up_fire():
    if "wrist" in arm_timers:
        app.cancel(arm_timers["wrist"])
    send("A")
    arm_timers["wrist"] = app.after(50, wrist_up_stop)

def wrist_down_stop():
    send("a")
    arm_timers.pop("wrist", None)

def wrist_down_fire():
    if "wrist" in arm_timers:
        app.cancel(arm_timers["wrist"])
    send("B")
    arm_timers["wrist"] = app.after(50, wrist_down_stop)

def elbow_up_stop():
    send("c")
    arm_timers.pop("elbow", None)

def elbow_up_fire():
    if "elbow" in arm_timers:
        app.cancel(arm_timers["elbow"])
    send("C")
    arm_timers["elbow"] = app.after(50, elbow_up_stop)

def elbow_down_stop():
    send("c")
    arm_timers.pop("elbow", None)

def elbow_down_fire():
    if "elbow" in arm_timers:
        app.cancel(arm_timers["elbow"])
    send("D")
    arm_timers["elbow"] = app.after(50, elbow_down_stop)

def arm_up_stop():
    send("g")
    arm_timers.pop("arm", None)

def arm_up_fire():
    if "arm" in arm_timers:
        app.cancel(arm_timers["arm"])
    send("G")
    arm_timers["arm"] = app.after(50, arm_up_stop)

def arm_down_stop():
    send("g")
    arm_timers.pop("arm", None)

def arm_down_fire():
    if "arm" in arm_timers:
        app.cancel(arm_timers["arm"])
    send("H")
    arm_timers["arm"] = app.after(50, arm_down_stop)

def grip_open_stop():
    send("i")
    arm_timers.pop("grip", None)

def grip_open_fire():
    if "grip" in arm_timers:
        app.cancel(arm_timers["grip"])
    send("I")
    arm_timers["grip"] = app.after(50, grip_open_stop)

def grip_close_stop():
    send("i")
    arm_timers.pop("grip", None)

def grip_close_fire():
    if "grip" in arm_timers:
        app.cancel(arm_timers["grip"])
    send("J")
    arm_timers["grip"] = app.after(50, grip_close_stop)
# ═════════════════════════

# ── Side-by-side panels ──
panels = Box(app, align="top", width="fill", height="fill", layout="grid")

# ── Drive panel (left) ──
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

# ── Arm panel (right) ──
arm_box = Box(panels, grid=[1, 0], border=True)
arm_box.set_border(1, "#999999")
Text(arm_box, text="ARM", size=14, font="sans-serif", bold=True)

arm_grid = Box(arm_box, layout="grid")

# ═══ NEW IN THIS STEP ═══ (wired to real serial commands)
Text(arm_grid, text="Wrist", grid=[0, 0], width=8, align="left")
PushButton(arm_grid, text="+", grid=[1, 0], width=6, height=3, command=wrist_up_fire)
PushButton(arm_grid, text="-", grid=[2, 0], width=6, height=3, command=wrist_down_fire)

Text(arm_grid, text="Elbow", grid=[0, 1], width=8, align="left")
PushButton(arm_grid, text="+", grid=[1, 1], width=6, height=3, command=elbow_up_fire)
PushButton(arm_grid, text="-", grid=[2, 1], width=6, height=3, command=elbow_down_fire)

Text(arm_grid, text="Arm", grid=[0, 2], width=8, align="left")
PushButton(arm_grid, text="+", grid=[1, 2], width=6, height=3, command=arm_up_fire)
PushButton(arm_grid, text="-", grid=[2, 2], width=6, height=3, command=arm_down_fire)

Text(arm_grid, text="Grip", grid=[0, 3], width=8, align="left")
PushButton(arm_grid, text="+", grid=[1, 3], width=6, height=3, command=grip_open_fire)
PushButton(arm_grid, text="-", grid=[2, 3], width=6, height=3, command=grip_close_fire)
# ═════════════════════════

# ── Cleanup on close ──
def on_close():
    send("O")
    send("a")
    send("c")
    send("g")
    send("i")
    ser.close()
    app.destroy()

app.when_closed = on_close

app.display()
