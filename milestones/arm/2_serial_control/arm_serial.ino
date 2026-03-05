// ================================================================
// ARM SERIAL CONTROL — Day 2 HR3 (Hard Deadline)
// ================================================================
// Purpose: Servo control with limits and smooth stepping for GUI
//          integration
// Team:    Arm
//
// What changed from Day 1:
//   - Serial commands instead of manual +/- testing
//   - Angle limits per joint (prevents mechanical damage)
//   - Smooth stepping (2 deg every 20ms) instead of instant jumps
//
// Commands:
//   A/B = Wrist up/down       a = Wrist stop
//   C/D = Elbow up/down       c = Elbow stop
//   G/H = Arm up/down         g = Arm stop
//   I/J = Grip open/close     i = Grip stop
// ================================================================

#include <Servo.h>

// ── Servo config ────────────────────────────────────────
// Index: 0=Wrist, 1=Elbow, 2=Arm, 3=Grip
const int NUM_SERVOS    = 4;
const int servoPins[4]  = {10, 12, 9, 11};
const int servoMin[4]   = {45,  0, 70,  0};
const int servoMax[4]   = {135, 90, 135, 180};
const int servoStart[4] = {90, 45, 90, 90};
const int STEP_DEG      = 2;
const unsigned long SERVO_INTERVAL = 20; // ms between steps

Servo servos[NUM_SERVOS];
int   servoAngle[NUM_SERVOS];
int   servoDir[NUM_SERVOS]; // -1 = down, 0 = idle, +1 = up

unsigned long lastServoTick = 0;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].attach(servoPins[i]);
    servoAngle[i] = servoStart[i];
    servos[i].write(servoAngle[i]);
    servoDir[i] = 0;
  }

  delay(500);
}

void loop() {
  // Drain buffer — only act on the latest byte
  char cmd = 0;
  while (Serial.available() > 0) {
    cmd = Serial.read();
  }

  if (cmd) {
    handleCommand(cmd);
  }

  // Servo tick: step active servos every SERVO_INTERVAL ms
  unsigned long now = millis();
  if (now - lastServoTick >= SERVO_INTERVAL) {
    lastServoTick = now;
    tickServos();
  }
}

void handleCommand(char cmd) {
  switch (cmd) {
    // Start moving: uppercase sets direction
    case 'A': servoDir[0] =  1; break; // Wrist up
    case 'B': servoDir[0] = -1; break; // Wrist down
    case 'C': servoDir[1] =  1; break; // Elbow up
    case 'D': servoDir[1] = -1; break; // Elbow down
    case 'G': servoDir[2] =  1; break; // Arm up
    case 'H': servoDir[2] = -1; break; // Arm down
    case 'I': servoDir[3] =  1; break; // Grip open
    case 'J': servoDir[3] = -1; break; // Grip close

    // Stop moving: lowercase sets idle
    case 'a': servoDir[0] = 0; break;
    case 'c': servoDir[1] = 0; break;
    case 'g': servoDir[2] = 0; break;
    case 'i': servoDir[3] = 0; break;
  }
}

void tickServos() {
  for (int i = 0; i < NUM_SERVOS; i++) {
    if (servoDir[i] == 0) continue;

    int next = servoAngle[i] + (servoDir[i] * STEP_DEG);
    if (next < servoMin[i]) next = servoMin[i];
    if (next > servoMax[i]) next = servoMax[i];

    if (next != servoAngle[i]) {
      servoAngle[i] = next;
      servos[i].write(servoAngle[i]);
    }
  }
}
