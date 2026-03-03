#include <Servo.h>
#define speedL 200
#define speedR 220
#define pause 100
char c;
// ── Tank ────────────────────────────────────────────── 
int EN_A = 6;  //controls the speed of motor A
int IN1 = 4;   //motor A direction
int IN2 = 3;   //motor A direction
int IN3 = 7;   //motor B direction
int IN4 = 8;   //motor B direction
int EN_B = 5;  //controls the speed of motor B

// ── WRIST ──────────────────────────────────────────────
Servo wrist;
const int wristPin = 10;
const int minAngle_wrist = 45;
const int maxAngle_wrist = 135;
const int stepDegrees = 5;
int currentAngle_wrist = 90;

// ── ELBOW ──────────────────────────────────────────────
Servo elbow;
const int elbowPin = 12;
const int minAngle_elbow = 0;
const int maxAngle_elbow = 90;
int currentAngle_elbow = 45;

// ── HAND ───────────────────────────────────────────────
Servo hand;
const int handPin = 0;
const int minAngle_hand = 90;
const int maxAngle_hand = 135;
int currentAngle_hand = 90;

// ── ARM ────────────────────────────────────────────────
Servo arm;
const int armPin = 9;
const int minAngle_arm = 70;
const int maxAngle_arm = 135;
int currentAngle_arm = 90;

// ── FINGERS ────────────────────────────────────────────
Servo fingers;
const int fingersPin = 11;
const int minAngle_fingers = 0;
const int maxAngle_fingers = 180;
int currentAngle_fingers =90;


// ══════════════════════════════════════════════════════
void setup() {
  Serial.begin(9600);
  //Initializing the motor pins as outputs
  pinMode(EN_A, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(EN_B, OUTPUT);

  wrist.attach(wristPin);
  wrist.write(currentAngle_wrist);

  elbow.attach(elbowPin);
  elbow.write(currentAngle_elbow);

  hand.attach(handPin);
  hand.write(currentAngle_hand);

  arm.attach(armPin);
  arm.write(currentAngle_arm);

  fingers.attach(fingersPin);
  fingers.write(currentAngle_fingers);

  delay(500);
}

void loop() {
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
      c = Serial.read();
    }
    Serial.println(c);
    switch (c) {
      // WRIST
      case 'A':
        wristUp();
        break;
      case 'B':
        wristDown();
        break;
      // ELBOW
      case 'C':
        elbowUp();
        break;
      case 'D':
        elbowDown();
        break;
      // HAND
      case 'E':
        handUp();
        break;
      case 'F':
        handDown();
        break;
      // ARM
      case 'G':
        armUp();
        break;
      case 'H':
        armDown();
        break;
      // FINGERS
      case 'I':
        fingersUp();
        break;
      case 'J':
        fingersDown();
        break;
      case 'K':
        turnLeft();
        break;
      case 'L':
        turnRight();
        break;
      case 'M':
        forward();
        break;
      case 'N':
        backward();
        break;
      case 'O':
        stop();
        break;
    }
  }
}


// ── WRIST FUNCTIONS ────────────────────────────────────
void wristUp() {
  if (currentAngle_wrist > maxAngle_wrist) {
    Serial.println("Already at limit, cannot move further.");
    return;
  } else {
    wrist.write(currentAngle_wrist + stepDegrees);
    delay(15);
    currentAngle_wrist += stepDegrees;
    Serial.print("Moved to: ");
    Serial.print(currentAngle_wrist);
    Serial.println("°");
  }
}
void wristDown() {
  if (currentAngle_wrist < minAngle_wrist) {
    Serial.println("Already at limit, cannot move further.");
    return;
  } else {
    wrist.write(currentAngle_wrist - stepDegrees);
    delay(15);
    currentAngle_wrist -= stepDegrees;
    Serial.print("Moved to: ");
    Serial.print(currentAngle_wrist);
    Serial.println("°");
  }
}

