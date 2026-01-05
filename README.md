# Arduino Traffic Light Controller (FSM-Based)

## Overview
This project implements a **non-blocking traffic light controller** on an Arduino Uno using **finite state machine (FSM) architecture**.  
It supports a **pedestrian crossing button**, including a **solid WALK phase** followed by a **flashing DON’T WALK warning**, all without using blocking delays.

The design emphasizes **deterministic behavior**, **event-driven logic**, and **clean separation of concerns**, similar to real embedded control systems.

---

## Features
- Traffic light FSM (GREEN → YELLOW → RED)
- Pedestrian FSM (IDLE → WALK → FLASH → IDLE)
- Debounced active-low push button using internal pull-up
- Non-blocking timing using `millis()`
- Minimum green time safety constraint
- Flashing “don’t walk” warning phase
- Modular, readable embedded C++ code

---

## Hardware Used
- Arduino Uno
- Breadboard
- 3 LEDs (Red, Yellow, Green)
- 1 LED (Pedestrian WALK)
- 220 Ω resistors
- 4-pin tactile push button
- Jumper wires

---

## Software Concepts Demonstrated
- Finite State Machines (FSM)
- Non-blocking scheduling
- Event latching
- Software debouncing
- Active-low digital inputs
- Separation of control logic and hardware outputs

---

## Wiring
See `docs/wiring_diagram.jpg` for full wiring details.

---

## State Machine Design

### Traffic FSM
- GREEN
- YELLOW
- RED

### Pedestrian FSM
- IDLE
- WALK (solid)
- FLASH (don’t walk warning)

The pedestrian FSM only serves requests when the traffic FSM is in RED, ensuring safe operation.

---

## Demo
A short demo video showing normal operation and pedestrian requests is available here:

👉 `demo/traffic_controller_demo.mp4`

---

## How to Run
1. Open `src/traffic_controller.ino` in the Arduino IDE
2. Select **Board: Arduino Uno**
3. Select the correct COM port
4. Upload the sketch
5. Press the pedestrian button during GREEN to request crossing

---

## Future Improvements
- Night mode (blinking yellow)
- Interrupt-driven button handling
- Serial telemetry logging
- Two-direction intersection support

---

## Author
**Jared Pearson**  
Computer Engineering | Embedded & Firmware Systems
