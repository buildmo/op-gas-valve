#include <Servo.h>

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

// ── Servo config ────────────────────────────────────────
// Index: 0=Wrist, 1=Elbow, 2=Arm, 3=Grip
const int NUM_SERVOS    = 4;
const int servoPins[4]  = {10, 12, 9, 11};
const int servoMin[4]   = {45,  0, 70,  0};
const int servoMax[4]   = {135, 90, 135, 180};
const int servoStart[4] = {90, 45, 90, 90};
const int STEP_DEG      = 2;
const unsigned long SERVO_INTERVAL = 20; // ms between servo steps

Servo servos[NUM_SERVOS];
int   servoAngle[NUM_SERVOS];
int   servoDir[NUM_SERVOS]; // -1 = down, 0 = idle, +1 = up

unsigned long lastServoTick = 0;

// ══════════════════════════════════════════════════════════
void setup() {
  Serial.begin(9600);

  pinMode(EN_A, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EN_B, OUTPUT);

  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].attach(servoPins[i]);
    servoAngle[i] = servoStart[i];
    servos[i].write(servoAngle[i]);
    servoDir[i] = 0;
  }

  stopMotors();
  delay(500);
}

// ══════════════════════════════════════════════════════════
void loop() {
  // ── Read serial: only act on the latest byte ──────────
  char cmd = 0;
  while (Serial.available() > 0) {
    cmd = Serial.read();
  }

  if (cmd) {
    handleCommand(cmd);
  }

  // ── Servo tick: step active servos every SERVO_INTERVAL ms ──
  unsigned long now = millis();
  if (now - lastServoTick >= SERVO_INTERVAL) {
    lastServoTick = now;
    tickServos();
  }
}

// ══════════════════════════════════════════════════════════
void handleCommand(char cmd) {
  switch (cmd) {
    // Tank
    case 'M': driveForward();  break;
    case 'N': driveBackward(); break;
    case 'K': driveLeft();     break;
    case 'L': driveRight();    break;
    case 'O': stopMotors();    break;

    // Servo start: uppercase = set direction
    case 'A': servoDir[0] =  1; break; // Wrist up
    case 'B': servoDir[0] = -1; break; // Wrist down
    case 'C': servoDir[1] =  1; break; // Elbow up
    case 'D': servoDir[1] = -1; break; // Elbow down
    case 'G': servoDir[2] =  1; break; // Arm up
    case 'H': servoDir[2] = -1; break; // Arm down
    case 'I': servoDir[3] =  1; break; // Grip open
    case 'J': servoDir[3] = -1; break; // Grip close

    // Servo stop: lowercase = set idle
    case 'a': servoDir[0] = 0; break;
    case 'c': servoDir[1] = 0; break;
    case 'g': servoDir[2] = 0; break;
    case 'i': servoDir[3] = 0; break;
  }
}

// ── Servo tick ──────────────────────────────────────────
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

// ── Tank drive functions ────────────────────────────────
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
