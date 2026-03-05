# ================================================================
# TANK GUI — Day 1 HR5
# ================================================================
# Purpose: Minimal D-pad to test tank driving from the Pi
# Team:    Pilot
# Test:    Click buttons — tank should move, stop on Stop
#
# This sends single characters over serial to the Arduino:
#   M=Forward  N=Backward  K=Left  L=Right  O=Stop
#
# If nothing happens: check serial port (ls /dev/ttyACM* or
# ls /dev/ttyUSB*) and update the port variable below.
# ================================================================

from guizero import App, Box, Text, PushButton
import serial
import time

# ── Serial config ── update port if needed ──
port = "/dev/ttyACM0"
baud = 9600

ser = serial.Serial(port, baud, timeout=1)
time.sleep(2)  # wait for Arduino to reset after serial connect


def send(char):
    ser.write(char.encode())


# ── GUI ──────────────────────────────────────
app = App(title="Tank Controller", width=400, height=400)

Text(app, text="TANK DRIVE", size=14, bold=True)

dpad = Box(app, layout="grid")

PushButton(dpad, text="Forward",  grid=[1, 0], width=12, height=3,
           command=lambda: send("M"))
PushButton(dpad, text="Left",     grid=[0, 1], width=12, height=3,
           command=lambda: send("K"))

stop_btn = PushButton(dpad, text="Stop", grid=[1, 1], width=12, height=3,
                      command=lambda: send("O"))
stop_btn.bg = "#ff6666"

PushButton(dpad, text="Right",    grid=[2, 1], width=12, height=3,
           command=lambda: send("L"))
PushButton(dpad, text="Backward", grid=[1, 2], width=12, height=3,
           command=lambda: send("N"))


# ── Cleanup ──────────────────────────────────
def on_close():
    send("O")
    ser.close()
    app.destroy()


app.when_closed = on_close
app.display()
