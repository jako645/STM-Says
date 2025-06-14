# STM-Says

## Overview

This project implements the classic Simon Says game on the STM32 NUCLEO-L476RG development board. The game involves replicating a random sequence of colors displayed on LEDs, using buttons to input the sequence. The sequence starts with a single color and grows as the player successfully replicates the sequence. If the player enters an incorrect sequence, the game ends and a visual animation signals the failure.

<p align="center">
    <img src="media/SimoneSays_photo.jpg" alt="STM Says Hardware" width="600"/>
</p>

## Features

- **Random Color Sequence**: The game randomly selects a sequence of colors.
- **Button Input**: Player replicates the color sequence using physical buttons.
- **LED Feedback**: LEDs light up to display the sequence, and the player’s input is also shown.
- **Game Over Animation**: If the player makes a mistake, the game ends with a visual cue.

## Hardware Setup

The game uses the following components:
- **NUCLEO-L476RG board**: STM32L476 microcontroller
- **4 LEDs (Red, Green, Blue, Yellow)**: Display the color sequence.
- **4 Buttons**: Player inputs the sequence by pressing the corresponding buttons.

### Circuit Diagram
You can view the schematic of the hardware setup in the `schematic/` folder:
- [Electronic_Schematic.pdf](schematic/Electronic_Schematic.pdf)
