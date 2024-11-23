#include <AccelStepper.h>
#include <Servo.h>
#include <math.h>  // For trigonometric functions

// Pin definitions for stepper motors
#define X_STEP_PIN 3
#define X_DIR_PIN 2
#define Y_STEP_PIN 10
#define Y_DIR_PIN 9

// Pin definition for the servo
#define SERVO_PIN 4

// Stepper motor objects
AccelStepper stepperX(AccelStepper::DRIVER, X_STEP_PIN, X_DIR_PIN);
AccelStepper stepperY(AccelStepper::DRIVER, Y_STEP_PIN, Y_DIR_PIN);
Servo penServo;

// Steps needed to plot one millimeter on the page
float stepsPerMM_X = 2.5;  // Set this according to your calibration
float stepsPerMM_Y = 1.66;  // Set this according to your calibration

// Pen positions
int penUpPos = 90;   // Servo position for pen up
int penDownPos = 0;  // Servo position for pen down

// Movement parameters
const int stepCount = 25; // Steps to move for each command

void setup() {
    Serial.begin(9600);
    penServo.attach(SERVO_PIN);
    penUp();  // Start with the pen up

    // Max speed and acceleration for steppers
    stepperX.setMaxSpeed(500);       
    stepperY.setMaxSpeed(500);       
    stepperX.setAcceleration(200);    
    stepperY.setAcceleration(200);    
}

void loop() {
    // Wait for input from the serial monitor
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');  // Read the command string
        Serial.print("Command Received: ");
        Serial.println(command);

        if (command == "w") { moveY(1); }  // Move Y up
        else if (command == "s") { moveY(-1); }  // Move Y down
        else if (command == "a") { moveX(-1); }  // Move X left
        else if (command == "d") { moveX(1); }  // Move X right
        else if (command == "u") { penUp(); }  // Pen up
        else if (command == "pd") { penDown(); }  // Pen down
        else if (command.startsWith("circle")) { 
            drawCircle(50, 50, 30);  // Example: draw a circle with center at (50, 50) and radius of 30 mm
        } else {
            Serial.println("Unknown command");
        }
    }
}

// Function to raise the pen using the servo
void penUp() {
    penServo.write(penUpPos);
    Serial.println("Pen Up");
}

// Function to lower the pen using the servo
void penDown() {
    penServo.write(penDownPos);
    Serial.println("Pen Down");
}

// Function to move Y
void moveY(int direction) {
    stepperY.move(stepsPerMM_Y * direction * stepCount);  // Move Y in the specified direction
    stepperY.runToPosition();                  // Wait for the move to complete
    Serial.print("Moved Y: ");
    Serial.println(direction > 0 ? "Up" : "Down");
}

// Function to move X
void moveX(int direction) {
    stepperX.move(stepsPerMM_X * direction * stepCount);  // Move X in the specified direction
    stepperX.runToPosition();                  // Wait for the move to complete
    Serial.print("Moved X: ");
    Serial.println(direction > 0 ? "Right" : "Left");
}

// Function to draw a circle given center (centerX, centerY) and radius in mm
void drawCircle(float centerX, float centerY, float radius) {
    const int segments = 100;  // Number of segments to approximate the circle
    float angleIncrement = 2 * M_PI / segments;  // Increment angle per segment

    penDown();  // Lower pen to start drawing

    for (int i = 0; i <= segments; i++) {
        float angle = i * angleIncrement;
        
        // Calculate next position on the circle
        float targetX = centerX + radius * cos(angle);
        float targetY = centerY + radius * sin(angle);

        // Move to calculated position
        moveToPosition(targetX, targetY);
    }

    penUp();  // Lift pen after drawing
}

// Helper function to move to a specific (x, y) position
void moveToPosition(float x, float y) {
    int deltaX = (x * stepsPerMM_X) - stepperX.currentPosition();
    int deltaY = (y * stepsPerMM_Y) - stepperY.currentPosition();

    stepperX.move(deltaX);
    stepperY.move(deltaY);

    stepperX.runToPosition();
    stepperY.runToPosition();
}
