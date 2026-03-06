// ═══════════════════════════════════════════════════════════
// STEP 2 — Two Motors (Tank Drive)
// Learn: controlling two motors, named drive functions
// Test: Serial Monitor — send M / N / K / L / O
// ═══════════════════════════════════════════════════════════

#define SPEED_L 200
#define SPEED_R 220

// ── Left motor pins ──
const int EN_A = 6;
const int IN1  = 4;
const int IN2  = 3;

// ═══ NEW IN THIS STEP ═══
// ── Right motor pins ──
const int IN3  = 7;
const int IN4  = 8;
const int EN_B = 5;
// ═════════════════════════

void setup() {
  Serial.begin(9600);

  pinMode(EN_A, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // ═══ NEW IN THIS STEP ═══
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EN_B, OUTPUT);
  // ═════════════════════════

  stopMotors();
  Serial.println("Step 2 ready — send M / N / K / L / O");
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    if (cmd == 'M') {
      Serial.println("Forward");
      driveForward();
    }
    else if (cmd == 'N') {
      Serial.println("Backward");
      driveBackward();
    }
    else if (cmd == 'K') {
      Serial.println("Left");
      driveLeft();
    }
    else if (cmd == 'L') {
      Serial.println("Right");
      driveRight();
    }
    else if (cmd == 'O') {
      Serial.println("Stop");
      stopMotors();
    }
  }
}

// ═══ NEW IN THIS STEP ═══
// ── Named drive functions ──────────────────────────────

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
// ═════════════════════════
