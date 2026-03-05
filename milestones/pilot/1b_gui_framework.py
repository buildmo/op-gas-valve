# ================================================================
# GUI FRAMEWORK — Day 1 HR7
# ================================================================
# Purpose: Full layout with tank + arm panels, but only tank is
#          wired to serial. Arm buttons are placeholders — they
#          print to console but don't send serial commands yet.
# Team:    Pilot
#
# Why: The Arm team is still wiring servos on Day 1. There's no
# Arduino serial protocol to talk to yet. This lets Pilot get the
# layout and structure ready so on Day 2 you just swap the
# placeholder functions for real serial sends.
#
# Tank commands (live):  M=fwd  N=back  K=left  L=right  O=stop
# Arm commands (placeholder): prints to console only
# ================================================================

from guizero import App, Box, Text, PushButton
import serial
import time

# ── Serial config ── update port if needed ──
port = "/dev/ttyACM0"
baud = 9600

ser = serial.Serial(port, baud, timeout=1)
time.sleep(2)


def send(char):
    ser.write(char.encode())


# ── Placeholder for arm commands (not wired yet) ──
def arm_placeholder(joint, direction):
    def action():
        print(f"[ARM PLACEHOLDER] {joint} {direction}")
    return action


# ── GUI ──────────────────────────────────────
app = App(title="Op Gas Valve - Robot Controller", width=800, height=600)

panels = Box(app, align="top", width="fill", height="fill", layout="grid")

# ── Drive panel (LIVE) ───────────────────────
tank_box = Box(panels, grid=[0, 0], border=True)
tank_box.set_border(1, "#999999")
Text(tank_box, text="DRIVE", size=14, font="sans-serif", bold=True)

dpad = Box(tank_box, layout="grid")

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

# ── Arm panel (PLACEHOLDER) ─────────────────
arm_box = Box(panels, grid=[1, 0], border=True)
arm_box.set_border(1, "#999999")
Text(arm_box, text="ARM (not wired yet)", size=14, font="sans-serif", bold=True)

arm_grid = Box(arm_box, layout="grid")

arm_joints = ["Wrist", "Elbow", "Arm", "Grip"]

for row, joint in enumerate(arm_joints):
    Text(arm_grid, text=joint, grid=[0, row], width=8, align="left")
    PushButton(arm_grid, text="+", grid=[1, row], width=6, height=3,
               command=arm_placeholder(joint, "+"))
    PushButton(arm_grid, text="-", grid=[2, row], width=6, height=3,
               command=arm_placeholder(joint, "-"))


# ── Cleanup ──────────────────────────────────
def on_close():
    send("O")
    ser.close()
    app.destroy()


app.when_closed = on_close
app.display()
