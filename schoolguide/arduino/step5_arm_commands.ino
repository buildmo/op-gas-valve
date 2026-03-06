// ═══════════════════════════════════════════════════════════
// STEP 5 — Arm Commands (Letter-Based + Smooth Stepping)
//
// WHAT THIS DOES:
//   Instead of selecting a joint and pressing +/-, each joint
//   now has its own letter commands. This lets the GUI send
//   precise commands directly (no "select then nudge" needed).
//
//   The servo also moves SMOOTHLY now — instead of jumping
//   5° at a time, it glides in small steps using a timer.
//
// WHAT YOU LEARN:
//   - Direction variables (-1 / 0 / +1) for smooth motion
//   - millis() timing — the Arduino's built-in stopwatch
//   - Step size control (bigger steps = faster movement)
//
// HOW TO TEST:
//   1. Upload to Arduino
//   2. Open Serial Monitor (9600 baud)
//   3. Send A → wrist starts moving up (continuously!)
//   4. Send a → wrist stops
//   5. Send 5 → set step size to 5 degrees (faster)
//   6. Send A → wrist moves up faster now
//   7. Send a → wrist stops
//
// COMMANDS:
//   Wrist:  A = up    B = down    a = stop
//   Elbow:  C = up    D = down    c = stop
//   Arm:    G = up    H = down    g = stop
//   Grip:   I = open  J = close   i = stop
//   Speed:  1 / 2 / 3 / 4 / 5 = degrees per step
// ═══════════════════════════════════════════════════════════

#include <Servo.h>


// ── WRIST SETTINGS ───────────────────────────────────────
const int WRIST_PIN   = 10;
const int WRIST_MIN   = 45;
const int WRIST_MAX   = 135;
const int WRIST_START = 90;

// ── ELBOW SETTINGS ───────────────────────────────────────
const int ELBOW_PIN   = 12;
const int ELBOW_MIN   = 0;
const int ELBOW_MAX   = 90;
const int ELBOW_START = 45;

// ── ARM SETTINGS ─────────────────────────────────────────
const int ARM_PIN   = 9;
const int ARM_MIN   = 70;
const int ARM_MAX   = 135;
const int ARM_START = 90;

// ── GRIP SETTINGS ────────────────────────────────────────
const int GRIP_PIN   = 11;
const int GRIP_MIN   = 0;
const int GRIP_MAX   = 180;
const int GRIP_START = 90;


// ── SERVO OBJECTS (one per joint) ────────────────────────
Servo wristServo;
Servo elbowServo;
Servo armServo;
Servo gripServo;

// ── ANGLE TRACKING (current position of each joint) ─────
int wristAngle = WRIST_START;
int elbowAngle = ELBOW_START;
int armAngle   = ARM_START;
int gripAngle  = GRIP_START;


// ═══ NEW IN THIS STEP ═══════════════════════════════════

// ── DIRECTION VARIABLES ──────────────────────────────────
// Each joint has a direction:
//   +1 = moving up (or opening for grip)
//    0 = stopped / idle
//   -1 = moving down (or closing for grip)
//
// When you send 'A', wristDir becomes +1 and the wrist
// starts moving up. When you send 'a', it becomes 0 and stops.
int wristDir = 0;
int elbowDir = 0;
int armDir   = 0;
int gripDir  = 0;

// ── STEP SIZE ────────────────────────────────────────────
// How many degrees to move each "tick" (each step).
// Range: 1 (slow, precise) to 5 (fast, coarse).
// Default is 2 degrees per tick.
int stepDeg = 2;

// ── TIMING ───────────────────────────────────────────────
// SERVO_INTERVAL = how many milliseconds between steps.
// 20ms = 50 steps per second — smooth and responsive.
//
// millis() returns the number of milliseconds since the
// Arduino powered on. We use it like a stopwatch:
// "Has enough time passed since the last step?"
const unsigned long SERVO_INTERVAL = 20;
unsigned long lastServoTick = 0;

// ═════════════════════════════════════════════════════════


// ══════════════════════════════════════════════════════════
// SETUP
// ══════════════════════════════════════════════════════════
void setup() {
  Serial.begin(9600);

  // Connect and position each servo.
  wristServo.attach(WRIST_PIN);
  wristServo.write(wristAngle);

  elbowServo.attach(ELBOW_PIN);
  elbowServo.write(elbowAngle);

  armServo.attach(ARM_PIN);
  armServo.write(armAngle);

  gripServo.attach(GRIP_PIN);
  gripServo.write(gripAngle);

  // Wait for servos to reach their start positions.
  delay(500);

  Serial.println("Step 5 ready — A/B/a C/D/c G/H/g I/J/i, 1-5 step size");
}


