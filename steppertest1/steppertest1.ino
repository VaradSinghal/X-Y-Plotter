#include <AccelStepper.h>

#define motorInterfaceType 1
#define stepPin1 10
#define dirPin1 9
#define stepPin2 3
#define dirPin2 2


AccelStepper stepper1(motorInterfaceType, stepPin1, dirPin1);
AccelStepper stepper2(motorInterfaceType, stepPin2, dirPin2);

void setup() {
  
  stepper1.setMaxSpeed(100); 
  stepper1.setSpeed(100);
  stepper2.setMaxSpeed(100); 
  stepper2.setSpeed(100);          
}

void loop() {
  stepper1.runSpeed();
  stepper2.runSpeed();
}
