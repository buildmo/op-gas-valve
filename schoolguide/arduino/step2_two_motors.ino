// ═══════════════════════════════════════════════════════════
// STEP 2 — Two Motors (Tank Drive)
//
// WHAT THIS DOES:
//   Controls BOTH motors so the robot can drive forward,
//   backward, spin left, spin right, or stop.
//   This is called "tank drive" because each side has its
//   own motor, just like a tank's treads.
//
// WHAT YOU LEARN:
//   - Adding a second motor (same wiring pattern)
//   - Creating named functions to keep code organized
//   - How turning works: spin motors in opposite directions
//
// HOW TO TEST:
//   1. Upload to Arduino
//   2. Open Serial Monitor (9600 baud)
//   3. Send M → both motors forward
//   4. Send K → robot spins left
//   5. Send O → everything stops
//
// COMMANDS:
//   M = forward    N = backward
//   K = left       L = right
//   O = stop
// ═══════════════════════════════════════════════════════════


// ── MOTOR SPEEDS ─────────────────────────────────────────
// Left and right motors often need slightly different speeds
// to drive straight (motors are never perfectly identical).
#define SPEED_L 200    // left motor speed  (0–255)
#define SPEED_R 220    // right motor speed (0–255)


// ── LEFT MOTOR PINS ──────────────────────────────────────
// Same three wires as Step 1 — speed + two direction pins.
const int EN_A = 6;    // left motor speed (PWM)
const int IN1  = 4;    // left motor direction wire 1
const int IN2  = 3;    // left motor direction wire 2

// ═══ NEW IN THIS STEP ═══════════════════════════════════
// ── RIGHT MOTOR PINS ─────────────────────────────────────
// Second motor uses a different set of pins on the driver board.
const int IN3  = 7;    // right motor direction wire 1
const int IN4  = 8;    // right motor direction wire 2
const int EN_B = 5;    // right motor speed (PWM)
// ═════════════════════════════════════════════════════════


// ══════════════════════════════════════════════════════════
// SETUP
// ══════════════════════════════════════════════════════════
void setup() {
  Serial.begin(9600);

  // Set all motor pins as outputs.
  // Left motor:
  pinMode(EN_A, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // ═══ NEW IN THIS STEP ═════════════════════════════════
  // Right motor:
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EN_B, OUTPUT);
  // ═══════════════════════════════════════════════════════

  // Start with everything stopped.
  stopMotors();

  Serial.println("Step 2 ready — send M / N / K / L / O");
}


// ══════════════════════════════════════════════════════════
// LOOP
// ══════════════════════════════════════════════════════════
void loop() {

  if (Serial.available() > 0) {
    char cmd = Serial.read();

    // Each letter calls a named function.
    // This keeps the code clean and easy to read.
    switch (cmd) {
      case 'M':
        Serial.println("Forward");
        driveForward();
        break;

      case 'N':
        Serial.println("Backward");
        driveBackward();
        break;

      case 'K':
        Serial.println("Left");
        driveLeft();
        break;

      case 'L':
        Serial.println("Right");
        driveRight();
        break;

      case 'O':
        Serial.println("Stop");
        stopMotors();
        break;
    }
  }
}


// ═══ NEW IN THIS STEP ═══════════════════════════════════
// ── DRIVE FUNCTIONS ──────────────────────────────────────
// Each function sets both motors at once.
// "Named functions" are like giving a recipe a name —
// instead of repeating all the steps, you just say the name.

// FORWARD: both motors spin the same way
void driveForward() {
  // Left motor → forward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(EN_A, SPEED_L);
  // Right motor → forward
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(EN_B, SPEED_R);
}

// BACKWARD: both motors spin the other way
void driveBackward() {
  // Left motor → backward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(EN_A, SPEED_L);
  // Right motor → backward
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(EN_B, SPEED_R);
}

// LEFT: left motor forward, right motor backward
// This makes the robot spin in place like a tank.
void driveLeft() {
  // Left motor → forward
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(EN_A, SPEED_L);
  // Right motor → backward (opposite!)
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(EN_B, SPEED_R);
}

// RIGHT: left motor backward, right motor forward
void driveRight() {
  // Left motor → backward
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(EN_A, SPEED_L);
  // Right motor → forward (opposite!)
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(EN_B, SPEED_R);
}

// STOP: turn off everything
void stopMotors() {
  // Left motor → off
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(EN_A, 0);
  // Right motor → off
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(EN_B, 0);
}
// ═════════════════════════════════════════════════════════
