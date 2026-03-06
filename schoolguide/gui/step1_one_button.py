"""
═══════════════════════════════════════════════════════════
STEP 1 — One Button

WHAT THIS DOES:
  A window with a single "Forward" button.
  Click it and the robot drives forward.

WHAT YOU LEARN:
  - Opening a window with guizero
  - Connecting to the Arduino over USB (serial)
  - Making a button that sends a command

HOW TO TEST:
  1. Make sure Arduino has step2_two_motors.ino uploaded
  2. Run this file:  python3 step1_one_button.py
  3. Click "Forward" — the robot drives forward
  4. Close the window to exit
═══════════════════════════════════════════════════════════
"""

# "guizero" is a simple library for making graphical interfaces.
# We only need two pieces: App (the window) and PushButton (a clickable button).
from guizero import App, PushButton

# "serial" lets us talk to the Arduino over the USB cable.
import serial

# "time" lets us pause — we need a short wait after connecting.
import time


# ── CONNECT TO THE ARDUINO ────────────────────────────────
# "/dev/ttyACM0" is the USB port name on a Raspberry Pi.
# 9600 is the communication speed — must match the Arduino sketch.
# timeout=1 means "don't hang forever if nothing arrives."
ser = serial.Serial("/dev/ttyACM0", 9600, timeout=1)

# Wait 2 seconds for the Arduino to reset after connecting.
# (The Arduino reboots every time a serial connection opens.)
time.sleep(2)


# ── HELPER: SEND A COMMAND ────────────────────────────────
# This tiny function sends one letter to the Arduino.
# .encode() converts the text letter into bytes (the format serial needs).
def send(char):
    ser.write(char.encode())


# ── CREATE THE WINDOW ─────────────────────────────────────
# App() creates the main window on screen.
app = App(title="Robot Controller", width=300, height=200)


# ── WHAT HAPPENS WHEN THE BUTTON IS CLICKED ───────────────
# This function runs every time someone clicks "Forward".
# It sends the letter "M" to the Arduino, which means "drive forward."
def forward_click():
    send("M")


# ── CREATE THE BUTTON ─────────────────────────────────────
# PushButton() puts a clickable button inside the window.
# text = what the button says
# command = which function to run when clicked
# width/height = how big the button is (in text characters)
PushButton(app, text="Forward", command=forward_click, width=12, height=3)


# ── SHOW THE WINDOW ───────────────────────────────────────
# app.display() opens the window and keeps it running.
# The program stays here until you close the window.
app.display()
