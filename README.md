# Maze-Solver

## Introduction

This repository contains code for a maze-solving robot implemented using a Zumo 32U4 robot platform. The code is designed to enable the robot to search a maze until it finds a houses or reaches the maximum move count, upon reaching either of these thresholds the robot will return to its start point and begin searching again. The robot will repeat this robot until a defined number of Houses have been found and it has returned to the start for the final time.

## References

- [Arduino](https://www.arduino.cc/)
- [Pololu Zumo 32U4 Robot](https://www.pololu.com/category/170/zumo-32u4-robot)

## Instructions

### Software Setup

1. Install the Arduino IDE from the [Arduino website](https://www.arduino.cc/en/software) or using Apps Anywhere on a SHU Computer.
2. Install the Zumo 32U4 libraries:
   - Open the Arduino IDE.
   - Navigate to **Sketch > Include Library > Manage Libraries...**
   - Search for "Zumo32U4" and install the library.

### Building and Uploading the Code

1. Open the provided code in the Arduino IDE.
2. Connect the Arduino board to your computer using a USB cable.
3. Select the appropriate board and port in the Arduino IDE.
4. Click the upload button to compile and upload the code to the Arduino board.

### Using the Program

1. After uploading the code, place the robot half way over a black line, and then power on the robot.
2. The robot will then calibrate its sensors by rotating 180deg in both directions.
3. Once the calibration is completed the robot will imediately start searching the maze.
4. The robot emits sounds using the buzzer when a house is found or when it reaches the maximum move count.
5. The robot will then backtrack and emit sounds accordingly.
6.The robot will search the maze until it finds the maximum number of houses defined by `MAX_HOUSE`.
7. After completing the task, the robot will stop moving.

### Functionality Overview

- `calibrateSensors()`: Calibrates the line and proximity sensors during setup.
- `initializeLineSensors()`: Initializes the line sensors.
- `initializeProximitySensors()`: Initializes the proximity sensors.
- `printReadingsToSerial()`: Prints sensor readings to the serial monitor for debugging purposes.
- `turnAround()`, `turnLeft()`, `turnRight()`, `reverse()`, `forward()`: Functions for controlling the robot's movement.
- `loop()`: The main loop of the program where the robot continuously reads sensor data and makes decisions based on it.

### Notes

- Ensure proper calibration of sensors for optimal performance.
- Adjust movement parameters (`turnAround()`, `turnLeft()`, etc.) as needed based on your robot's physical characteristics.
- If the black line detection sensitivity needs changing locate `LINE_THRESHOLD` and change this value acordingly.
- If the max number of moves is too few this value can be adjusted with `MAX_MOVES`.
- To change the number houses to search for before stopping change the `MAX_HOUSE` value.

Feel free to reach out for any further assistance or clarification at c1009859@hallam.shu.ac.uk.
