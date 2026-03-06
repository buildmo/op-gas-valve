// ═══════════════════════════════════════════════════════════
// STEP 5 — Arm Commands (Letter-Based + Smooth Stepping)
// Learn: direction variables, millis() timing, step size control
// Test: Serial Monitor — A/B/a (wrist), C/D/c (elbow),
//       G/H/g (arm), I/J/i (grip), 1-5 (step size)
// ═══════════════════════════════════════════════════════════

#include <Servo.h>

// ── Wrist ──
const int WRIST_PIN   = 10;
const int WRIST_MIN   = 45;
const int WRIST_MAX   = 135;
const int WRIST_START = 90;

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

// ── Servo objects ──
Servo wristServo;
Servo elbowServo;
Servo armServo;
Servo gripServo;

// ── Angle tracking ──
int wristAngle = WRIST_START;
int elbowAngle = ELBOW_START;
int armAngle   = ARM_START;
int gripAngle  = GRIP_START;

// ═══ NEW IN THIS STEP ═══
// ── Direction variables: -1 = down, 0 = idle, +1 = up ──
int wristDir = 0;
int elbowDir = 0;
int armDir   = 0;
int gripDir  = 0;

// ── Step size and timing ──
int stepDeg = 2;
const unsigned long SERVO_INTERVAL = 20;  // ms between steps
unsigned long lastServoTick = 0;
// ═════════════════════════

void setup() {
  Serial.begin(9600);

  wristServo.attach(WRIST_PIN);
  wristServo.write(wristAngle);

  elbowServo.attach(ELBOW_PIN);
  elbowServo.write(elbowAngle);

  armServo.attach(ARM_PIN);
  armServo.write(armAngle);

  gripServo.attach(GRIP_PIN);
  gripServo.write(gripAngle);

  delay(500);
  Serial.println("Step 5 ready — A/B/a C/D/c G/H/g I/J/i, 1-5 step size");
}

void loop() {
  // ── Read serial ──
  if (Serial.available() > 0) {
    char cmd = Serial.read();
    handleCommand(cmd);
  }

  // ═══ NEW IN THIS STEP ═══
  // ── Smooth stepping with millis() ──
  unsigned long now = millis();
  if (now - lastServoTick >= SERVO_INTERVAL) {
    lastServoTick = now;
    moveServos();
  }
  // ═════════════════════════
}

// ═══ NEW IN THIS STEP ═══
void handleCommand(char cmd) {
  // ── Wrist: A = up, B = down, a = stop ──
  if (cmd == 'A') {
    wristDir = 1;
    Serial.println("Wrist UP");
  }
  else if (cmd == 'B') {
    wristDir = -1;
    Serial.println("Wrist DOWN");
  }
  else if (cmd == 'a') {
    wristDir = 0;
    Serial.println("Wrist STOP");
  }

  // ── Elbow: C = up, D = down, c = stop ──
  else if (cmd == 'C') {
    elbowDir = 1;
    Serial.println("Elbow UP");
  }
  else if (cmd == 'D') {
    elbowDir = -1;
    Serial.println("Elbow DOWN");
  }
  else if (cmd == 'c') {
    elbowDir = 0;
    Serial.println("Elbow STOP");
  }

  // ── Arm: G = up, H = down, g = stop ──
  else if (cmd == 'G') {
    armDir = 1;
    Serial.println("Arm UP");
  }
  else if (cmd == 'H') {
    armDir = -1;
    Serial.println("Arm DOWN");
  }
  else if (cmd == 'g') {
    armDir = 0;
    Serial.println("Arm STOP");
  }

  // ── Grip: I = open, J = close, i = stop ──
  else if (cmd == 'I') {
    gripDir = 1;
    Serial.println("Grip OPEN");
  }
  else if (cmd == 'J') {
    gripDir = -1;
    Serial.println("Grip CLOSE");
  }
  else if (cmd == 'i') {
    gripDir = 0;
    Serial.println("Grip STOP");
  }

  // ── Step size: '1'-'5' ──
  else if (cmd == '1') { stepDeg = 1; Serial.println("Step: 1"); }
  else if (cmd == '2') { stepDeg = 2; Serial.println("Step: 2"); }
  else if (cmd == '3') { stepDeg = 3; Serial.println("Step: 3"); }
  else if (cmd == '4') { stepDeg = 4; Serial.println("Step: 4"); }
  else if (cmd == '5') { stepDeg = 5; Serial.println("Step: 5"); }
}

void moveServos() {
  // ── Wrist ──
  if (wristDir != 0) {
    int next = wristAngle + (wristDir * stepDeg);
    if (next < WRIST_MIN) next = WRIST_MIN;
    if (next > WRIST_MAX) next = WRIST_MAX;
    if (next != wristAngle) {
      wristAngle = next;
      wristServo.write(wristAngle);
    }
  }

  // ── Elbow ──
  if (elbowDir != 0) {
    int next = elbowAngle + (elbowDir * stepDeg);
    if (next < ELBOW_MIN) next = ELBOW_MIN;
    if (next > ELBOW_MAX) next = ELBOW_MAX;
    if (next != elbowAngle) {
      elbowAngle = next;
      elbowServo.write(elbowAngle);
    }
  }

  // ── Arm ──
  if (armDir != 0) {
    int next = armAngle + (armDir * stepDeg);
    if (next < ARM_MIN) next = ARM_MIN;
    if (next > ARM_MAX) next = ARM_MAX;
    if (next != armAngle) {
      armAngle = next;
      armServo.write(armAngle);
    }
  }

  // ── Grip ──
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
// ═════════════════════════
