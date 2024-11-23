#include <AccelStepper.h>


#define STEP_PIN 10    
#define DIR_PIN 9   

// Create an instance of the AccelStepper class
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void setup() {
  // Set maximum speed and acceleration
  stepper.setMaxSpeed(100);     // Set the speed (steps per second)
  stepper.setAcceleration(100);  // Set acceleration

  // Move the stepper motor 10,000 steps
  stepper.moveTo(100);  // Set target position to 10,000 steps away
}

void loop() {
  // Run the stepper to the target position
  if (stepper.distanceToGo() != 0) {
    stepper.run();
  }
}