// ══════════════════════════════════════════════════════════
// LOOP
// ══════════════════════════════════════════════════════════
void loop() {

  // ── Read any incoming command ──
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    handleCommand(cmd);
  }

  // ═══ NEW IN THIS STEP ═════════════════════════════════
  // ── SMOOTH STEPPING WITH millis() ──────────────────────
  // Instead of moving instantly when a command arrives,
  // we move a tiny bit every 20 milliseconds.
  // This creates smooth, continuous motion.
  unsigned long now = millis();
  if (now - lastServoTick >= SERVO_INTERVAL) {
    lastServoTick = now;
    moveServos();
  }
  // ═══════════════════════════════════════════════════════
}


// ═══ NEW IN THIS STEP ═══════════════════════════════════

// ══════════════════════════════════════════════════════════
// HANDLE COMMAND — decide what to do with each letter
// ══════════════════════════════════════════════════════════
void handleCommand(char cmd) {

  switch (cmd) {

    // ── WRIST COMMANDS ──
    // Uppercase = start moving, lowercase = stop
    case 'A':                    // Wrist UP
      wristDir = 1;
      Serial.println("Wrist UP");
      break;
    case 'B':                    // Wrist DOWN
      wristDir = -1;
      Serial.println("Wrist DOWN");
      break;
    case 'a':                    // Wrist STOP
      wristDir = 0;
      Serial.println("Wrist STOP");
      break;

    // ── ELBOW COMMANDS ──
    case 'C':                    // Elbow UP
      elbowDir = 1;
      Serial.println("Elbow UP");
      break;
    case 'D':                    // Elbow DOWN
      elbowDir = -1;
      Serial.println("Elbow DOWN");
      break;
    case 'c':                    // Elbow STOP
      elbowDir = 0;
      Serial.println("Elbow STOP");
      break;

    // ── ARM COMMANDS ──
    case 'G':                    // Arm UP
      armDir = 1;
      Serial.println("Arm UP");
      break;
    case 'H':                    // Arm DOWN
      armDir = -1;
      Serial.println("Arm DOWN");
      break;
    case 'g':                    // Arm STOP
      armDir = 0;
      Serial.println("Arm STOP");
      break;

    // ── GRIP COMMANDS ──
    case 'I':                    // Grip OPEN
      gripDir = 1;
      Serial.println("Grip OPEN");
      break;
    case 'J':                    // Grip CLOSE
      gripDir = -1;
      Serial.println("Grip CLOSE");
      break;
    case 'i':                    // Grip STOP
      gripDir = 0;
      Serial.println("Grip STOP");
      break;

    // ── STEP SIZE COMMANDS ──
    // Bigger number = faster movement (more degrees per tick).
    case '1': stepDeg = 1; Serial.println("Step size: 1 deg"); break;
    case '2': stepDeg = 2; Serial.println("Step size: 2 deg"); break;
    case '3': stepDeg = 3; Serial.println("Step size: 3 deg"); break;
    case '4': stepDeg = 4; Serial.println("Step size: 4 deg"); break;
    case '5': stepDeg = 5; Serial.println("Step size: 5 deg"); break;
  }
}


// ══════════════════════════════════════════════════════════
// MOVE SERVOS — called every 20ms by the timer in loop()
// ══════════════════════════════════════════════════════════
// For each joint: if the direction is not zero, calculate
// the next angle, clamp it within limits, and move the servo.
void moveServos() {

  // ── WRIST ──
  if (wristDir != 0) {
    // Calculate the next angle:
    //   current angle + (direction * step size)
    //   Example: 90 + (1 * 2) = 92 degrees
    int next = wristAngle + (wristDir * stepDeg);
    // Clamp: don't go below min or above max
    if (next < WRIST_MIN) next = WRIST_MIN;
    if (next > WRIST_MAX) next = WRIST_MAX;
    // Only move if the angle actually changed
    // (prevents unnecessary writes when at a limit)
    if (next != wristAngle) {
      wristAngle = next;
      wristServo.write(wristAngle);
    }
  }

  // ── ELBOW ──
  if (elbowDir != 0) {
    int next = elbowAngle + (elbowDir * stepDeg);
    if (next < ELBOW_MIN) next = ELBOW_MIN;
    if (next > ELBOW_MAX) next = ELBOW_MAX;
    if (next != elbowAngle) {
      elbowAngle = next;
      elbowServo.write(elbowAngle);
    }
  }

  // ── ARM ──
  if (armDir != 0) {
    int next = armAngle + (armDir * stepDeg);
    if (next < ARM_MIN) next = ARM_MIN;
    if (next > ARM_MAX) next = ARM_MAX;
    if (next != armAngle) {
      armAngle = next;
      armServo.write(armAngle);
    }
  }

  // ── GRIP ──
  if (gripDir != 0) {
    int next = gripAngle + (gripDir * stepDeg);
    if (next < GRIP_MIN) next = GRIP_MIN;
    if (next > GRIP_MAX) next = GRIP_MAX;
    if (next != gripAngle) {
      gripAngle = next;
      gripServo.write(gripAngle);
    }
  }
}

// ═════════════════════════════════════════════════════════
