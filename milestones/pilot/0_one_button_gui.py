# ================================================================
# ONE BUTTON GUI — Base Code
# ================================================================
# Purpose: Get one button sending a serial command to the Arduino
# Team:    Pilot
# Test:    Click the button — the tank should drive forward
#
# This is the simplest possible GUI. One button, one command.
# Once this works, you know serial communication between the Pi
# and Arduino is working. Then add more buttons.
#
# If nothing happens: check the serial port with
#   ls /dev/ttyACM* or ls /dev/ttyUSB*
# and update the port variable below.
# ================================================================

from guizero import App, PushButton
import serial
import time

# ── Serial config ── update port if needed ──
port = "/dev/ttyACM0"
baud = 9600

ser = serial.Serial(port, baud, timeout=1)
time.sleep(2)  # wait for Arduino to reset after serial connect

app = App(title="One Button Test", width=300, height=200)

PushButton(app, text="Forward", width=20, height=5,
           command=lambda: ser.write(b"M"))

app.display()
