# EcoBin Alert System 

A smart waste bin monitoring system using ESP32, ultrasonic sensors, and ThingSpeak dashboard integration. Designed to provide real-time bin fill levels and alert statuses for efficient waste management.

##  Features
- Real-time bin fill percentage tracking
- Status alerts: EMPTY, HALF, FULL
- Traffic-style LED indicators
- Cloud dashboard via ThingSpeak
- Compact and stakeholder-friendly visualization

##  Hardware
- ESP32 microcontroller
- HC-SR04 ultrasonic sensor
- Traffic LED module with built-in resistors
- Wi-Fi connectivity
- Jumper wires

##  Cloud Integration
- ThingSpeak channel with:
  - Field 1: Bin Fill Percentage
  - Field 2: Alert Status (0 = EMPTY, 1 = HALF, 2 = FULL)
  - LED widgets and charts for live monitoring

##  Files Included
- `EcoBin_ESP32.ino`: Arduino code for ESP32
- `ThingSpeak_Dashboard_Screenshot.png`: Live dashboard view
  
##  Future Enhancements
- Buzzer alert for critical fill levels
- SMS/email notifications
- Multi-bin dashboard integration

##  License
MIT License
