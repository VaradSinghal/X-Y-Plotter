from machine import Pin, PWM
from math import cos, sin, radians, pi
import time

# Define servo and motor pins
SERVO_PIN = 4
MOTOR1_STEP_PIN = 10
MOTOR1_DIR_PIN = 9
MOTOR2_STEP_PIN = 3
MOTOR2_DIR_PIN = 2

# Initialize servo
servo = PWM(Pin(SERVO_PIN))
servo.freq(50)  # Typical servo frequency is 50 Hz

# Define circle parameters
radius = 100  # Circle radius
steps_per_revolution = 200  # Steps for a full rotation (adjust for your motor)
num_steps = 360  # Steps for a complete circle
angle_step = 360 / num_steps  # Angle increment
pen_up_position = 20  # Adjusted for MicroPython (20°)
pen_down_position = 90  # Adjusted for MicroPython (90°)

# Initialize motor pins
motor1_step = Pin(MOTOR1_STEP_PIN, Pin.OUT)
motor1_dir = Pin(MOTOR1_DIR_PIN, Pin.OUT)
motor2_step = Pin(MOTOR2_STEP_PIN, Pin.OUT)
motor2_dir = Pin(MOTOR2_DIR_PIN, Pin.OUT)

# Set the pen to the "down" position
def set_servo_angle(angle):
    duty = int((angle / 180) * 1023 + 26)  # Calculate duty for MicroPython (ESP32/ESP8266)
    servo.duty(duty)

# Define a function to step the motors
def step_motor(motor_step, motor_dir, steps, direction):
    motor_dir.value(direction)
    for _ in range(steps):
        motor_step.on()
        time.sleep_us(500)  # Control speed by adjusting delay
        motor_step.off()
        time.sleep_us(500)

def draw_circle():
    set_servo_angle(pen_down_position)
    time.sleep(2)
    
    for i in range(num_steps):
        angle = i * angle_step
        x = radius * cos(radians(angle))
        y = radius * sin(radians(angle))
        
        # Convert x and y positions to steps for each motor
        step1 = int(x * (steps_per_revolution / (2 * pi * radius)))
        step2 = int(y * (steps_per_revolution / (2 * pi * radius)))
        
        # Move each motor
        step_motor(motor1_step, motor1_dir, abs(step1), 1 if step1 > 0 else 0)
        step_motor(motor2_step, motor2_dir, abs(step2), 1 if step2 > 0 else 0)
        
        time.sleep(0.01)  # Small delay to slow down the drawing

    set_servo_angle(pen_up_position)
    time.sleep(2)

# Run once
draw_circle()
