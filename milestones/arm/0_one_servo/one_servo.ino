// ================================================================
// ONE SERVO TEST — Base Code
// ================================================================
// Purpose: Get a single servo moving
// Team:    Arm
// Test:    Open Serial Monitor (9600 baud), send single characters
//
// Commands:
//   + = Move servo up 5 degrees
//   - = Move servo down 5 degrees
//   0 = Centre servo to 90 degrees
//
// This only controls one servo on pin 10. Once this works, you
// know your servo wiring and power are correct. Then add more
// servos.
//
// IMPORTANT: Power the servo from the buck converter, NOT from
// the Arduino 5V pin.
// ================================================================

#include <Servo.h>

Servo myServo;
int angle = 90;  // Start at centre

void setup() {
  Serial.begin(9600);

  myServo.attach(10);  // Servo on pin 10
  myServo.write(angle);

  delay(500);

  Serial.println("=== One Servo Test Ready ===");
  Serial.println("Commands: +=up  -=down  0=centre");
  Serial.print("Angle: ");
  Serial.println(angle);
}

void loop() {
  if (Serial.available() > 0) {
    char cmd = Serial.read();

    switch (cmd) {
      case '+':
        angle = min(180, angle + 5);
        myServo.write(angle);
        break;

      case '-':
        angle = max(0, angle - 5);
        myServo.write(angle);
        break;

      case '0':
        angle = 90;
        myServo.write(angle);
        break;
    }

    Serial.print("Angle: ");
    Serial.println(angle);
  }
}
