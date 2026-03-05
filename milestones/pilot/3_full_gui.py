# ================================================================
# FULL GUI — Day 2 HR5
# ================================================================
# Purpose: Complete controller — tank D-pad + arm controls + speed
#          sliders + burst timers + cleanup
# Team:    Pilot
#
# What changed from Day 2 HR3:
#   - Arm controls added (+/- for Wrist, Elbow, Arm, Grip)
#   - Separate speed sliders for tank and arm
#   - Unified burst timer for all controls
#   - Proper cleanup on close (stops motors + all servos)
#
# Tank commands: M=forward  N=backward  K=left  L=right  O=stop
# Arm commands:  A/B=wrist  C/D=elbow  G/H=arm  I/J=grip
#                a/c/g/i = stop individual joints
# ================================================================

from guizero import App, Box, Text, PushButton, Slider
import serial
import time


def run_gui(config=None):
    port = "/dev/ttyACM0"
    baud = 9600

    if config and "serial" in config:
        port = config["serial"].get("port", port)
        baud = config["serial"].get("baud_rate", baud)

    ser = serial.Serial(port, baud, timeout=1)
    time.sleep(2)

    def send(char):
        ser.write(char.encode())

    app = App(title="Op Gas Valve - Robot Controller", width=800, height=600)

    # ── Burst timer helper ────────────────────────────────
    timers = {}

    def make_burst(start_char, stop_char, key, slider):
        def stop():
            send(stop_char)
            timers.pop(key, None)
        def burst():
            if key in timers:
                app.cancel(timers[key])
            send(start_char)
            timers[key] = app.after(int(slider.value), stop)
        return burst

    # ── Settings bar (burst duration sliders) ──────────
    # Created before panels so tkinter reserves space at the bottom first.
    settings_bar = Box(app, align="bottom", width="fill")

    tank_row = Box(settings_bar, width="fill", align="top")
    Text(tank_row, text="Tank speed", align="left", width=10)
    tank_slider = Slider(tank_row, start=25, end=200, align="left")
    tank_slider.value = 100
    Text(tank_row, text="ms", align="left")

    arm_row = Box(settings_bar, width="fill", align="top")
    Text(arm_row, text="Arm speed", align="left", width=10)
    arm_slider = Slider(arm_row, start=25, end=200, align="left")
    arm_slider.value = 100
    Text(arm_row, text="ms", align="left")

    # ── Main panels (drive + arm side by side) ─────────
    panels = Box(app, align="top", width="fill", height="fill",
                 layout="grid")

    # ── Drive panel ────────────────────────────────────
    tank_box = Box(panels, grid=[0, 0], border=True)
    tank_box.set_border(1, "#999999")
    Text(tank_box, text="DRIVE", size=14, font="sans-serif", bold=True)

    dpad = Box(tank_box, layout="grid")

    tank_buttons = {
        "Forward":  ("M", [1, 0]),
        "Left":     ("K", [0, 1]),
        "Right":    ("L", [2, 1]),
        "Backward": ("N", [1, 2]),
    }

    for label, (cmd, pos) in tank_buttons.items():
        PushButton(dpad, text=label, grid=pos, width=12, height=3,
                   command=make_burst(cmd, "O", "tank", tank_slider))

    def stop_tank():
        if "tank" in timers:
            app.cancel(timers["tank"])
            timers.pop("tank")
        send("O")

    stop_btn = PushButton(dpad, text="Stop", grid=[1, 1], width=12, height=3,
                          command=stop_tank)
    stop_btn.bg = "#ff6666"

    # ── Arm panel ──────────────────────────────────────
    arm_box = Box(panels, grid=[1, 0], border=True)
    arm_box.set_border(1, "#999999")
    Text(arm_box, text="ARM", size=14, font="sans-serif", bold=True)

    arm_grid = Box(arm_box, layout="grid")

    arm_cmds = {
        "Wrist":  ("A", "B", "a"),
        "Elbow":  ("C", "D", "c"),
        "Arm":    ("G", "H", "g"),
        "Grip":   ("I", "J", "i"),
    }

    for row, (label, (up, down, stop)) in enumerate(arm_cmds.items()):
        Text(arm_grid, text=label, grid=[0, row], width=8, align="left")
        PushButton(arm_grid, text="+", grid=[1, row], width=6, height=3,
                   command=make_burst(up, stop, label, arm_slider))
        PushButton(arm_grid, text="-", grid=[2, row], width=6, height=3,
                   command=make_burst(down, stop, label, arm_slider))

    # ── Cleanup on close ──────────────────────────────
    def on_close():
        send("O")
        for _, _, stop_char in arm_cmds.values():
            send(stop_char)
        ser.close()
        app.destroy()

    app.when_closed = on_close
    app.display()


if __name__ == "__main__":
    run_gui()
