# Aeronautical Anti-Icing System (Embedded Systems Project)

This is an **Embedded Systems** project that simulates the operation of an anti-icing system for aircraft. The project integrates hardware and software, demonstrating the application of control logic in a simulated environment.

## Demonstrated Concepts

*   **Embedded Systems:** Project involving the interaction between software (C code for microcontroller) and hardware (circuit simulation).
*   **C Programming for Microcontrollers:** The software is written in C and intended to run on a microcontroller (likely PIC, given the context of the project files `Sistema Antigelo.X`).
*   **Hardware Simulation:** Use of **Proteus** software to simulate the electronic circuit and the microcontroller, allowing the code to be tested in a virtual environment.
*   **Control Logic:** Implementation of an algorithm that monitors conditions (such as temperature and humidity) and activates the anti-icing system according to the defined control logic.
*   **Peripheral Communication:** Use of an LCD display (`lcd.h`, `main.c`) to show the system status.

## Project Structure

The project is divided into two main parts:

1.  **Software (`Sistema Antigelo.X`):** Contains the C source code (`main.c`, `lcd.h`) and the project files for the development environment (likely MPLAB X).
2.  **Hardware/Simulation (Files `.pdsprj`):** Contains the schematic diagram and the electronic circuit simulation in Proteus.

## How to View and Analyze

To view the complete project, **Proteus ISIS** software (for the circuit) and the **MPLAB X** development environment (for the C code) are required.

**Recommendation for Recruiters:** The value of this project lies in the **logic implemented in the `main.c` file** and the system concept. Analysis of the C code demonstrates the ability to write low-level software for hardware control.

**Language:** C (Software), Proteus (Hardware Simulation) **Topics:** Embedded Systems, Microcontrollers (PIC/AVR), Control Logic, Hardware Simulation, Low-Level Programming.
