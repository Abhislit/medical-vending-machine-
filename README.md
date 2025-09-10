
1. Overview
MediVend is an IoT-based smart medical vending machine that enables touchless medicine
dispensing. Users can scan a QR code, browse medicines on a mobile-friendly website, pay via
UPI/digital payment, and instantly receive medicines from the vending machine. The system
integrates ESP32, Arduino, servo motors, and IoT APIs for real-time control and inventory tracking.
2. Problem & Solution
Problem Statement:
- Lack of round-the-clock access to essential medicines in rural areas, highways, and emergencies.
- Manual vending/medical counters lead to time delays, hygiene issues, and poor stock
management.
Solution:
- A QR-enabled, contactless medicine vending machine with a web interface for browsing,
selection, and payment.
- Automated dispensing via ESP32 + Arduino controlled servos.
- Real-time inventory updates and admin alerts for stock management.
3. Logic & Workflow
Data Collection: User selects medicines on the web interface; transaction/payment data captured.
Processing: ESP32 validates payment and communicates with Arduino; servo motors and sensors
handle dispensing.
Output: Medicine is dispensed to the user; stock count updated in EEPROM database.
User Side: Scan QR → Browse medicines → Add to cart → Pay → Collect medicines.
Admin Side: Restock via /api/inventory; monitor inventory levels and system health.
4. Tech Stack
- Frontend: HTML, CSS, JavaScript (responsive web app with cart & payment UI)
- Backend / IoT: ESP32 (C++ Arduino framework), WebServer library
- Hardware: Arduino + Servo Motors + IR Sensors + LEDs + Buzzer
- Database: EEPROM (on ESP32 for inventory persistence)
- Communication: REST APIs hosted on ESP32
5. Future Scope
- Aadhaar-based user authentication
- Doctor’s e-prescription verification
- Solar power integration for rural/off-grid deployment# ESP32 Medicine Vending Machine Setup Guide

## Hardware Requirements

### ESP32 Board
- ESP32 DevKit C or similar
- Minimum 520KB RAM, 4MB Flash

### Components 
- **10x Servo Motors** (SG90 or MG996R) - for dispensing mechanisms
- **1x Buzzer** - for audio feedback
- **3x LEDs** - status indicators (built-in LED + 2 external)
- **10x IR Sensors** (optional) - to detect dispensed items
- **Resistors** - 220Ω for LEDs, 10kΩ for pull-ups
- **Power Supply** - 12V/5A for servos, 5V/2A for ESP32
- **Breadboard/PCB** - for connections
- **Jumper Wires**

## Wiring Diagram

```
ESP32 Pin Connections:
├── Servo Motors (PWM Control)
│   ├── GPIO 13 → Servo 1 (Medicine Slot 1)
│   ├── GPIO 12 → Servo 2 (Medicine Slot 2)
│   ├── GPIO 14 → Servo 3 (Medicine Slot 3)
│   ├── GPIO 27 → Servo 4 (Medicine Slot 4)
│   ├── GPIO 26 → Servo 5 (Medicine Slot 5)
│   ├── GPIO 25 → Servo 6 (Medicine Slot 6)
│   ├── GPIO 33 → Servo 7 (Medicine Slot 7)
│   ├── GPIO 32 → Servo 8 (Medicine Slot 8)
│   ├── GPIO 35 → Servo 9 (Medicine Slot 9)
│   └
