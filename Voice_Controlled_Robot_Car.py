from machine import Pin, UART
import utime

# Define motor pins
motor_pins = [10, 11, 12, 13]  # Replace these with your actual motor pins
motors = [Pin(pin, Pin.OUT) for pin in motor_pins]

# Define ultrasonic sensor pins
ultrasonic_pins = [(2, 3), (4, 5)]  # Replace these with your actual sensor pins
trig_pins = [Pin(pin[0], Pin.OUT) for pin in ultrasonic_pins]
echo_pins = [Pin(pin[1], Pin.IN) for pin in ultrasonic_pins]

# Define Bluetooth UART
uart = UART(0, baudrate=9600, tx=Pin(0), rx=Pin(1))  # Replace tx, rx with your UART pins

# Constants
STOP_DISTANCE_FRONT = 10
STOP_DISTANCE_BACK = 20

# Function to control motors
def move_motors(motor_states):
    for i, motor in enumerate(motors):
        if motor_states[i] == 1:
            motor.on()
        elif motor_states[i] == 0:
            motor.off()

# Function to perform forward movement
def forward():
    move_motors([1, 0, 1, 0])  # Define appropriate motor states for forward movement

# Function to perform backward movement
def backward():
    move_motors([0, 1, 0, 1])  # Define appropriate motor states for backward movement

# Function to perform left turn
def left():
    move_motors([1, 0, 0, 1])  # Define appropriate motor states for left turn

# Function to perform right turn
def right():
    move_motors([0, 1, 1, 0])  # Define appropriate motor states for right turn

# Function to stop all motors
def stop():
    move_motors([0, 0, 0, 0])

# Function to measure distance using ultrasonic sensor
def measure_distance(trig_pin, echo_pin):
    # Send a pulse to trigger the ultrasonic sensor
    trig_pin.off()
    utime.sleep_us(2)
    trig_pin.on()
    utime.sleep_us(5)
    trig_pin.off()

    # Measure the duration of the pulse sent by the sensor
    pulse_time = machine.time_pulse_us(echo_pin, 1, 30000)  # Maximum timeout of 30ms

    # Calculate distance based on the speed of sound
    # Speed of sound is approximately 34300 cm/s (or 343 m/s)
    # Distance = (time * speed_of_sound) / 2 (since the pulse travels to and from)
    distance = (pulse_time * 34300) / 2 if pulse_time > 0 else 0

    return distance


# Main loop
while True:
    # Measure distances
    distance_front = measure_distance(trig_pins[0], echo_pins[0])
    distance_back = measure_distance(trig_pins[1], echo_pins[1])

    # Check for obstacles and take action
    if distance_front < STOP_DISTANCE_FRONT:
        stop()
        backward()
        utime.sleep(0.1)
        stop()
        print("STOP..._obstacle1")

    if distance_back < STOP_DISTANCE_BACK:
        stop()
        forward()
        utime.sleep(0.1)
        stop()
        print("STOP..._obstacle2")

    # Check for Bluetooth commands
    if uart.any():
        command = uart.readline().strip().decode('utf-8')
        print(command)
        if command == "go ahead":
            forward()
        elif command == "go back":
            backward()
        elif command == "stop":
            stop()
        elif command == "left":
            left()
        elif command == "right":
            right()
