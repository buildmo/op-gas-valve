# Operation Gas Valve — System Architecture

This document is for the **Integration team**. It describes how every component of the robot connects, what each team is responsible for, and how the pieces fit together. Use this to build your dependency map, track cross-team handoffs, and make informed Go/No-Go decisions.

---

## How the robot works (end to end)

```
 ┌──────────────────────────────────────────────────────────────┐
 │                     PILOT'S LAPTOP / PI                      │
 │                                                              │
 │   ┌──────────────────────────────────────────────────────┐   │
 │   │                    Python GUI                        │   │
 │   │                                                      │   │
 │   │   ┌────────────┐           ┌──────────────────────┐  │   │
 │   │   │  DRIVE      │           │  ARM                 │  │   │
 │   │   │  [Forward]  │           │  Wrist  [+] [-]     │  │   │
 │   │   │  [L][Stop][R]│          │  Elbow  [+] [-]     │  │   │
 │   │   │  [Backward] │           │  Arm    [+] [-]     │  │   │
 │   │   │             │           │  Grip   [+] [-]     │  │   │
 │   │   └────────────┘           └──────────────────────┘  │   │
 │   │                                                      │   │
 │   │   Tank speed [====|====] ms    Arm speed [====|====] ms  │
 │   └──────────────────────────────────────────────────────┘   │
 │                            │                                  │
 │                      Serial (USB)                             │
 │                      9600 baud                                │
 │                     Single characters                         │
 └────────────────────────────┬─────────────────────────────────┘
                              │
           ┌──────────────────┴──────────────────┐
           │           ARDUINO (Uno/Mega)         │
           │                                      │
           │   Serial input ──► handleCommand()   │
           │         │                   │        │
           │    Motor commands      Servo commands │
           │    (M/N/K/L/O)       (A-J, a/c/g/i) │
           │         │                   │        │
           │    Drive functions    tickServos()    │
           │    (PWM + direction)  (2° steps,     │
           │                        20ms interval) │
           └─────────┬───────────────────┬────────┘
                     │                   │
              ┌──────┴──────┐    ┌───────┴────────┐
              │  H-BRIDGE   │    │  SERVO RAIL    │
              │  (L298N)    │    │  (Buck Conv.)  │
              │             │    │                │
              │ IN1-IN4     │    │ 4 servos       │
              │ ENA, ENB    │    │ 5V 5A+ BEC    │
              │ 7-12V motor │    │ NEVER from     │
              │ supply      │    │ Arduino/Pi     │
              └──────┬──────┘    └───────┬────────┘
                     │                   │
              ┌──────┴──────┐    ┌───────┴────────┐
              │  DC MOTORS  │    │   SERVOS       │
              │  Left + Right│   │   Wrist (pin 10)│
              │             │    │   Elbow (pin 12)│
              │             │    │   Arm   (pin 9) │
              │             │    │   Grip  (pin 11)│
              └─────────────┘    └────────────────┘
```

**The signal chain:** GUI button click → serial character over USB → Arduino reads character → Arduino drives motors or moves servos.

That's it. One USB cable carries all commands. One Arduino runs everything.

---

## Team responsibilities and interfaces

### Who builds what

| Team | What they own | What they produce |
|---|---|---|
| **Tank** | Chassis, DC motors, H-bridge wiring | A tank that drives when it receives serial commands M/N/K/L/O |
| **Arm** | Servo mounting, Arduino wiring | An arm that moves when it receives serial commands A-J |
| **Pilot** | Python GUI on the Pi | A GUI that sends serial characters over USB when buttons are clicked |
| **Power** | Battery, buck converter, voltage rails | Stable power to all subsystems without brownouts |
| **Comms** | Pi Connect (RPC) link | A reliable video/audio channel to Kelsey for remote operation |
| **Grip** | 3D-printed end-effector | A gripper that fits the valve handle and mounts to the arm |

### How teams connect to each other

