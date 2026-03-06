// ═══════════════════════════════════════════════════════════
// STEP 4 — Four Servos
// Learn: multiple servos, selecting which one to control
// Test: Serial Monitor — send 1-4 to select, + / - to nudge, 0 to center all
// ═══════════════════════════════════════════════════════════

#include <Servo.h>

// ── Wrist ──
const int WRIST_PIN   = 10;
const int WRIST_MIN   = 45;
const int WRIST_MAX   = 135;
const int WRIST_START = 90;

// ═══ NEW IN THIS STEP ═══
// ── Elbow ──
const int ELBOW_PIN   = 12;
const int ELBOW_MIN   = 0;
const int ELBOW_MAX   = 90;
const int ELBOW_START = 45;

// ── Arm ──
const int ARM_PIN   = 9;
const int ARM_MIN   = 70;
const int ARM_MAX   = 135;
const int ARM_START = 90;

// ── Grip ──
const int GRIP_PIN   = 11;
const int GRIP_MIN   = 0;
const int GRIP_MAX   = 180;
const int GRIP_START = 90;
// ═════════════════════════

Servo wristServo;
int   wristAngle = WRIST_START;

// ═══ NEW IN THIS STEP ═══
Servo elbowServo;
int   elbowAngle = ELBOW_START;

Servo armServo;
int   armAngle = ARM_START;

Servo gripServo;
int   gripAngle = GRIP_START;

int selectedServo = 1;  // 1=Wrist, 2=Elbow, 3=Arm, 4=Grip
// ═════════════════════════

void setup() {
  Serial.begin(9600);

  wristServo.attach(WRIST_PIN);
  wristServo.write(wristAngle);

  // ═══ NEW IN THIS STEP ═══
  elbowServo.attach(ELBOW_PIN);
  elbowServo.write(elbowAngle);

  armServo.attach(ARM_PIN);
  armServo.write(armAngle);

  gripServo.attach(GRIP_PIN);
  gripServo.write(gripAngle);
  // ═════════════════════════

  Serial.println("Step 4 ready — send 1-4 to select, + / - to nudge, 0 to center all");
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    // ═══ NEW IN THIS STEP ═══
    // Select which servo to control
    if (cmd == '1') {
      selectedServo = 1;
      Serial.println("Selected: Wrist");
    }
    else if (cmd == '2') {
      selectedServo = 2;
      Serial.println("Selected: Elbow");
    }
    else if (cmd == '3') {
      selectedServo = 3;
      Serial.println("Selected: Arm");
    }
    else if (cmd == '4') {
      selectedServo = 4;
      Serial.println("Selected: Grip");
    }
    // ═════════════════════════

    else if (cmd == '+') {
      nudge(+5);
    }
    else if (cmd == '-') {
      nudge(-5);
    }
    else if (cmd == '0') {
      centerAll();
    }
  }
}

// ═══ NEW IN THIS STEP ═══
void nudge(int amount) {
  if (selectedServo == 1) {
    wristAngle = wristAngle + amount;
    if (wristAngle < WRIST_MIN) wristAngle = WRIST_MIN;
    if (wristAngle > WRIST_MAX) wristAngle = WRIST_MAX;
    wristServo.write(wristAngle);
    Serial.print("Wrist: ");
    Serial.println(wristAngle);
  }
  else if (selectedServo == 2) {
    elbowAngle = elbowAngle + amount;
    if (elbowAngle < ELBOW_MIN) elbowAngle = ELBOW_MIN;
    if (elbowAngle > ELBOW_MAX) elbowAngle = ELBOW_MAX;
    elbowServo.write(elbowAngle);
    Serial.print("Elbow: ");
    Serial.println(elbowAngle);
  }
  else if (selectedServo == 3) {
    armAngle = armAngle + amount;
    if (armAngle < ARM_MIN) armAngle = ARM_MIN;
    if (armAngle > ARM_MAX) armAngle = ARM_MAX;
    armServo.write(armAngle);
    Serial.print("Arm: ");
    Serial.println(armAngle);
  }
  else if (selectedServo == 4) {
    gripAngle = gripAngle + amount;
    if (gripAngle < GRIP_MIN) gripAngle = GRIP_MIN;
    if (gripAngle > GRIP_MAX) gripAngle = GRIP_MAX;
    gripServo.write(gripAngle);
    Serial.print("Grip: ");
    Serial.println(gripAngle);
  }
}

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
// ═════════════════════════
