
---

# Vole Machine Language Simulator

**A C++ Object-Oriented Simulation of the Vole CPU Architecture.**

## 💻 Project Overview

This project is a functional simulator for the **Vole Machine**. The simulator is designed to fetch, decode, and execute machine language instructions, providing a clear view of the internal state of the CPU, including registers, program counter (PC), instruction register (IR), and main memory.

The core of the project focuses on **Object-Oriented Design (OOD)**, using abstraction and composition to model the interaction between the processor and memory.

### Key Features


**Instruction Cycle:** Fully simulates the Fetch-Decode-Execute cycle.



**Memory Management:** Supports a 256-cell memory space.



**Register File:** Includes 16 general-purpose registers.



**Detailed Status Reporting:** Real-time display of the PC, IR, registers, and memory after each execution step.



**File I/O:** Loads machine code programs from external text files.



---

## 🛠️ Supported Instruction Set

The simulator supports the following machine language operations:

* **LOAD (1, 2):** Load data from memory or immediate values into registers.
* **STORE (3):** Move data from registers to memory (including memory-mapped screen output at address `00`).
* **MOVE (4):** Copy bit patterns between registers.
* **ADD (5, 6):** Perform Two's Complement and Floating-Point addition.
* **JUMP (B):** Conditional branching based on register comparisons.
* **HALT (C):** Terminate program execution.

---

## 📐 Architecture & Design

The system is built using **Modular C++** and separate compilation (headers and implementation files). The design is centered around several key classes:

* **Machine:** The top-level controller that manages the overall simulation.
* **CPU:** Handles the execution logic, containing the Register file and ALU.
* **Memory:** Manages the storage cells and data retrieval.
* **Instruction:** An abstraction for decoding and validating opcode-operand pairs.



---

## 👥 Team Collaboration

Developed as a group project of 3, this repository showcases:

* **Integrated Codebase:** Merging individual modules into a single working application.


* **UML Modeling:** Systematic design of class relationships and hierarchies.


* **Version Control:** Developed and managed through a private GitHub repository.



---

## 🚀 How to Use

1. **Clone the Repository:**
```bash
git clone https://github.com/yourusername/vole-machine-simulator.git

```


2. **Compile:** Use any standard C++ compiler (C++11 or higher recommended).


3. **Run:** Load a formatted text file (e.g., `program.txt`) containing space-separated instructions like `10E0 11E1 5201 C000`.


4. **Step Through:** Use the menu to execute instructions step-by-step or run the full program to completion.



---