```
                    ┌────────────┐
                    │ INTEGRATION│  (you)
                    │ tracks all │
                    │ interfaces │
                    └─────┬──────┘
                          │
        ┌─────────────────┼─────────────────┐
        │                 │                 │
  ┌─────┴─────┐    ┌─────┴─────┐    ┌──────┴─────┐
  │   TANK    │    │    ARM    │    │   PILOT    │
  │           │    │           │    │            │
  │ H-bridge  │    │  Servos   │    │  GUI       │
  │ DC motors │    │  Arduino  │    │  Serial    │
  └─────┬─────┘    └─────┬─────┘    └──────┬─────┘
        │                │                  │
        │         ┌──────┴──────┐           │
        │         │    GRIP     │           │
        │         │ End-effector│           │
        │         │ mounts here │           │
        │         └─────────────┘           │
        │                                   │
  ┌─────┴───────────────────────────────────┴─────┐
  │                    POWER                       │
  │  3 isolated rails: 5V logic, 7-12V motor,     │
  │  5V servo — all sharing common ground          │
  └────────────────────────┬──────────────────────┘
                           │
                    ┌──────┴──────┐
                    │    COMMS    │
                    │  Pi Connect │
                    │  to Kelsey  │
                    └─────────────┘
```

---

## Critical interfaces (what teams must agree on)

These are the handoff points between teams. If any of these are wrong, integration breaks.

### 1. Serial protocol (Tank + Arm → Pilot)

All teams must use the **same serial commands**. This is non-negotiable — the GUI sends these characters and the Arduino must respond to them.

**Tank commands:**
| Character | Action |
|---|---|
| `M` | Forward |
| `N` | Backward |
| `K` | Left |
| `L` | Right |
| `O` | Stop all motors |

**Arm commands:**
| Character | Action | Character | Action |
|---|---|---|---|
| `A` | Wrist up | `a` | Wrist stop |
| `B` | Wrist down | `c` | Elbow stop |
| `C` | Elbow up | `g` | Arm stop |
| `D` | Elbow down | `i` | Grip stop |
| `G` | Arm up | | |
| `H` | Arm down | | |
| `I` | Grip open | | |
| `J` | Grip close | | |

**Serial settings:** 9600 baud, 8N1, over USB (`/dev/ttyACM0`).

> **Integration checkpoint:** If Tank or Arm changes a command character, Pilot must update the GUI to match. Track this.

### 2. Physical mounting (Tank ↔ Arm)

The arm base bolts to the chassis top plate. Both teams need to agree on:
- Bolt pattern and hole spacing
- Weight distribution (arm at front? centre?)
- Cable routing — servo wires must not pinch across full arm travel

> **Integration checkpoint:** Confirm this interface at Day 2 HR3 before bolting together.

### 3. Gripper interface (Arm ↔ Grip)

The gripper attaches to the arm's end-effector mount (last servo shaft). Teams must agree on:
- Shaft diameter and bolt pattern
- Torque rating (can the last servo turn the valve?)
- Cable clearance through full jaw travel

> **Integration checkpoint:** Chief Engineer CAD session at Day 2 HR2 locks this spec.

### 4. Power rails (Power → everyone)

Three isolated rails, all sharing a common ground bus:

| Rail | Voltage | Supplies | Source |
|---|---|---|---|
| Logic | 5V 3A | Raspberry Pi | USB-C power bank |
| Motor | 7-12V | H-bridge → DC motors | LiPo / NiMH battery |
| Servo | 5V 5A+ | All 4 servos | Buck converter / BEC |

**NEVER power servos from Arduino or Pi pins.** A single servo under load draws up to 2.5A. The Arduino 5V rail is rated 500mA total. Powering servos from it will permanently damage the board.

> **Integration checkpoint:** Power team must physically inspect both the AU and Kelsey builds to verify no servo is powered from Arduino/Pi pins.

---

## Day-by-day integration timeline

### Day 1 — Build independently, verify interfaces

