// ================================================================
// ONE MOTOR TEST — Base Code
// ================================================================
// Purpose: Get a single DC motor spinning via the H-bridge
// Team:    Tank
// Test:    Open Serial Monitor (9600 baud), send single characters
//
// Commands:
//   F = Forward (motor spins one way)
//   B = Backward (motor spins the other way)
//   S = Stop
//
// This only controls the LEFT motor. Once this works, you know
// your H-bridge wiring is correct for one side. Then add the
// right motor.
// ================================================================

#define SPEED 200  // Motor speed (0-255)

// Left motor pins — match these to YOUR wiring
const int EN_A = 6;   // Enable pin (PWM speed control)
const int IN1  = 4;   // Direction pin 1
const int IN2  = 3;   // Direction pin 2

void setup() {
  Serial.begin(9600);

  pinMode(EN_A, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Start with motor off
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(EN_A, 0);

  delay(500);

  Serial.println("=== One Motor Test Ready ===");
  Serial.println("Commands: F=forward  B=backward  S=stop");
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    switch (cmd) {
      case 'F':
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        analogWrite(EN_A, SPEED);
        Serial.println(">> Forward");
        break;

      case 'B':
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(EN_A, SPEED);
        Serial.println(">> Backward");
        break;

      case 'S':
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        analogWrite(EN_A, 0);
        Serial.println(">> Stop");
        break;
    }
  }
}
