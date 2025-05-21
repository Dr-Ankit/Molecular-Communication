# Molecular-Communication

## Overview

This repository explores an innovative, biologically inspired approach to communication known as **Molecular Communication (MC)**. Unlike conventional systems that rely on electromagnetic (EM) waves, MC uses **chemical molecules** as information carriers. This paradigm is particularly effective in environments where EM communication fails—such as inside pipelines, underwater, in biological tissues, or within nanoscale devices.

Inspired by how nature transmits information (e.g., pheromones in insects, hormone signaling in cells), MC offers advantages such as:

* Robustness in signal-blocking environments
* Energy efficiency
* Biocompatibility 

This project demonstrates a basic **transmitter-receiver setup using Arduino microcontrollers**, capable of encoding, transmitting, and decoding molecular signals.

### `transmitter_code.ino`
This Arduino sketch is responsible for **encoding and releasing chemical signals** into the environment. It uses actuators like relays, pumps, and fans to emit molecules (e.g., alcohol vapor) that represent digital bits (e.g., binary 1s and 0s). Transmission timing is critical, and the code manages signal synchronization with delays and control pulses.

### `receiver_code.ino`
This sketch handles the **detection and decoding** of the transmitted molecular signals. It typically uses a gas sensor (e.g., MQ-3) to detect the presence or absence of specific chemical concentrations. The receiver interprets the signals based on predefined thresholds and timing intervals to reconstruct the transmitted message.
