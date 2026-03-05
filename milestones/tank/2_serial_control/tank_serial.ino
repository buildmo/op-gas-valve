// ================================================================
// TANK SERIAL CONTROL — Day 2 HR3 (Hard Deadline)
// ================================================================
// Purpose: Tank motor control ready for GUI integration
// Team:    Tank
//
// What changed from Day 1:
//   - Serial buffer drain (reads latest command only, ignores stale)
//   - No Serial.println feedback (cleaner for GUI integration)
//
// Commands: M=forward  N=backward  K=left  L=right  O=stop
// ================================================================

// ── Motor speeds ────────────────────────────────────────
#define SPEED_L 200
#define SPEED_R 220

// ── Motor pins ──────────────────────────────────────────
const int EN_A = 6;
const int IN1  = 4;
const int IN2  = 3;
const int IN3  = 7;
const int IN4  = 8;
const int EN_B = 5;

void setup() {
  Serial.begin(9600);

  pinMode(EN_A, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EN_B, OUTPUT);

  stopMotors();
  delay(500);
}

void loop() {
  // Drain buffer — only act on the latest byte
  char cmd = 0;
  while (Serial.available() > 0) {
    cmd = Serial.read();
  }

  if (cmd) {
    switch (cmd) {
      case 'M': driveForward();  break;
      case 'N': driveBackward(); break;
      case 'K': driveLeft();     break;
      case 'L': driveRight();    break;
      case 'O': stopMotors();    break;
    }
  }
}

// ── Drive functions ──────────────────────────────────────

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
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  analogWrite(EN_A, SPEED_L);
  digitalWrite(IN3, LOW);  digitalWrite(IN4, HIGH);
  analogWrite(EN_B, SPEED_R);
}

void driveRight() {
  digitalWrite(IN1, LOW);  digitalWrite(IN2, HIGH);
  analogWrite(EN_A, SPEED_L);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
  analogWrite(EN_B, SPEED_R);
}

void stopMotors() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, LOW);
  analogWrite(EN_A, 0);
  digitalWrite(IN3, LOW); digitalWrite(IN4, LOW);
  analogWrite(EN_B, 0);
}
