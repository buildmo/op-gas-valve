// ═══════════════════════════════════════════════════════════
// STEP 3 — One Servo (Wrist)
// Learn: Servo library, angle tracking, constrain to limits
// Test: Serial Monitor — send + / - / 0
// ═══════════════════════════════════════════════════════════

#include <Servo.h>

// ── Wrist servo config ──
const int WRIST_PIN   = 10;
const int WRIST_MIN   = 45;
const int WRIST_MAX   = 135;
const int WRIST_START = 90;

Servo wristServo;
int   wristAngle = WRIST_START;

void setup() {
  Serial.begin(9600);

  wristServo.attach(WRIST_PIN);
  wristServo.write(wristAngle);

  Serial.println("Step 3 ready — send + / - / 0");
  Serial.print("Wrist angle: ");
  Serial.println(wristAngle);
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    if (cmd == '+') {
      wristAngle = wristAngle + 5;
      if (wristAngle > WRIST_MAX) wristAngle = WRIST_MAX;
      wristServo.write(wristAngle);
      Serial.print("Wrist angle: ");
      Serial.println(wristAngle);
    }
    else if (cmd == '-') {
      wristAngle = wristAngle - 5;
      if (wristAngle < WRIST_MIN) wristAngle = WRIST_MIN;
      wristServo.write(wristAngle);
      Serial.print("Wrist angle: ");
      Serial.println(wristAngle);
    }
    else if (cmd == '0') {
      wristAngle = WRIST_START;
      wristServo.write(wristAngle);
      Serial.print("Wrist centered: ");
      Serial.println(wristAngle);
    }
  }
}
