# Operation Gas Valve — Software Setup

Everything you need to install on the Raspberry Pi before the event. Run the install script or follow the manual steps below.

## Table of Contents

- [Quick setup (run the script)](#quick-setup-run-the-script)
- [Python (Pilot team — GUI)](#python-pilot-team--gui)
- [Arduino IDE (Tank + Arm teams)](#arduino-ide-tank--arm-teams)
  - [Option A: Desktop app (recommended for teams)](#option-a-desktop-app-recommended-for-teams)
  - [Option B: arduino-cli (command line)](#option-b-arduino-cli-command-line)
- [Raspberry Pi Connect (Comms team)](#raspberry-pi-connect-comms-team)
- [Verify everything works](#verify-everything-works)

---

## Quick setup (run the script)

On each Raspberry Pi, run:

```bash
bash setup.sh
```

This installs Python dependencies, tkinter, arduino-cli, and Raspberry Pi Connect in one go. See below for what it does and how to install things manually.

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

## Raspberry Pi Connect (Comms team)

[Raspberry Pi Connect](https://www.raspberrypi.com/documentation/services/connect.html) lets you remotely access a Pi's desktop or shell from a browser. The Comms team uses this to control Kelsey's Pi from AU during the mission.

**Requirements:**
- Raspberry Pi 4, Pi 400, or Pi 5
- 64-bit Raspberry Pi OS Bookworm
- A [Raspberry Pi ID](https://id.raspberrypi.com/) (free account)

### Install

Pi Connect comes pre-installed on Raspberry Pi OS Desktop (Bookworm). If it's missing:

```bash
sudo apt-get update
sudo apt-get install -y rpi-connect
sudo reboot
```

### Link your Pi to your account

1. After reboot, click the **Raspberry Pi Connect icon** in the menu bar (top-right)
2. Select **"Turn On Raspberry Pi Connect"**
3. Your browser opens — sign in with your Raspberry Pi ID
4. The Pi is now linked to your account

### Connect remotely

1. On any computer, go to [connect.raspberrypi.com](https://connect.raspberrypi.com)
2. Sign in with the same Raspberry Pi ID
3. Click your Pi — you get a full remote desktop or shell in the browser

### Test the link

Both the AU Pi and Kelsey's Pi need to be set up and linked. To verify:

1. Set up Pi Connect on **both** Pis using the steps above
2. From the AU station, open [connect.raspberrypi.com](https://connect.raspberrypi.com)
3. Connect to Kelsey's Pi — confirm you can see her desktop and control it
4. Open a terminal on Kelsey's Pi via Connect and run a GUI script to verify the full chain works

> **Comms team Day 1 HR6 deadline:** The RPC link must be live. If Pi Connect won't connect, check that both Pis are on the internet and signed into the same Raspberry Pi ID. Set up a backup channel (Teams, phone) in case the primary link drops.

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