| Hour | Integration focus |
|---|---|
| HR2 | Watch team self-organisation. Flag cross-team dependencies early. |
| HR3 | PDR: Capture each team's top 2 risks. Map all cross-team dependencies on a visible board. |
| HR5-7 | Circulate. Confirm: Tank has motors spinning. Arm has servos responding. Pilot has GUI sending serial. Power has voltage map documented. |
| HR6 | Confirm Comms has RPC link live. First relay sent to Kelsey. |
| HR7 | **Verify AU build is 2+ steps ahead of Kelsey.** |

**Day 1 integration deliverable:** Dependency map is live and visible. Every team can demonstrate their subsystem independently.

### Day 2 — Connect everything

| Hour | Integration focus |
|---|---|
| HR1-2 | Close gaps from Day 1. Chief Engineer CAD session (Grip ↔ Arm interface). |
| **HR3** | **HARD DEADLINE.** Tank must drive via code. Arm must move via code. If not: facilitator provides backup code. |
| HR3-5 | Mount arm on tank. Route cables. Connect GUI to Arduino. Test end-to-end: GUI click → motor/servo moves. |
| HR5-6 | Full system test on AU build. Fix issues. Start collecting Go/No-Go data. |
| HR6-7 | Pilot runs Kelsey's robot via RPC. All controls tested. |
| **HR7** | **GO/NO-GO DECISION.** |

**Go/No-Go checklist:**

| Question | GO means | NO-GO action |
|---|---|---|
| Remote-drive Kelsey's tank? | Pilot has driven it | Overnight troubleshoot or simplify |
| Remote-control Kelsey's arm? | All DOF responding via GUI | Reduce to essential DOF |
| Gripper engages valve? | Tested on real device | Adjust design or manual backup |
| Power lasts full mission? | Endurance confirmed | Plan shorter mission window |
| Comms reliable? | Backup channel also ready | Set up backup tonight |
| GUI works on Kelsey's robot? | Tested directly | Overnight fix or swap GUI |

### Day 3 — Mission day

| Hour | Integration focus |
|---|---|
| HR1 | Mission briefing. Confirm all teams on station. |
| HR2 | Pre-flight checks: all GUI controls confirmed on Kelsey's robot. |
| HR3 | Dress rehearsal: full sequence — navigate, deploy arm, turn valve — timed. |
| HR4 | Lunch. Kelsey positions robot. |
| **HR5** | **MISSION GO.** Pilot navigates Room 2. |
| **HR6** | **Arm deployment. Valve engagement. TURN THE VALVE.** |
| HR7 | Mission debrief. |

---

## What can go wrong (and who fixes it)

| Failure | First responder | Escalation |
|---|---|---|
| Motor spins wrong direction | Tank — swap H-bridge OUT wires | Check IN1/IN2 pin mapping in code |
| Servo doesn't respond | Arm — check PWM pin and power | Power — verify buck converter output above 4.8V |
| GUI click does nothing | Pilot — check serial port and baud | Tank/Arm — verify Arduino is running and listening |
| Servo jitters or resets | Power — current brownout on servo rail | Sequence servo movements, increase PWM ramp time |
| RPC link drops | Comms — reconnect primary, then backup | Facilitator provides backup hotspot after 3 min |
| Battery dies mid-mission | Power — call level via Comms | Integration decides: finish fast or abort + swap battery |
| GUI breaks during mission | Pilot — switch to backup GUI | Last resort: direct serial terminal to Arduino |

---

## The one diagram that matters

Print this. Put it on the wall. Every team should know where they sit.

```
┌─────────┐    serial     ┌─────────┐    wires    ┌──────────┐
│  GUI    │───(USB)──────►│ ARDUINO │────(PWM)───►│  MOTORS  │
│ (Pilot) │   chars       │(Tank+Arm)│            │  SERVOS  │
└─────────┘               └─────────┘            └──────────┘
     │                         │                       │
     │                         │                       │
  Pi Connect                                       ┌────┴────┐
  to Kelsey's Pi                                  │  POWER  │
  (Comms team)                                    │  3 rails │
                                                   └─────────┘
```

**If the GUI sends the right character, and the Arduino is running, and power is stable — the robot moves.** That's it. Everything else is making sure those three things are true.
