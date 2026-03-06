// ═══════════════════════════════════════════════════════════
// STEP 1 — One Motor
//
// WHAT THIS DOES:
//   You type a letter into the Serial Monitor and the motor
//   spins forward, backward, or stops.
//
// WHAT YOU LEARN:
//   - How to set up pins on the Arduino
//   - How to read a command from the computer
//   - How to make a motor spin
//
// HOW TO TEST:
//   1. Upload this sketch to the Arduino
//   2. Open Serial Monitor (magnifying glass icon, top right)
//   3. Make sure it says "9600 baud" at the bottom
//   4. Type F and press Send → motor spins forward
//   5. Type B and press Send → motor spins backward
//   6. Type S and press Send → motor stops
// ═══════════════════════════════════════════════════════════


// ── MOTOR SPEED ──────────────────────────────────────────
// This number controls how fast the motor spins.
// It goes from 0 (stopped) to 255 (full speed).
// 200 is about 80% power — a good default.
#define SPEED_L 200


// ── PIN ASSIGNMENTS ──────────────────────────────────────
// These three wires connect the Arduino to the motor driver.
// EN_A = "enable" pin — controls the speed (must be a PWM pin)
// IN1 and IN2 = "direction" pins — control which way the motor spins
//
// Think of it like a water tap:
//   EN_A = how far you open the tap (speed)
//   IN1/IN2 = which pipe the water flows through (direction)
const int EN_A = 6;    // speed control (PWM)
const int IN1  = 4;    // direction wire 1
const int IN2  = 3;    // direction wire 2


// ══════════════════════════════════════════════════════════
// SETUP — runs once when the Arduino powers on
// ══════════════════════════════════════════════════════════
void setup() {

  // Start the serial connection so we can talk to the computer.
  // 9600 is the communication speed — must match the Serial Monitor.
  Serial.begin(9600);

  // Tell the Arduino that these pins are OUTPUTS (sending signals out).
  // This is required before you can turn them on or off.
  pinMode(EN_A, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Start with the motor completely stopped.
  // LOW means "off" — no signal on the direction wires.
  // analogWrite(pin, 0) means speed = zero.
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(EN_A, 0);

  // Print a welcome message so we know the Arduino is ready.
  Serial.println("Step 1 ready — send F / B / S");
}


// ══════════════════════════════════════════════════════════
// LOOP — runs over and over, forever, after setup finishes
// ══════════════════════════════════════════════════════════
void loop() {

  // Check if the computer has sent us a letter.
  // Serial.available() returns how many letters are waiting.
  if (Serial.available() > 0) {

    // Read one letter from the serial buffer.
    // This removes it from the queue, like taking a ticket.
    char cmd = Serial.read();

    // Decide what to do based on which letter we received.
    // "switch" is like a menu — jump to the matching "case".
    switch (cmd) {

      // ── FORWARD ──
      // To spin the motor forward:
      //   IN1 = LOW, IN2 = HIGH  (direction = forward)
      //   EN_A = SPEED_L         (turn on the speed)
      case 'F':
        Serial.println("Forward");
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        analogWrite(EN_A, SPEED_L);
        break;  // "break" means "stop here, don't fall through"

      // ── BACKWARD ──
      // Same idea, but swap IN1 and IN2 to reverse direction.
      case 'B':
        Serial.println("Backward");
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        analogWrite(EN_A, SPEED_L);
        break;

      // ── STOP ──
      // Set both direction wires LOW and speed to zero.
      case 'S':
        Serial.println("Stop");
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        analogWrite(EN_A, 0);
        break;
    }
  }
}
