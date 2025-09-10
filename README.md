
<img width="541" height="335" alt="Screenshot 2025-09-10 205407" src="https://github.com/user-attachments/assets/98d46d14-7800-48b0-b6af-bdad43ba89e3" />
<img width="922" height="403" alt="Screenshot 2025-09-10 205325" src="https://github.com/user-attachments/assets/377b97f4-747b-41c3-bb55-a9bddaf19175" />
<img width="926" height="405" alt="Screenshot 2025-09-10 205148" src="https://github.com/user-attachments/assets/3feb764f-c7a5-4581-a215-3398835339eb" />
<img width="1601" height="870" alt="image1" src="https://github.com/user-attachments/assets/efd28f42-a921-417b-a62c-047f4094b984" />
<img width="1232" height="1044" alt="image 2" src="https://github.com/user-attachments/assets/dff41d8f-38e4-498e-8c08-0841f129c52d" />
<img width="1582" height="930" alt="ima" src="https://github.com/user-attachments/assets/2fb85eee-83ac-4d89-a3dd-9ab2d61ce14b" />


# MediVend: IoT-Based Smart Medical Vending Machine

## Overview

MediVend is an IoT-powered, touchless medical vending machine that enables users to quickly and hygienically purchase medicines. Simply scan a QR code, select medicines on a mobile-friendly site, pay digitally, and collect your medicines—all controlled in real-time via ESP32, Arduino, and IoT APIs.

---

## Problem & Solution

### The Problem
- Limited 24/7 access to essential medicines, especially in rural or remote areas.!

- Manual counters cause delays, hygiene concerns, and inefficient stock management.

### Our Solution
- A QR-based, contactless medicine vending machine with a web interface for browsing, selection, and digital payment.
- Automated dispensing using ESP32 and Arduino-controlled servo motors.
- Real-time inventory tracking and automated admin notifications for restocking.

---

## System Workflow

### User Workflow

[```mermaid
flowchart TD![Screenshot_20250911_011938_Chrome(2)](https://github.com/user-attachments/assets/97bc3c40-b7b8-4623-8438-17c1b697164d)

    A[Scan QR Code] --> B[Browse & Select Medicines on Web App]
    B --> C[Add to Cart & Pay Online]
    C --> D[Payment Verified by ESP32]
    D --> E[Medicine Dispensed by Vending Machine]
    E --> F[Stock Updated in Database]
    F --> G[User Collects Medicines]
```](https://www.mermaidchart.com/app/projects/e9c2126a-ba20-4d1f-ad5a-42eebda2b1a4/diagrams/82db50a2-8b5a-4390-b297-93e3e34820f0/version/v0.1/edit)

### Admin Workflow

```mermaid
flowchart TD
    H[Monitor Inventory Dashboard]
    H --> I[Receive Low Stock Alerts]
    I --> J[Restock Medicines via /api/inventory]
    J --> H
    H --> K[Monitor System Health]![Screenshot_20250911_011938_Chrome(2)](https://github.com/user-attachments/assets/765712f1-fa8c-43ec-9e9a-860efb818a6c)

```

---

## Technology Stack

- **Frontend:** HTML, CSS, JavaScript (mobile-optimized web app)
- **Backend / IoT:** ESP32 (using the Arduino framework, WebServer library)
- **Hardware:** Arduino, Servo Motors, IR Sensors, LEDs, Buzzer
- **Database:** EEPROM on ESP32 for persistent inventory
- **APIs:** REST APIs hosted on ESP32

---

## Hardware Requirements

- **ESP32 Board:** DevKit C or similar (min. 520KB RAM, 4MB Flash)
- **10x Servo Motors** for dispensing
- **Buzzer, LEDs, IR Sensors** for feedback and detection
- **Power Supply:** 12V/5A for servos, 5V/2A for ESP32
- **Miscellaneous:** Breadboard/PCB, jumper wires, resistors

---

## Future Enhancements

- Aadhaar-based user authentication
- Doctor’s e-prescription verification
- Solar power for off-grid deployments

---

