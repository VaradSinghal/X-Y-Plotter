from machine import Pin, PWM
import time

# Pin definitions
X_STEP_PIN = Pin(2, Pin.OUT)
X_DIR_PIN = Pin(3, Pin.OUT)
Y_STEP_PIN = Pin(4, Pin.OUT)
Y_DIR_PIN = Pin(5, Pin.OUT)

X_LIMIT_PIN = Pin(6, Pin.IN, Pin.PULL_UP)
Y_LIMIT_PIN = Pin(7, Pin.IN, Pin.PULL_UP)

SERVO_PIN = PWM(Pin(9))

# Steps per millimeter for the X and Y axes (adjust based on your setup)
stepsPerMM_X = 80
stepsPerMM_Y = 80

# Servo control angles for pen
penUpPosition = 90
penDownPosition = 0

# Motor control variables
max_speed = 1000
acceleration = 500

# Set initial servo position
def penUp():
    SERVO_PIN.duty_u16(int(65536 * (penUpPosition / 180.0)))

def penDown():
    SERVO_PIN.duty_u16(int(65536 * (penDownPosition / 180.0)))

# Home the steppers using limit switches
def autoHome():
    # Move towards the X-axis limit switch
    X_DIR_PIN.off()  # Move in negative direction
    while X_LIMIT_PIN.value():
        X_STEP_PIN.on()
        time.sleep_us(1000)  # Control speed of stepping
        X_STEP_PIN.off()
        time.sleep_us(1000)
    X_DIR_PIN.on()  # Reset direction

    # Move towards the Y-axis limit switch
    Y_DIR_PIN.off()  # Move in negative direction
    while Y_LIMIT_PIN.value():
        Y_STEP_PIN.on()
        time.sleep_us(1000)
        Y_STEP_PIN.off()
        time.sleep_us(1000)
    Y_DIR_PIN.on()  # Reset direction

# Move the stepper motors to a specific position
def moveTo(x_mm, y_mm):
    targetX = int(x_mm * stepsPerMM_X)
    targetY = int(y_mm * stepsPerMM_Y)

    # Move X-axis
    for _ in range(abs(targetX)):
        X_STEP_PIN.on()
        time.sleep_us(1000)
        X_STEP_PIN.off()
        time.sleep_us(1000)

    # Move Y-axis
    for _ in range(abs(targetY)):
        Y_STEP_PIN.on()
        time.sleep_us(1000)
        Y_STEP_PIN.off()
        time.sleep_us(1000)

# Function to process G-code and execute movements or pen control
def processGCode(gcode):
    gcode = gcode.strip()
    if gcode.startswith("G0") or gcode.startswith("G1"):
        x = extractCoordinate(gcode, 'X')
        y = extractCoordinate(gcode, 'Y')
        if x is not None and y is not None:
            moveTo(x, y)
    if 'Z' in gcode:
        z = extractCoordinate(gcode, 'Z')
        if z > 0:
            penUp()
        else:
            penDown()

# Extract the X, Y, or Z coordinate from the G-code command
def extractCoordinate(gcode, axis):
    idx = gcode.find(axis)
    if idx != -1:
        try:
            return float(gcode[idx + 1:])
        except ValueError:
            pass
    return None

# Main program loop
def main():
    penUp()  
    autoHome()  

    while True:
        if uart.any():
            gcode = uart.readline().decode().strip()
            processGCode(gcode)

# Initialize UART for receiving G-code
from machine import UART
uart = UART(0, baudrate=115200, tx=Pin(0), rx=Pin(1))

main()
