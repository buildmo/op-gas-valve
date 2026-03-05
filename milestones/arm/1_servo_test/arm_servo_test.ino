// ================================================================
// ARM SERVO TEST — Day 1 HR5
// ================================================================
// Purpose: Verify each servo is wired, powered, and responds
// Team:    Arm
// Test:    Open Serial Monitor (9600 baud), send single characters
//
// Commands:
//   1-4  = Select servo (Wrist, Elbow, Arm, Grip)
//   +    = Move selected servo up 5 degrees
//   -    = Move selected servo down 5 degrees
//   0    = Centre all servos to 90 degrees
//
// IMPORTANT: Power servos from the buck converter, NOT from
// the Arduino 5V pin. The Arduino cannot supply enough current
// for even one servo under load.
// ================================================================

#include <Servo.h>

const int NUM_SERVOS = 4;
const int servoPins[4]    = {10, 12, 9, 11};
const char* servoNames[4] = {"Wrist", "Elbow", "Arm", "Grip"};

Servo servos[NUM_SERVOS];
int angles[NUM_SERVOS];
int selected = 0;

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i].attach(servoPins[i]);
    angles[i] = 90;
    servos[i].write(90);
  }

  delay(500);

  Serial.println("=== Arm Servo Test Ready ===");
  Serial.println("Commands: 1-4=select  +=up  -=down  0=centre all");
  Serial.println();
  printSelected();
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    switch (cmd) {
      case '1': selected = 0; printSelected(); break;
      case '2': selected = 1; printSelected(); break;
      case '3': selected = 2; printSelected(); break;
      case '4': selected = 3; printSelected(); break;

      case '+':
        angles[selected] = min(180, angles[selected] + 5);
        servos[selected].write(angles[selected]);
        printAngle();
        break;

      case '-':
        angles[selected] = max(0, angles[selected] - 5);
        servos[selected].write(angles[selected]);
        printAngle();
        break;

      case '0':
        for (int i = 0; i < NUM_SERVOS; i++) {
          angles[i] = 90;
          servos[i].write(90);
        }
        Serial.println(">> All servos centred to 90 deg");
        break;
    }
  }
}

void printSelected() {
  Serial.print("Selected: ");
  Serial.print(servoNames[selected]);
  Serial.print(" (pin ");
  Serial.print(servoPins[selected]);
  Serial.print(") at ");
  Serial.print(angles[selected]);
  Serial.println(" deg");
}

void printAngle() {
  Serial.print("  ");
  Serial.print(servoNames[selected]);
  Serial.print(": ");
  Serial.print(angles[selected]);
  Serial.println(" deg");
}
