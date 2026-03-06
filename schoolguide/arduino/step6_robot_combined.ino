// ═══════════════════════════════════════════════════════════
// STEP 6 — Full Robot (Tank Drive + Arm Combined)
//
// WHAT THIS DOES:
//   This is the FINAL sketch. It merges the tank drive from
//   Step 2 with the arm commands from Step 5 into one program.
//   The robot can now drive AND move its arm at the same time.
//
// WHAT YOU LEARN:
//   - Combining two systems into one program
//   - Serial buffer drain (reading only the latest command)
//   - Removing debug prints for production use
//
// HOW TO TEST:
//   Pair this with gui/step5_full_gui.py for full control!
//   Or use Serial Monitor with any command from Step 2 or Step 5:
//     Tank:  M / N / K / L / O
//     Arm:   A/B/a  C/D/c  G/H/g  I/J/i
//     Speed: 1 / 2 / 3 / 4 / 5
// ═══════════════════════════════════════════════════════════

#include <Servo.h>


// ── MOTOR SPEEDS ─────────────────────────────────────────
#define SPEED_L 200    // left motor  (0–255)
#define SPEED_R 220    // right motor (0–255)


// ── MOTOR PINS (from Step 2) ─────────────────────────────
const int EN_A = 6;    // left motor speed (PWM)
const int IN1  = 4;    // left motor direction wire 1
const int IN2  = 3;    // left motor direction wire 2
const int IN3  = 7;    // right motor direction wire 1
const int IN4  = 8;    // right motor direction wire 2
const int EN_B = 5;    // right motor speed (PWM)


// ── SERVO SETTINGS (from Step 5) ─────────────────────────
const int WRIST_PIN   = 10;
const int WRIST_MIN   = 45;
const int WRIST_MAX   = 135;
const int WRIST_START = 90;

const int ELBOW_PIN   = 12;
const int ELBOW_MIN   = 0;
const int ELBOW_MAX   = 90;
const int ELBOW_START = 45;

const int ARM_PIN   = 9;
const int ARM_MIN   = 70;
const int ARM_MAX   = 135;
const int ARM_START = 90;

const int GRIP_PIN   = 11;
const int GRIP_MIN   = 0;
const int GRIP_MAX   = 180;
const int GRIP_START = 90;


// ── SERVO OBJECTS ────────────────────────────────────────
Servo wristServo;
Servo elbowServo;
Servo armServo;
Servo gripServo;

// ── ANGLE TRACKING ───────────────────────────────────────
int wristAngle = WRIST_START;
int elbowAngle = ELBOW_START;
int armAngle   = ARM_START;
int gripAngle  = GRIP_START;

// ── DIRECTION VARIABLES ──────────────────────────────────
int wristDir = 0;    // -1 = down, 0 = idle, +1 = up
int elbowDir = 0;
int armDir   = 0;
int gripDir  = 0;

// ── STEP SIZE AND TIMING ─────────────────────────────────
int stepDeg = 2;                          // degrees per tick
const unsigned long SERVO_INTERVAL = 20;  // ms between ticks
unsigned long lastServoTick = 0;


// ══════════════════════════════════════════════════════════
// SETUP
// ══════════════════════════════════════════════════════════
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

  // Start with motors stopped.
  stopMotors();

  // Give servos time to reach start positions.
  delay(500);
}


// ══════════════════════════════════════════════════════════
// LOOP
// ══════════════════════════════════════════════════════════
void loop() {

  // ═══ NEW IN THIS STEP ═════════════════════════════════
  // ── SERIAL BUFFER DRAIN ────────────────────────────────
  // When multiple commands arrive at once (the GUI sends
  // them fast), we only care about the LATEST one.
  //
  // This "while" loop reads through ALL waiting bytes and
  // keeps only the last one. It's like fast-forwarding a
  // voicemail to hear just the newest message.
  char cmd = 0;
  while (Serial.available() > 0) {
    cmd = Serial.read();
  }
  // ═══════════════════════════════════════════════════════

  // If we got a command, handle it.
  if (cmd) {
    handleCommand(cmd);
  }

  // ── Servo tick (smooth stepping) ──
  unsigned long now = millis();
  if (now - lastServoTick >= SERVO_INTERVAL) {
    lastServoTick = now;
    moveServos();
  }
}


// ══════════════════════════════════════════════════════════
// HANDLE COMMAND — routes each letter to the right action
// ══════════════════════════════════════════════════════════
void handleCommand(char cmd) {

  switch (cmd) {

    // ═══ NEW IN THIS STEP ═══════════════════════════════
    // ── TANK DRIVE COMMANDS (merged from Step 2) ──
    case 'M': driveForward();  return;  // "return" exits immediately
    case 'N': driveBackward(); return;
    case 'K': driveLeft();     return;
    case 'L': driveRight();    return;
    case 'O': stopMotors();    return;
    // ═══════════════════════════════════════════════════

    // ── WRIST ──
    case 'A': wristDir =  1; break;  // up
    case 'B': wristDir = -1; break;  // down
    case 'a': wristDir =  0; break;  // stop

    // ── ELBOW ──
    case 'C': elbowDir =  1; break;  // up
    case 'D': elbowDir = -1; break;  // down
    case 'c': elbowDir =  0; break;  // stop

    // ── ARM ──
    case 'G': armDir =  1; break;    // up
    case 'H': armDir = -1; break;    // down
    case 'g': armDir =  0; break;    // stop

    // ── GRIP ──
    case 'I': gripDir =  1; break;   // open
    case 'J': gripDir = -1; break;   // close
    case 'i': gripDir =  0; break;   // stop

    // ── STEP SIZE ──
    case '1': stepDeg = 1; break;
    case '2': stepDeg = 2; break;
    case '3': stepDeg = 3; break;
    case '4': stepDeg = 4; break;
    case '5': stepDeg = 5; break;
  }
}


// ══════════════════════════════════════════════════════════
// MOVE SERVOS — called every 20ms for smooth motion
// ══════════════════════════════════════════════════════════
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


// ══════════════════════════════════════════════════════════
// TANK DRIVE FUNCTIONS (from Step 2)
// ══════════════════════════════════════════════════════════

// FORWARD: both motors spin the same way
void driveForward() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  analogWrite(EN_A, SPEED_L);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  analogWrite(EN_B, SPEED_R);
}

// BACKWARD: both motors reversed
void driveBackward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  analogWrite(EN_A, SPEED_L);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(EN_B, SPEED_R);
}

// LEFT: left forward + right backward = spin left
void driveLeft() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  analogWrite(EN_A, SPEED_L);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(EN_B, SPEED_R);
}

// RIGHT: left backward + right forward = spin right
void driveRight() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  analogWrite(EN_A, SPEED_L);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  analogWrite(EN_B, SPEED_R);
}

// STOP: all pins off, speed to zero
void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  analogWrite(EN_A, 0);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(EN_B, 0);
}
