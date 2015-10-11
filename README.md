# Ticket Dispenser

C code for a simple concession ticket dispenser using:
- Deltronic DL-1275 ticket dispenser : http://www.deltroniclabs.com/?post_type=product&p=165
- Membrane 3x4 Matrix Keypad : https://www.adafruit.com/products/419
- Adafruit 0.56" 4 digit 7 segment display w/I2C Backpack : https://www.adafruit.com/products/881
---
#### DL-1275 Wiring:
Powering the Deltronic DL-1275 ticket dispenser : [http://www.deltroniclabs.com](http://www.deltroniclabs.com/?post_type=product&p=165)
- Blue Wire : 12 : Optical sensor (ticket count trigger)
- Black Wire : GND : Ground
- White Wire : 11 : DC Motor driver : on/off
- Red Wire : Vin : 12V constant
---
### Keypad
Using Membrane 3x4 Matrix Keypad : [https://www.adafruit.com/](https://www.adafruit.com/products/419)
- http://playground.arduino.cc/Code/Keypad
- https://www.adafruit.com/products/419 (keypad_test source)

#### Keypad Wiring:
- pins 2-8
- Check http://www.adafruit.com/images/large/membranekeypad34arduino_LRG.jpg
---
### 7 Segment Display
Using Adafruit 0.56" 4 digit 7 segment display w/I2C Backpack : [https://www.adafruit.com/](https://www.adafruit.com/products/881)
- http://www.instructables.com/id/Buzz-Wire-Alarm-Clock/step6/Coding/

#### 7 Segment Display Wiring:
- + = 5v
- - = GND
- D = A4
- C = A5
