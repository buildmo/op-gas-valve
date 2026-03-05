# ================================================================
# TANK BURST GUI — Day 2 HR3 (Hard Deadline)
# ================================================================
# Purpose: Tank GUI with burst-timer control and speed slider
# Team:    Pilot
#
# What changed from Day 1:
#   - Burst control: each button click sends a timed pulse
#     (start command → wait → stop command) instead of
#     holding the motor on until you click Stop
#   - Speed slider: adjusts burst duration (25-200ms)
#   - Smoother driving — no more "oops I left it going"
#
# The Stop button still works as an emergency override.
# ================================================================

from guizero import App, Box, Text, PushButton, Slider
import serial
import time

# ── Serial config ── update port if needed ──
port = "/dev/ttyACM0"
baud = 9600

ser = serial.Serial(port, baud, timeout=1)
time.sleep(2)


def send(char):
    ser.write(char.encode())


app = App(title="Tank Controller", width=500, height=450)

# ── Speed slider (pack at bottom first) ──────
settings_bar = Box(app, align="bottom", width="fill")
speed_row = Box(settings_bar, width="fill", align="top")
Text(speed_row, text="Tank speed", align="left", width=10)
tank_slider = Slider(speed_row, start=25, end=200, align="left")
tank_slider.value = 100
Text(speed_row, text="ms", align="left")

# ── Burst timer ──────────────────────────────
timers = {}


def burst(start_char, stop_char):
    def stop():
        send(stop_char)
        timers.pop("tank", None)

    def fire():
        if "tank" in timers:
            app.cancel(timers["tank"])
        send(start_char)
        timers["tank"] = app.after(int(tank_slider.value), stop)

    return fire


# ── D-pad ────────────────────────────────────
Text(app, text="TANK DRIVE", size=14, bold=True)
dpad = Box(app, layout="grid")

PushButton(dpad, text="Forward",  grid=[1, 0], width=12, height=3,
           command=burst("M", "O"))
PushButton(dpad, text="Left",     grid=[0, 1], width=12, height=3,
           command=burst("K", "O"))

stop_btn = PushButton(dpad, text="Stop", grid=[1, 1], width=12, height=3,
                      command=lambda: send("O"))
stop_btn.bg = "#ff6666"

PushButton(dpad, text="Right",    grid=[2, 1], width=12, height=3,
           command=burst("L", "O"))
PushButton(dpad, text="Backward", grid=[1, 2], width=12, height=3,
           command=burst("N", "O"))


# ── Cleanup ──────────────────────────────────
def on_close():
    send("O")
    ser.close()
    app.destroy()


app.when_closed = on_close
app.display()
