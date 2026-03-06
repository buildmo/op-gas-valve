"""
STEP 1 — One Button
Learn: guizero basics, serial connection, sending a command
Test: click Forward — robot should drive forward (pair with Arduino step2)
"""

from guizero import App, PushButton
import serial
import time

# ── Connect to Arduino ──
ser = serial.Serial("/dev/ttyACM0", 9600, timeout=1)
time.sleep(2)

def send(char):
    ser.write(char.encode())

# ── GUI ──
app = App(title="Robot Controller", width=300, height=200)

def forward_click():
    send("M")

PushButton(app, text="Forward", command=forward_click, width=12, height=3)

app.display()
