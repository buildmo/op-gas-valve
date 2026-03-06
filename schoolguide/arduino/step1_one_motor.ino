// ═══════════════════════════════════════════════════════════
// STEP 1 — One Motor
// Learn: pin setup, Serial commands, driving one motor
// Test: open Serial Monitor, send F / B / S
// ═══════════════════════════════════════════════════════════

#define SPEED_L 200

const int EN_A = 6;   // PWM speed pin (left motor)
const int IN1  = 4;   // direction pin 1
const int IN2  = 3;   // direction pin 2

void setup() {
  Serial.begin(9600);

  pinMode(EN_A, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Start stopped
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(EN_A, 0);

  Serial.println("Step 1 ready — send F / B / S");
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    if (cmd == 'F') {
      Serial.println("Forward");
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      analogWrite(EN_A, SPEED_L);
    }
    else if (cmd == 'B') {
      Serial.println("Backward");
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      analogWrite(EN_A, SPEED_L);
    }
    else if (cmd == 'S') {
      Serial.println("Stop");
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      analogWrite(EN_A, 0);
    }
  }
}
