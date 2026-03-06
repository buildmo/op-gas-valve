// ═══════════════════════════════════════════════════════════
// STEP 3 — One Servo (Wrist)
//
// WHAT THIS DOES:
//   Controls a single servo motor (the wrist joint).
//   A servo is different from a drive motor — instead of
//   spinning continuously, it moves to an exact angle
//   (like a clock hand pointing to a specific position).
//
// WHAT YOU LEARN:
//   - The Servo library (built into Arduino)
//   - Tracking an angle with a variable
//   - Setting min/max limits so the servo doesn't over-rotate
//
// HOW TO TEST:
//   1. Upload to Arduino
//   2. Open Serial Monitor (9600 baud)
//   3. Send + → servo nudges up 5 degrees
//   4. Send - → servo nudges down 5 degrees
//   5. Send 0 → servo returns to center (90°)
//   6. Watch the angle print after each move
// ═══════════════════════════════════════════════════════════


// This line loads the Servo library — a collection of code
// that someone already wrote so we don't have to figure out
// the low-level signal timing ourselves.
#include <Servo.h>


// ── WRIST SERVO SETTINGS ─────────────────────────────────
// PIN   = which Arduino pin the servo's signal wire is on
// MIN   = lowest angle we allow (protects the hardware)
// MAX   = highest angle we allow
// START = where the servo begins when powered on
const int WRIST_PIN   = 10;
const int WRIST_MIN   = 45;    // degrees — don't go below this
const int WRIST_MAX   = 135;   // degrees — don't go above this
const int WRIST_START = 90;    // degrees — center position


// ── SERVO OBJECT ─────────────────────────────────────────
// "Servo wristServo" creates a servo controller.
// Think of it as a remote control — you tell it an angle
// and it moves the physical servo to that position.
Servo wristServo;

// This variable remembers the current angle.
// We change this number, then tell the servo to go there.
int wristAngle = WRIST_START;


// ══════════════════════════════════════════════════════════
// SETUP
// ══════════════════════════════════════════════════════════
void setup() {
  Serial.begin(9600);

  // "attach" connects our servo object to the physical pin.
  // After this line, we can use wristServo.write(angle) to move it.
  wristServo.attach(WRIST_PIN);

  // Move the servo to its starting position.
  wristServo.write(wristAngle);

  Serial.println("Step 3 ready — send + / - / 0");
  Serial.print("Wrist angle: ");
  Serial.println(wristAngle);
}


// ══════════════════════════════════════════════════════════
// LOOP
// ══════════════════════════════════════════════════════════
void loop() {

  if (Serial.available() > 0) {
    char cmd = Serial.read();

    switch (cmd) {

      // ── NUDGE UP ──
      // Add 5 degrees to the angle.
      // If the result would exceed WRIST_MAX, clamp it.
      // "Clamping" = forcing a number to stay within bounds.
      case '+':
        wristAngle = wristAngle + 5;
        if (wristAngle > WRIST_MAX) {
          wristAngle = WRIST_MAX;   // don't exceed the limit
        }
        wristServo.write(wristAngle);
        Serial.print("Wrist angle: ");
        Serial.println(wristAngle);
        break;

      // ── NUDGE DOWN ──
      // Subtract 5 degrees. Clamp at WRIST_MIN.
      case '-':
        wristAngle = wristAngle - 5;
        if (wristAngle < WRIST_MIN) {
          wristAngle = WRIST_MIN;   // don't go below the limit
        }
        wristServo.write(wristAngle);
        Serial.print("Wrist angle: ");
        Serial.println(wristAngle);
        break;

      // ── CENTER ──
      // Jump straight back to the starting position.
      case '0':
        wristAngle = WRIST_START;
        wristServo.write(wristAngle);
        Serial.print("Wrist centered: ");
        Serial.println(wristAngle);
        break;
    }
  }
}
