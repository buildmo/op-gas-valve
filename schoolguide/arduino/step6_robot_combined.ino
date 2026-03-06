// ═══════════════════════════════════════════════════════════
// STEP 6 — Full Robot (Tank + Arm Combined)
// This is the final working code — same behavior as final_arduino
// Test: pair with gui/step5_full_gui.py
// ═══════════════════════════════════════════════════════════

#include <Servo.h>

// ── Motor speeds ──
#define SPEED_L 200
#define SPEED_R 220

// ── Motor pins ──
const int EN_A = 6;
const int IN1  = 4;
const int IN2  = 3;
const int IN3  = 7;
const int IN4  = 8;
const int EN_B = 5;

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

// ── Direction: -1 = down, 0 = idle, +1 = up ──
int wristDir = 0;
int elbowDir = 0;
int armDir   = 0;
int gripDir  = 0;

// ── Step size and timing ──
int stepDeg = 2;
const unsigned long SERVO_INTERVAL = 20;
unsigned long lastServoTick = 0;

// ═══════════════════════════════════════════════════════════
void setup() {
  Serial.begin(9600);

  // ── Motor pins ──
  pinMode(EN_A, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EN_B, OUTPUT);

  // ── Servos ──
  wristServo.attach(WRIST_PIN);
  wristServo.write(wristAngle);

  elbowServo.attach(ELBOW_PIN);
  elbowServo.write(elbowAngle);

  armServo.attach(ARM_PIN);
  armServo.write(armAngle);

  gripServo.attach(GRIP_PIN);
  gripServo.write(gripAngle);

  stopMotors();
  delay(500);
}

// ═══════════════════════════════════════════════════════════
void loop() {
  // ═══ NEW IN THIS STEP ═══
  // ── Serial buffer drain: only act on latest byte ──
  char cmd = 0;
  while (Serial.available() > 0) {
    cmd = Serial.read();
  }
  // ═════════════════════════

  if (cmd) {
    handleCommand(cmd);
  }

  // ── Servo tick ──
  unsigned long now = millis();
  if (now - lastServoTick >= SERVO_INTERVAL) {
    lastServoTick = now;
    moveServos();
  }
}

// ═══════════════════════════════════════════════════════════
void handleCommand(char cmd) {
  // ═══ NEW IN THIS STEP ═══
  // ── Tank commands ──
  switch (cmd) {
    case 'M': driveForward();  return;
    case 'N': driveBackward(); return;
    case 'K': driveLeft();     return;
    case 'L': driveRight();    return;
    case 'O': stopMotors();    return;
  }
  // ═════════════════════════

  // ── Arm commands (same as step 5) ──
  // Wrist
  if (cmd == 'A')      wristDir =  1;
  else if (cmd == 'B') wristDir = -1;
  else if (cmd == 'a') wristDir =  0;

  // Elbow
  else if (cmd == 'C') elbowDir =  1;
  else if (cmd == 'D') elbowDir = -1;
  else if (cmd == 'c') elbowDir =  0;

  // Arm
  else if (cmd == 'G') armDir =  1;
  else if (cmd == 'H') armDir = -1;
  else if (cmd == 'g') armDir =  0;

  // Grip
  else if (cmd == 'I') gripDir =  1;
  else if (cmd == 'J') gripDir = -1;
  else if (cmd == 'i') gripDir =  0;

  // Step size
  else if (cmd == '1') stepDeg = 1;
  else if (cmd == '2') stepDeg = 2;
  else if (cmd == '3') stepDeg = 3;
  else if (cmd == '4') stepDeg = 4;
  else if (cmd == '5') stepDeg = 5;
}

// ── Servo stepping (explicit per joint) ────────────────
void moveServos() {
  // Wrist
  if (wristDir != 0) {
    int next = wristAngle + (wristDir * stepDeg);
    if (next < WRIST_MIN) next = WRIST_MIN;
    if (next > WRIST_MAX) next = WRIST_MAX;
    if (next != wristAngle) {
      wristAngle = next;
      wristServo.write(wristAngle);
    }
  }

  // Elbow
  if (elbowDir != 0) {
    int next = elbowAngle + (elbowDir * stepDeg);
    if (next < ELBOW_MIN) next = ELBOW_MIN;
    if (next > ELBOW_MAX) next = ELBOW_MAX;
    if (next != elbowAngle) {
      elbowAngle = next;
      elbowServo.write(elbowAngle);
    }
  }

  // Arm
  if (armDir != 0) {
    int next = armAngle + (armDir * stepDeg);
    if (next < ARM_MIN) next = ARM_MIN;
    if (next > ARM_MAX) next = ARM_MAX;
    if (next != armAngle) {
      armAngle = next;
      armServo.write(armAngle);
    }
  }

  // Grip
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

// ── Tank drive functions ───────────────────────────────
void driveForward() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  analogWrite(EN_A, SPEED_L);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  analogWrite(EN_B, SPEED_R);
}

void driveBackward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  analogWrite(EN_A, SPEED_L);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(EN_B, SPEED_R);
}

void driveLeft() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  analogWrite(EN_A, SPEED_L);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(EN_B, SPEED_R);
}

void driveRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  analogWrite(EN_A, SPEED_L);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  analogWrite(EN_B, SPEED_R);
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  analogWrite(EN_A, 0);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(EN_B, 0);
}
