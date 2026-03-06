"""
STEP 3 — Burst Timer
Learn: timed sends (start → wait → stop), sliders
Test: each click sends a short burst instead of staying on forever
      (pair with Arduino step2)
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
app = App(title="Robot Controller", width=400, height=500)

# ═══ NEW IN THIS STEP ═══
# ── Tank speed slider ──
speed_row = Box(app, width="fill", align="bottom")
Text(speed_row, text="Tank speed", align="left", width=10)
tank_slider = Slider(speed_row, start=25, end=200, align="left")
tank_slider.value = 100
Text(speed_row, text="ms", align="left")

# ── Burst timer tracking ──
tank_timer = [None]  # list so inner functions can modify it

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
# ═════════════════════════

# ── D-Pad layout ──
dpad = Box(app, layout="grid")

PushButton(dpad, text="Forward",  grid=[1, 0], width=12, height=3, command=forward_fire)
PushButton(dpad, text="Left",     grid=[0, 1], width=12, height=3, command=left_fire)

stop_btn = PushButton(dpad, text="Stop", grid=[1, 1], width=12, height=3, command=stop_click)
stop_btn.bg = "#ff6666"

PushButton(dpad, text="Right",    grid=[2, 1], width=12, height=3, command=right_fire)
PushButton(dpad, text="Backward", grid=[1, 2], width=12, height=3, command=backward_fire)

# ── Cleanup ──
def on_close():
    send("O")
    ser.close()
    app.destroy()

app.when_closed = on_close

app.display()
