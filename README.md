# EStopAuthV3

## Description

EStopAuthV3 is the third iteration of a lightweight hashing authentication protocol for RPI's IGVC Emergency Stop. It uses BLAKE2s as the hashing algorithm and has robust Nonces to prevent replay attacks. This protocol ensures only authenticated signals trigger the Emergency Stop for our robot.

## Hardware Requirements

- Arduino boards (Uno)
- Buttons, 220Ω resistors, jumper wires.

## Software Requirements

- Arduino IDE
- Rhys Weatherley's arduinolibs Crypto library
- Mike McCauley's Radiohead library

## Installation

1. Clone this repository.
2. Build the required circuitry.
3. Open the appropriate .ino file in Arduino IDE.
4. Install any required libraries via Library Manager.
5. Upload the sketch to your Arduino board.

## Usage

The emitter hashes and packages the packet that will be sent to a receiver over wireless transmission. The reciever will unpackage and verify that the received packet was sent from a good actor.

## License

MIT license