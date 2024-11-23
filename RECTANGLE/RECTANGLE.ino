#include <AccelStepper.h>
#include <Servo.h>



// Define stepper motor connections and interface type
#define STEP_PIN_X 10   // Pin for stepper motor X (X-axis)
#define DIR_PIN_X 9    // Pin for direction of stepper motor X

#define STEP_PIN_Y 3   // Pin for stepper motor Y (Y-axis)
#define DIR_PIN_Y 2    // Pin for direction of stepper motor Y
#define SERVO_PIN 4 // Pin connected to the servo control
// Create instances of the AccelStepper class
AccelStepper stepperX(AccelStepper::DRIVER, STEP_PIN_X, DIR_PIN_X);
AccelStepper stepperY(AccelStepper::DRIVER, STEP_PIN_Y, DIR_PIN_Y);
Servo penServo;

// Define positions for pen up and down
const int penUpPosition = 90;    // Adjust this value for the pen up position
const int penDownPosition = 180;  // Adjust this value for the pen down positio

// Define rectangle dimensions in steps
const long x_steps = 100; // Length of the rectangle along X-axis
const long y_steps = 100;  // Width of the rectangle along Y-axis

void setup() {
  // Set maximum speed and acceleration
  stepperX.setMaxSpeed(500);    
  stepperX.setAcceleration(250); 

  stepperY.setMaxSpeed(500);   
  stepperY.setAcceleration(250); 
  penServo.attach(SERVO_PIN);

  // Start with the pen up
  penServo.write(penDownPosition);
  delay(1000); // wait tit check that pen is lifted

  // Draw rectangle
  drawRectangle();
  
}

void loop() {
  // Nothing to do here
}

void drawRectangle() {
  penServo.write(penDownPosition);
  delay(2000); 

  stepperX.moveTo(x_steps);
  while (stepperX.distanceToGo() != 0) {
    stepperX.run();
  }

  // Move along Y-axis (forward)
  stepperY.moveTo(y_steps);
  while (stepperY.distanceToGo() != 0) {
    stepperY.run();
  }

  // Move back along X-axis (backward)
  stepperX.moveTo(0);
  while (stepperX.distanceToGo() != 0) {
    stepperX.run();
  }

  // Move back along Y-axis (backward)
  stepperY.moveTo(0);
  while (stepperY.distanceToGo() != 0) {
    stepperY.run();
  }
  penServo.write(penUpPosition);
  delay(2000); 
}