// ── ELBOW FUNCTIONS ────────────────────────────────────
void elbowUp() {
  if (currentAngle_elbow > maxAngle_elbow) {
    Serial.println("Already at limit, cannot move further.");
    return;
  } else {
    elbow.write(currentAngle_elbow + stepDegrees);
    delay(15);
    currentAngle_elbow += stepDegrees;
    Serial.print("Moved to: ");
    Serial.print(currentAngle_elbow);
    Serial.println("°");
  }
}
void elbowDown() {
  if (currentAngle_elbow < minAngle_elbow) {
    Serial.println("Already at limit, cannot move further.");
    return;
  } else {
    elbow.write(currentAngle_elbow - stepDegrees);
    delay(15);
    currentAngle_elbow -= stepDegrees;
    Serial.print("Moved to: ");
    Serial.print(currentAngle_elbow);
    Serial.println("°");
  }
}

// ── HAND FUNCTIONS ─────────────────────────────────────
void handUp() {
  if (currentAngle_hand > maxAngle_hand) {
    Serial.println("Already at limit, cannot move further.");
    return;
  } else {
    hand.write(currentAngle_hand + stepDegrees);
    delay(15);
    currentAngle_hand += stepDegrees;
    Serial.print("Moved to: ");
    Serial.print(currentAngle_hand);
    Serial.println("°");
  }
}
void handDown() {
  if (currentAngle_hand < minAngle_hand) {
    Serial.println("Already at limit, cannot move further.");
    return;
  } else {
    hand.write(currentAngle_hand - stepDegrees);
    delay(15);
    currentAngle_hand -= stepDegrees;
    Serial.print("Moved to: ");
    Serial.print(currentAngle_hand);
    Serial.println("°");
  }
}

// ── ARM FUNCTIONS ──────────────────────────────────────
void armUp() {
  if (currentAngle_arm > maxAngle_arm) {
    Serial.println("Already at limit, cannot move further.");
    return;
  } else {
    arm.write(currentAngle_arm + stepDegrees);
    delay(15);
    currentAngle_arm += stepDegrees;
    Serial.print("Moved to: ");
    Serial.print(currentAngle_arm);
    Serial.println("°");
  }
}
void armDown() {
  if (currentAngle_arm < minAngle_arm) {
    Serial.println("Already at limit, cannot move further.");
    return;
  } else {
    arm.write(currentAngle_arm - stepDegrees);
    delay(15);
    currentAngle_arm -= stepDegrees;
    Serial.print("Moved to: ");
    Serial.print(currentAngle_arm);
    Serial.println("°");
  }
}

// ── FINGERS FUNCTIONS ──────────────────────────────────
void fingersUp() {
  if (currentAngle_fingers > maxAngle_fingers) {
    Serial.println("Already at limit, cannot move further.");
    return;
  } else {
    fingers.write(currentAngle_fingers + stepDegrees);
    delay(15);
    currentAngle_fingers += stepDegrees;
    Serial.print("Moved to: ");
    Serial.print(currentAngle_fingers);
    Serial.println("°");
  }
}
void fingersDown() {
  if (currentAngle_fingers < minAngle_fingers) {
    Serial.println("Already at limit, cannot move further.");
    return;
  } else {
    fingers.write(currentAngle_fingers - stepDegrees);
    delay(15);
    currentAngle_fingers -= stepDegrees;
    Serial.print("Moved to: ");
    Serial.print(currentAngle_fingers);
    Serial.println("°");
  }
}
//Driving Functions
void turnLeft() {
  back_left();
  forward_right();
  delay(pause);
  stop();
  
}
void turnRight() {
  forward_left();
  back_right();
  delay(pause);
  stop();
}
void forward() {
  forward_left();
  forward_right();
  delay(pause);
  stop();
}
void backward() {
  back_left();
  back_right();
  delay(pause);
  stop();
}
void stop(){
  halt_left();
  halt_right();
  Serial.println("°");
}

void forward_left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(EN_A, speedL);
}
void back_left() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(EN_A, speedL);
}
void halt_left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(EN_A, 0);
}
void forward_right() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(EN_B, speedR);
}
void back_right() {
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(EN_B, speedR);
}
void halt_right() {
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(EN_B, 0);
}
