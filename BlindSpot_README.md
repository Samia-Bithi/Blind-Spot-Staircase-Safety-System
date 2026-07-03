# Blind Spot — Adaptive Staircase Lighting and Fall Prevention System

An intelligent embedded systems project that combines predictive staircase lighting, fall detection, and IoT-based caregiver notification into a unified safety platform.

---

## Overview

Blind Spot is a staircase safety system built on the ATmega32 microcontroller. Unlike conventional systems that react only after motion is detected, this system **anticipates user movement** and illuminates staircase zones ahead of the user before arrival.

In the event of a fall or prolonged inactivity on the staircase, the system automatically sends an alert to a caregiver's smartphone through the **Blynk platform** via WiFi.

---

## Features

- Predictive LED illumination based on detected direction of travel
- Adaptive brightness control using ambient light sensing (LDR + PWM)
- Two-stage fall detection and warning system
- Remote caregiver notification via ESP32 + Blynk (WiFi)
- Password-protected door entry using push buttons and servo motor
- Real-time system status display on OLED

---

## Components Used

| Component | Purpose |
|-----------|---------|
| ATmega32 + AVR Programmer | Main microcontroller and programming interface |
| HC-SR501 PIR Sensors | Presence detection and directional movement logic |
| IR Sensor | Entry and exit confirmation |
| LDR + Resistor | Ambient light sensing |
| LEDs | Adaptive staircase zone illumination |
| OLED Module | System status display |
| ESP32 Module | WiFi communication and Blynk integration |
| Buzzer | Local warning and alert indication |
| LM2596 Buck Converter | 5V regulated power supply |
| Lithium Battery | Power source |
| Push Buttons | Password input |
| Servo Motor | Door lock operation |

---

## How It Works

1. **Door Entry** — User inputs a pattern-based password via push buttons. Correct input triggers the servo motor to unlock the door.

2. **Direction Detection** — PIR sensors at entry points use sequential triggering logic to determine whether the user is going up or down the staircase.

3. **Predictive Lighting** — LED zones ahead of the user are activated before arrival. The LDR continuously measures ambient brightness and the ATmega32 adjusts PWM duty cycles accordingly.

4. **Fall Detection** — Mid-stair PIR sensors monitor for inactivity. If movement stops beyond a set duration:
   - **Stage 1:** Local buzzer warning activates
   - **Stage 2:** Remote notification sent to caregiver's smartphone via Blynk

---

## ATmega32 Features Utilized

- GPIO interfacing for sensors, LEDs, LCD, buttons, and buzzer
- ADC for real-time ambient light measurement
- PWM generation for adaptive brightness control
- UART communication with ESP32
- Timers and interrupts for fall detection and event handling

---

## Repository Structure

```
├── src/
│   ├── Source_Code_Staircase.c       # Main staircase lighting and fall detection logic
│   ├── Password_based_door_lock.c    # Password entry and door lock system
│   └── esp_communication/            # ESP32 WiFi and Blynk integration code
├── docs/
│   ├── Blind_Spot.pdf                # Project proposal
│   ├── Group_4_report.pdf            # Full project report
│   └── Group_4.pptx                  # Presentation slides
└── README.md
```

---

## How to Compile (ATmega32)

Using AVR-GCC:
```bash
avr-gcc -mmcu=atmega32 -o output.elf Source_Code_Staircase.c
avr-objcopy -O ihex output.elf output.hex
avrdude -c usbasp -p m32 -U flash:w:output.hex
```

---

## Tools & Technologies

- Embedded C
- ATmega32 (AVR microcontroller)
- ESP32 (WiFi + Blynk IoT)
- AVR Studio / AVRDUDE
- Blynk IoT Platform
- PWM, ADC, UART, GPIO, Timers

---

## Project Context

Developed as part of **MTE 3104 — Group 4** coursework project.
