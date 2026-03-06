// ═══════════════════════════════════════════════════════════
// STEP 4 — Four Servos
//
// WHAT THIS DOES:
//   Controls all four joints of the robot arm:
//     Wrist  — rotates the gripper up/down
//     Elbow  — bends the middle of the arm
//     Arm    — raises/lowers the whole arm
//     Grip   — opens/closes the claw
//
//   You select which joint to move (1–4), then nudge it
//   with + or -. Press 0 to center all joints at once.
//
// WHAT YOU LEARN:
//   - Adding more servos (same pattern, different pins/limits)
//   - Selecting which thing to control
//   - Each joint has its own safe range of motion
//
// HOW TO TEST:
//   1. Upload to Arduino
//   2. Open Serial Monitor (9600 baud)
//   3. Send 1 → selects Wrist
//   4. Send + → wrist moves up 5°
//   5. Send 2 → selects Elbow
//   6. Send + → elbow moves up 5°
//   7. Send 0 → all four servos return to center
// ═══════════════════════════════════════════════════════════

#include <Servo.h>


// ── WRIST SETTINGS ───────────────────────────────────────
const int WRIST_PIN   = 10;
const int WRIST_MIN   = 45;    // lowest safe angle
const int WRIST_MAX   = 135;   // highest safe angle
const int WRIST_START = 90;    // power-on position

// ═══ NEW IN THIS STEP ═══════════════════════════════════
// ── ELBOW SETTINGS ───────────────────────────────────────
// The elbow has a smaller range of motion than the wrist.
const int ELBOW_PIN   = 12;
const int ELBOW_MIN   = 0;
const int ELBOW_MAX   = 90;
const int ELBOW_START = 45;

// ── ARM SETTINGS ─────────────────────────────────────────
// The arm (shoulder) lifts the whole arm assembly.
const int ARM_PIN   = 9;
const int ARM_MIN   = 70;
const int ARM_MAX   = 135;
const int ARM_START = 90;

// ── GRIP SETTINGS ────────────────────────────────────────
// The grip has full 180° range — fully open to fully closed.
const int GRIP_PIN   = 11;
const int GRIP_MIN   = 0;
const int GRIP_MAX   = 180;
const int GRIP_START = 90;
// ═════════════════════════════════════════════════════════


// ── SERVO OBJECTS ────────────────────────────────────────
// One "remote control" for each joint.
Servo wristServo;
Servo elbowServo;
Servo armServo;
Servo gripServo;

// ── ANGLE TRACKING ───────────────────────────────────────
// Each variable remembers where that joint is right now.
int wristAngle = WRIST_START;
int elbowAngle = ELBOW_START;
int armAngle   = ARM_START;
int gripAngle  = GRIP_START;

// ═══ NEW IN THIS STEP ═══════════════════════════════════
// ── WHICH JOINT IS SELECTED ──────────────────────────────
// 1 = Wrist, 2 = Elbow, 3 = Arm, 4 = Grip
// When you press + or -, only the selected joint moves.
int selectedServo = 1;
// ═════════════════════════════════════════════════════════


// ══════════════════════════════════════════════════════════
// SETUP
// ══════════════════════════════════════════════════════════
void setup() {
  Serial.begin(9600);

  // Connect each servo object to its physical pin
  // and move it to the starting position.
  wristServo.attach(WRIST_PIN);
  wristServo.write(wristAngle);

  elbowServo.attach(ELBOW_PIN);
  elbowServo.write(elbowAngle);

  armServo.attach(ARM_PIN);
  armServo.write(armAngle);

  gripServo.attach(GRIP_PIN);
  gripServo.write(gripAngle);

  Serial.println("Step 4 ready — 1-4 to select, + / - to nudge, 0 to center all");
}


// ══════════════════════════════════════════════════════════
// LOOP
// ══════════════════════════════════════════════════════════
void loop() {

  if (Serial.available() > 0) {
    char cmd = Serial.read();

    switch (cmd) {

      // ── SELECT WHICH JOINT ──
      // Press 1–4 to pick the joint you want to move next.
      case '1':
        selectedServo = 1;
        Serial.println("Selected: Wrist");
        break;

      case '2':
        selectedServo = 2;
        Serial.println("Selected: Elbow");
        break;

      case '3':
        selectedServo = 3;
        Serial.println("Selected: Arm");
        break;

      case '4':
        selectedServo = 4;
        Serial.println("Selected: Grip");
        break;

      // ── NUDGE THE SELECTED JOINT ──
      case '+':
        nudge(+5);
        break;

      case '-':
        nudge(-5);
        break;

      // ── CENTER ALL JOINTS ──
      case '0':
        centerAll();
        break;
    }
  }
}


// ═══ NEW IN THIS STEP ═══════════════════════════════════
// ── NUDGE FUNCTION ───────────────────────────────────────
// Moves whichever joint is currently selected by "amount" degrees.
// Positive = up/open, negative = down/close.
// Each joint is handled separately with its own limits.
void nudge(int amount) {
  switch (selectedServo) {

    case 1:  // Wrist
      wristAngle = wristAngle + amount;
      if (wristAngle < WRIST_MIN) wristAngle = WRIST_MIN;
      if (wristAngle > WRIST_MAX) wristAngle = WRIST_MAX;
      wristServo.write(wristAngle);
      Serial.print("Wrist: ");
      Serial.println(wristAngle);
      break;

    case 2:  // Elbow
      elbowAngle = elbowAngle + amount;
      if (elbowAngle < ELBOW_MIN) elbowAngle = ELBOW_MIN;
      if (elbowAngle > ELBOW_MAX) elbowAngle = ELBOW_MAX;
      elbowServo.write(elbowAngle);
      Serial.print("Elbow: ");
      Serial.println(elbowAngle);
      break;

    case 3:  // Arm
      armAngle = armAngle + amount;
      if (armAngle < ARM_MIN) armAngle = ARM_MIN;
      if (armAngle > ARM_MAX) armAngle = ARM_MAX;
      armServo.write(armAngle);
      Serial.print("Arm: ");
      Serial.println(armAngle);
      break;

    case 4:  // Grip
      gripAngle = gripAngle + amount;
      if (gripAngle < GRIP_MIN) gripAngle = GRIP_MIN;
      if (gripAngle > GRIP_MAX) gripAngle = GRIP_MAX;
      gripServo.write(gripAngle);
      Serial.print("Grip: ");
      Serial.println(gripAngle);
      break;
  }
}


// ── CENTER ALL ───────────────────────────────────────────
// Resets every joint to its starting position at once.
void centerAll() {
  wristAngle = WRIST_START;
  wristServo.write(wristAngle);

  elbowAngle = ELBOW_START;
  elbowServo.write(elbowAngle);

  armAngle = ARM_START;
  armServo.write(armAngle);

  gripAngle = GRIP_START;
  gripServo.write(gripAngle);

  Serial.println("All servos centered");
}
// ═════════════════════════════════════════════════════════
