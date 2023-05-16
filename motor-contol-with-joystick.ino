#include <Servo.h>

// Define servo objects
Servo servo1;
Servo servo2;

// Joystick pins
const int joystickXPin = A0;  // Analog pin for X-axis
const int joystickYPin = A1;  // Analog pin for Y-axis

// Servo control pins
const int servo1Pin = 9;  // Control pin for servo 1
const int servo2Pin = 10; // Control pin for servo 2

// Ultrasonic sensor pins
const int TRIGGER_PIN = 2;
const int ECHO_PIN = 3;

// Servo limits
const int servoMin = 0;   // Minimum servo position
const int servoMax = 180; // Maximum servo position

// Joystick deadzone
const int joystickDeadzone = 50;

// Default position
const int defaultPosition = 90;

// Distance threshold to stop the servos
const int distanceThreshold = 5; // in centimeters

void setup() {
  // Attach servo objects to the control pins
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  // Set default position
  servo1.write(defaultPosition);
  servo2.write(defaultPosition);

  // Initialize ultrasonic sensor pins
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Initialize serial communication
  Serial.begin(9600);
}

void loop() {
  // Read joystick values
  int joystickXValue = analogRead(joystickXPin);
  int joystickYValue = analogRead(joystickYPin);

  // Map joystick values to servo angles
  int servo1Angle = map(joystickXValue, 0, 1023, servoMin, servoMax);
  int servo2Angle = map(joystickYValue, 0, 1023, servoMin, servoMax);

  // Apply deadzone to the joystick
  if (abs(joystickXValue - 512) < joystickDeadzone) {
    servo1Angle = defaultPosition; // Center position
  }
  if (abs(joystickYValue - 512) < joystickDeadzone) {
    servo2Angle = defaultPosition; // Center position
  }

  // Check the distance from the ultrasonic sensor
  int distance = measureDistance();

  // Stop the servos if the distance is less than or equal to the threshold
  if (distance <= distanceThreshold) {
    servo1.detach();
    servo2.detach();
  } else {
    if (!servo1.attached()) {
      servo1.attach(servo1Pin);
    }
    if (!servo2.attached()) {
      servo2.attach(servo2Pin);
    }
    servo1.write(servo1Angle);
    servo2.write(servo2Angle);
  }
}

int measureDistance() {
  // Trigger the ultrasonic sensor by sending a 10µs pulse
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);

  // Measure the duration of the pulse received by the echo pin
  long duration = pulseIn(ECHO_PIN, HIGH);

  // Calculate the distance in centimeters
  int distance_cm = duration * 0.034 / 2;

  // Print the distance on the serial monitor
  Serial.print("Distance: ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  return distance_cm;
}

