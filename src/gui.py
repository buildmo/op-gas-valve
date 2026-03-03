from guizero import App, Text, PushButton, CheckBox
import serial
import time


def run_gui(config=None):
    # Serial config - can be overridden per Pi via config yaml
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

    # Command map: label -> serial char
    tank_cmds = {
        "Forward": "M", "Backward": "N",
        "Left": "K", "Right": "L", "Stop": "O",
    }

    arm_cmds = {
        "Arm":    ("G", "H"),
        "Elbow":  ("C", "D"),
        "Wrist":  ("A", "B"),
        "Hand":   ("E", "F"),
        "Device": ("I", "J"),
    }

    app = App(title="Op Gas Valve - Robot Controller", layout="grid")
    app.when_closed = on_close

    # ── Hold / Burst mode toggle ─────────────────────────
    hold_mode = CheckBox(app, text="Hold Mode", grid=[0, 0, 5, 1])
    hold_mode.value = True

    # Track scheduled burst-stop so we can cancel it
    burst_timer = {"id": None}

    # ── Tank button helpers ──────────────────────────────

    def bind_hold(btn, char):
        """Hold mode: move while pressed, stop on release."""
        btn.when_left_button_pressed = lambda: send(char)
        btn.when_left_button_released = lambda: send("O")
        btn.update_command(lambda: None)

    def bind_burst(btn, char):
        """Burst mode: click sends move, auto-stops after 500ms."""
        btn.when_left_button_pressed = None
        btn.when_left_button_released = None

        def burst():
            if burst_timer["id"] is not None:
                app.cancel(burst_timer["id"])
            send(char)
            burst_timer["id"] = app.after(500, lambda: send("O"))

        btn.update_command(burst)

    def rebind_tank():
        """Switch all tank buttons between hold and burst mode."""
        for btn, char in tank_buttons:
            if hold_mode.value:
                bind_hold(btn, char)
            else:
                bind_burst(btn, char)

    # Create tank buttons (no command yet — rebind_tank sets them up)
    tank_buttons = []
    tank_grid = {
        "Forward":  [2, 1],
        "Backward": [2, 3],
        "Left":     [1, 2],
        "Right":    [3, 2],
    }
    for label, pos in tank_grid.items():
        btn = PushButton(app, text=label, grid=pos, width=20, height=7)
        tank_buttons.append((btn, tank_cmds[label]))

    PushButton(app, command=lambda: send("O"), text="Stop", grid=[2, 2], width=20, height=7)

    hold_mode.update_command(rebind_tank)
    rebind_tank()  # apply initial mode

    # ── Arm button helpers (repeating callback) ──────────

    arm_timers = {}

    def arm_press(char, key):
        """Start sending the servo command every 80ms."""
        send(char)
        arm_timers[key] = app.repeat(80, lambda: send(char))

    def arm_release(key):
        """Stop the repeating servo command."""
        if key in arm_timers:
            app.cancel(arm_timers[key])
            del arm_timers[key]

    # Arm controls
    arm_layout = [
        ("Wrist",  4, 1),
        ("Elbow",  4, 3),
        ("Arm",    4, 5),
        ("Hand",   6, 3),
        ("Device", 6, 1),
    ]

    for label, col, row in arm_layout:
        plus_char, minus_char = arm_cmds[label]
        Text(app, text=label, grid=[col, row], width=20, height=7)

        plus_key = f"{label}_plus"
        plus_btn = PushButton(app, text="+", grid=[col+1, row], width=20, height=7)
        plus_btn.when_left_button_pressed = lambda c=plus_char, k=plus_key: arm_press(c, k)
        plus_btn.when_left_button_released = lambda k=plus_key: arm_release(k)
        plus_btn.update_command(lambda: None)

        minus_key = f"{label}_minus"
        minus_btn = PushButton(app, text="-", grid=[col+1, row+1], width=20, height=7)
        minus_btn.when_left_button_pressed = lambda c=minus_char, k=minus_key: arm_press(c, k)
        minus_btn.when_left_button_released = lambda k=minus_key: arm_release(k)
        minus_btn.update_command(lambda: None)

    app.display()


if __name__ == "__main__":
    run_gui()
