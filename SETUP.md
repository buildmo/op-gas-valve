# Operation Gas Valve — Software Setup

Everything you need to install on the Raspberry Pi before the event. Run the install script or follow the manual steps below.

## Table of Contents

- [Quick setup (run the script)](#quick-setup-run-the-script)
- [Python (Pilot team — GUI)](#python-pilot-team--gui)
- [Arduino IDE (Tank + Arm teams)](#arduino-ide-tank--arm-teams)
  - [Option A: Desktop app (recommended for teams)](#option-a-desktop-app-recommended-for-teams)
  - [Option B: arduino-cli (command line)](#option-b-arduino-cli-command-line)
- [Verify everything works](#verify-everything-works)

---

## Quick setup (run the script)

On each Raspberry Pi, run:

```bash
bash setup.sh
```

This installs Python dependencies, tkinter, and arduino-cli in one go. See below for what it does and how to install things manually.

---

## Python (Pilot team — GUI)

The GUI runs on the Raspberry Pi using Python 3. Two packages are needed:

- **guizero** — Simple GUI library (wraps Tkinter). Gives you buttons, sliders, and layout boxes.
- **pyserial** — Talks to the Arduino over USB serial.

### Install

```bash
# Python 3 should already be on Pi OS — check with:
python3 --version

# Install tkinter (GUI framework that guizero uses)
sudo apt-get update
sudo apt-get install -y python3-tk

# Install the Python packages
pip3 install guizero pyserial
```

### Test

```bash
python3 -c "from guizero import App; import serial; print('Ready')"
```

If you see `Ready`, you're good. If you get `ModuleNotFoundError: No module named 'tkinter'`, run the `apt-get install python3-tk` line above.

### Run a GUI script

```bash
python3 milestones/pilot/1_tank_gui.py
```

---

## Arduino IDE (Tank + Arm teams)

Teams need the Arduino IDE to write, compile, and upload sketches to the Arduino board. The **Serial Monitor** built into the IDE is how Tank and Arm teams will test their Day 1 code.

### Option A: Desktop app (recommended for teams)

1. Download from [https://www.arduino.cc/en/software](https://www.arduino.cc/en/software)
2. Install on the Pi or team laptop
3. Open a `.ino` file — the IDE opens the sketch automatically
4. Select board: **Tools > Board** (e.g., Arduino Uno or Arduino Mega)
5. Select port: **Tools > Port** (e.g., `/dev/ttyACM0`)
6. Click **Upload** (arrow button) to compile and flash
7. Click **Serial Monitor** (magnifying glass button) to send/receive characters — set baud to **9600**

The Serial Monitor is how you test: type a command character (e.g., `M` for forward) and hit Enter to see if the hardware responds.

### Option B: arduino-cli (command line)

The install script sets this up automatically. To do it manually:

```bash
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
sudo mv bin/arduino-cli /usr/local/bin/
arduino-cli core install arduino:avr
```

Compile and upload a sketch:
```bash
arduino-cli compile --fqbn arduino:avr:uno milestones/tank/1_motor_test
arduino-cli upload -p /dev/ttyACM0 --fqbn arduino:avr:uno milestones/tank/1_motor_test
```

Open a serial monitor:
```bash
arduino-cli monitor -p /dev/ttyACM0 --config baudrate=9600
```

> **Note:** If using an Arduino Mega, replace `arduino:avr:uno` with `arduino:avr:mega:cpu=atmega2560` in all commands.

---

## Verify everything works

After setup, run the test script to confirm all code compiles and parses:

```bash
bash test_milestones.sh
```

All items should show green checkmarks. To also upload each sketch to the Arduino:

```bash
bash test_milestones.sh --upload
```

Override board or port if needed:

```bash
BOARD=arduino:avr:mega:cpu=atmega2560 PORT=/dev/ttyUSB0 bash test_milestones.sh
```
