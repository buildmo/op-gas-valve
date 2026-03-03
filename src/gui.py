from guizero import App, Text, PushButton
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

    # Tank controls
    PushButton(app, command=lambda: send("M"), text="Forward",  grid=[2,1], width=20, height=7)
    PushButton(app, command=lambda: send("N"), text="Backward", grid=[2,3], width=20, height=7)
    PushButton(app, command=lambda: send("K"), text="Left",     grid=[1,2], width=20, height=7)
    PushButton(app, command=lambda: send("L"), text="Right",    grid=[3,2], width=20, height=7)
    PushButton(app, command=lambda: send("O"), text="Stop",     grid=[2,2], width=20, height=7)

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
        PushButton(app, command=lambda c=plus_char:  send(c), text="+", grid=[col+1, row],   width=20, height=7)
        PushButton(app, command=lambda c=minus_char: send(c), text="-", grid=[col+1, row+1], width=20, height=7)

    app.display()


if __name__ == "__main__":
    run_gui()
