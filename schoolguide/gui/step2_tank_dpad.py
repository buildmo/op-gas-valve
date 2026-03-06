"""
STEP 2 — Tank D-Pad
Learn: grid layout, multiple buttons, on_close cleanup
Test: all 5 buttons work with Arduino step2
"""

from guizero import App, Box, PushButton
import serial
import time

# ── Connect to Arduino ──
ser = serial.Serial("/dev/ttyACM0", 9600, timeout=1)
time.sleep(2)

def send(char):
    ser.write(char.encode())

# ── GUI ──
app = App(title="Robot Controller", width=400, height=400)

# ═══ NEW IN THIS STEP ═══
# ── D-Pad layout (grid) ──
dpad = Box(app, layout="grid")

def forward_click():
    send("M")

def left_click():
    send("K")

def stop_click():
    send("O")

def right_click():
    send("L")

def backward_click():
    send("N")

PushButton(dpad, text="Forward",  grid=[1, 0], width=12, height=3, command=forward_click)
PushButton(dpad, text="Left",     grid=[0, 1], width=12, height=3, command=left_click)

stop_btn = PushButton(dpad, text="Stop", grid=[1, 1], width=12, height=3, command=stop_click)
stop_btn.bg = "#ff6666"

PushButton(dpad, text="Right",    grid=[2, 1], width=12, height=3, command=right_click)
PushButton(dpad, text="Backward", grid=[1, 2], width=12, height=3, command=backward_click)

# ── Cleanup ──
def on_close():
    send("O")
    ser.close()
    app.destroy()

app.when_closed = on_close
# ═════════════════════════

app.display()
