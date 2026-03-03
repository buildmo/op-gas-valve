from guizero import App, Box, Text, PushButton, CheckBox
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
        send("O")
        ser.close()
        app.destroy()

    app = App(title="Op Gas Valve - Robot Controller", width=800, height=600)
    app.when_closed = on_close

    # ── Top bar ────────────────────────────────────────
    top_bar = Box(app, align="top", width="fill")
    hold_mode = CheckBox(top_bar, text="Hold Mode", align="right")
    hold_mode.value = True
    burst_timer = {"id": None}
    hold_timer = {"id": None}

    # ── Main panels (drive + arm side by side) ─────────
    panels = Box(app, align="top", width="fill", height="fill",
                 layout="grid")

    # ── Drive panel ────────────────────────────────────
    tank_box = Box(panels, grid=[0, 0], border=True)
    tank_box.set_border(1, "#999999")
    Text(tank_box, text="DRIVE", size=14, font="sans-serif", bold=True)

    dpad = Box(tank_box, layout="grid")

    # ── Tank helpers ───────────────────────────────────

    def bind_hold(btn, char):
        def start():
            if hold_timer["id"] is not None:
                app.cancel(hold_timer["id"])
            send(char)
            hold_timer["id"] = app.repeat(100, lambda: send(char))

        def stop():
            if hold_timer["id"] is not None:
                app.cancel(hold_timer["id"])
                hold_timer["id"] = None
            send("O")

        btn.when_left_button_pressed = start
        btn.update_command(stop)

    def bind_burst(btn, char):
        btn.when_left_button_pressed = None
        btn.when_left_button_released = None

        def burst():
            if burst_timer["id"] is not None:
                app.cancel(burst_timer["id"])
            send(char)
            burst_timer["id"] = app.after(500, lambda: send("O"))

        btn.update_command(burst)

    def rebind_tank():
        for btn, char in tank_buttons:
            if hold_mode.value:
                bind_hold(btn, char)
            else:
                bind_burst(btn, char)

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
    tank_buttons = []
    for label, pos in tank_grid.items():
        btn = PushButton(dpad, text=label, grid=pos, width=12, height=3)
        tank_buttons.append((btn, tank_cmds[label]))

    stop_btn = PushButton(dpad, text="Stop", grid=[1, 1], width=12, height=3)
    stop_btn.update_command(lambda: send("O"))
    stop_btn.bg = "#ff6666"

    hold_mode.update_command(rebind_tank)
    rebind_tank()

    # ── Arm panel ──────────────────────────────────────
    arm_box = Box(panels, grid=[1, 0], border=True)
    arm_box.set_border(1, "#999999")
    Text(arm_box, text="ARM", size=14, font="sans-serif", bold=True)

    arm_grid = Box(arm_box, layout="grid")

    # ── Arm helpers ────────────────────────────────────

    def make_arm_press(start_char):
        return lambda: send(start_char)

    def make_arm_release(stop_char):
        return lambda: send(stop_char)

    arm_cmds = {
        "Wrist":  ("A", "B", "a"),
        "Elbow":  ("C", "D", "c"),
        "Arm":    ("G", "H", "g"),
        "Grip":   ("I", "J", "i"),
    }

    for row, (label, chars) in enumerate(arm_cmds.items()):
        up_char, down_char, stop_char = chars
        Text(arm_grid, text=label, grid=[0, row], width=8, align="left")

        plus_btn = PushButton(arm_grid, text="+", grid=[1, row],
                              width=6, height=3)
        plus_btn.when_left_button_pressed = make_arm_press(up_char)
        plus_btn.when_left_button_released = make_arm_release(stop_char)
        plus_btn.update_command(lambda: None)

        minus_btn = PushButton(arm_grid, text="-", grid=[2, row],
                               width=6, height=3)
        minus_btn.when_left_button_pressed = make_arm_press(down_char)
        minus_btn.when_left_button_released = make_arm_release(stop_char)
        minus_btn.update_command(lambda: None)

    app.display()


if __name__ == "__main__":
    run_gui()
