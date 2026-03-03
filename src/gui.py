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

    def on_close():
        for char in "Oacgi":
            send(char)
        ser.close()
        app.destroy()

    app = App(title="Op Gas Valve - Robot Controller", width=800, height=600)
    app.when_closed = on_close

    burst_timer = {"id": None}

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

    # ── Tank d-pad buttons ─────────────────────────────
    tank_cmds = {
        "Forward": "M", "Backward": "N",
        "Left": "K", "Right": "L",
    }
    tank_grid = {
        "Forward":  [1, 0],
        "Left":     [0, 1],
        "Right":    [2, 1],
        "Backward": [1, 2],
    }
    def make_burst(char):
        def burst():
            if burst_timer["id"] is not None:
                app.cancel(burst_timer["id"])
            send(char)
            burst_timer["id"] = app.after(int(tank_slider.value), lambda: send("O"))
        return burst

    for label, pos in tank_grid.items():
        PushButton(dpad, text=label, grid=pos, width=12, height=3,
                   command=make_burst(tank_cmds[label]))

    stop_btn = PushButton(dpad, text="Stop", grid=[1, 1], width=12, height=3,
                          command=lambda: send("O"))
    stop_btn.bg = "#ff6666"

    # ── Arm panel ──────────────────────────────────────
    arm_box = Box(panels, grid=[1, 0], border=True)
    arm_box.set_border(1, "#999999")
    Text(arm_box, text="ARM", size=14, font="sans-serif", bold=True)

    arm_grid = Box(arm_box, layout="grid")

    # ── Arm helpers ────────────────────────────────────
    arm_timers = {}

    def make_arm_burst(start_char, stop_char, key):
        def burst():
            if key in arm_timers:
                app.cancel(arm_timers[key])
            send(start_char)
            arm_timers[key] = app.after(int(arm_slider.value), lambda: send(stop_char))
        return burst

    arm_cmds = {
        "Wrist":  ("A", "B", "a"),
        "Elbow":  ("C", "D", "c"),
        "Arm":    ("G", "H", "g"),
        "Grip":   ("I", "J", "i"),
    }

    for row, (label, chars) in enumerate(arm_cmds.items()):
        up_char, down_char, stop_char = chars
        Text(arm_grid, text=label, grid=[0, row], width=8, align="left")

        PushButton(arm_grid, text="+", grid=[1, row], width=6, height=3,
                   command=make_arm_burst(up_char, stop_char, label))
        PushButton(arm_grid, text="-", grid=[2, row], width=6, height=3,
                   command=make_arm_burst(down_char, stop_char, label))

    app.display()


if __name__ == "__main__":
    run_gui()
