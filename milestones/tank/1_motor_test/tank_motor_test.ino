// ================================================================
// TANK MOTOR TEST — Day 1 HR5
// ================================================================
// Purpose: Verify H-bridge wiring and motor direction
// Team:    Tank
// Test:    Open Serial Monitor (9600 baud), send single characters
//
// Commands:
//   M = Forward    N = Backward
//   K = Left       L = Right
//   O = Stop
//
// If a motor spins the wrong way, swap its OUT1/OUT2 wires on
// the H-bridge — that's a wiring fix, not a code fix.
// ================================================================

// Motor speeds — tweak these if one side pulls harder
#define SPEED_L 200
#define SPEED_R 220

// Motor pins — match these to YOUR wiring
const int EN_A = 6;   // Left motor enable (PWM)
const int IN1  = 4;   // Left motor direction
const int IN2  = 3;   // Left motor direction
const int IN3  = 7;   // Right motor direction
const int IN4  = 8;   // Right motor direction
const int EN_B = 5;   // Right motor enable (PWM)

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

  Serial.println("=== Tank Motor Test Ready ===");
  Serial.println("Commands: M=fwd  N=back  K=left  L=right  O=stop");
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    switch (cmd) {
      case 'M': driveForward();  Serial.println(">> Forward");  break;
      case 'N': driveBackward(); Serial.println(">> Backward"); break;
      case 'K': driveLeft();     Serial.println(">> Left");     break;
      case 'L': driveRight();    Serial.println(">> Right");    break;
      case 'O': stopMotors();    Serial.println(">> Stop");     break;
    }
  }
}

// ── Drive functions ──────────────────────────────────────────

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
