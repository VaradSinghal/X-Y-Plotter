#include <AccelStepper.h>   
#include <Servo.h>    
#include <M5Stack.h>
#include "Module_GRBL_13.2.h"      


#define X_STEP_PIN 2
#define X_DIR_PIN 3
#define Y_STEP_PIN 4
#define Y_DIR_PIN 5


#define X_LIMIT_PIN 6        
#define Y_LIMIT_PIN 7        

#define SERVO_PIN 9

// Initialize the stepper motors (DRIVER mode for A4988 drivers)
AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);

// Initialize the servo motor for pen control
Servo penServo;

// Steps per millimeter for the X and Y axes (based on motor/driver configuration)
int stepsPerMM_X = 80;      // Adjust based on your stepper setup
int stepsPerMM_Y = 80;      // Adjust based on your stepper setup

// Pen positions for servo control
int penUpPosition = 90;     // Servo angle for pen up (adjust as needed)
int penDownPosition = 0;    // Servo angle for pen down (adjust as needed)

void setup() {
  // Serial communication for receiving G-code from UGS
  Serial.begin(115200);

  // Attach the servo motor for pen control
  penServo.attach(SERVO_PIN);
  penUp();  // Start with the pen in the up position

  // Configure stepper motor speed and acceleration
  stepperX.setMaxSpeed(1000);       // Set maximum speed for X-axis
  stepperX.setAcceleration(500);    // Set acceleration for X-axis
  stepperY.setMaxSpeed(1000);       // Set maximum speed for Y-axis
  stepperY.setAcceleration(500);    // Set acceleration for Y-axis

  // Set up the limit switches
  pinMode(X_LIMIT_PIN, INPUT_PULLUP);  // Configure the X-axis limit switch as an input
  pinMode(Y_LIMIT_PIN, INPUT_PULLUP);  // Configure the Y-axis limit switch as an input

  // Auto-homing (moving to limit switches)
  autoHome();  // Move to the home position (0,0) at start
}

void loop() {
  // Check if there's incoming G-code from UGS
  if (Serial.available() > 0) {
    String gcode = Serial.readStringUntil('\n');  // Read one line of G-code
    processGCode(gcode);  // Process the G-code command
  }
}

// Function to process the incoming G-code
void processGCode(String gcode) {
  gcode.trim();  // Clean up extra spaces and newlines

  // Handle G0 or G1 commands (movement)
  if (gcode.startsWith("G0") || gcode.startsWith("G1")) {
    float x = extractCoordinate(gcode, 'X');
    float y = extractCoordinate(gcode, 'Y');
    moveTo(x, y);  // Move to the specified X and Y coordinates
  }

  // Handle Z-axis commands (use for pen up/down via servo)
  if (gcode.indexOf('Z') != -1) {
    float z = extractCoordinate(gcode, 'Z');
    if (z > 0) {
      penUp();  // Z > 0 means pen up
    } else {
      penDown();  // Z <= 0 means pen down
    }
  }
}

// Extract the X, Y, or Z coordinate from the G-code command
float extractCoordinate(String gcode, char axis) {
  int idx = gcode.indexOf(axis);  // Find the position of the axis (X, Y, or Z)
  if (idx != -1) {
    return gcode.substring(idx + 1).toFloat();  // Extract the value after the axis letter
  }
  return NAN;  // Return NaN if the axis isn't found
}

// Move the pen to the specified X and Y coordinates
void moveTo(float x_mm, float y_mm) {
  long targetX = x_mm * stepsPerMM_X;  // Convert X coordinate to steps
  long targetY = y_mm * stepsPerMM_Y;  // Convert Y coordinate to steps

  stepperX.moveTo(targetX);  // Move X stepper to the target position
  stepperY.moveTo(targetY);  // Move Y stepper to the target position

  // Run the motors until they reach the target positions
  while (stepperX.distanceToGo() != 0 || stepperY.distanceToGo() != 0) {
    stepperX.run();
    stepperY.run();
  }
}

// Raise the pen using the servo
void penUp() {
  penServo.write(penUpPosition);  
}

// Lower the pen using the servo
void penDown() {
  penServo.write(penDownPosition);  // Move the servo to the "pen down" position
}


void autoHome() {
  
  stepperX.setSpeed(-200);  // Move in the negative direction towards the limit
  while (digitalRead(X_LIMIT_PIN) == HIGH) {
    stepperX.runSpeed();  // Run the motor until the limit switch is pressed
  }
  stepperX.setCurrentPosition(0);  // Set the current position as 0 (home)
  
  // Move towards the Y-axis limit switch
  stepperY.setSpeed(-200);  // Move in the negative direction towards the limit
  while (digitalRead(Y_LIMIT_PIN) == HIGH) {
    stepperY.runSpeed();  // Run the motor until the limit switch is pressed
  }
  stepperY.setCurrentPosition(0);  // Set the current position as 0 (home)
}
