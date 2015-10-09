# Ticket Dispenser

C code for a simple concession ticket dispenser using:
- Deltronic DL-1275 ticket dispenser : http://www.deltroniclabs.com/?post_type=product&p=165
- Membrane 3x4 Matrix Keypad : https://www.adafruit.com/products/419
- Adafruit 0.56" 4 digit 7 segment display w/I2C Backpack : https://www.adafruit.com/products/881

### Wiring Guide
Powering the Deltronic DL-1275 ticket dispenser : [http://www.deltroniclabs.com](http://www.deltroniclabs.com/?post_type=product&p=165)
- Blue Wire: Optical sensor (ticket count trigger)
- Black Wire: Ground
- White Wire: DC Motor driver : on/off
- Red Wire: 12V constant

### Keypad
Using Membrane 3x4 Matrix Keypad : [https://www.adafruit.com/](https://www.adafruit.com/products/419)
- http://playground.arduino.cc/Code/Keypad
- https://www.adafruit.com/products/419 (keypad_test source)

### 7 Segment Display
Using Adafruit 0.56" 4 digit 7 segment display w/I2C Backpack : [https://www.adafruit.com/](https://www.adafruit.com/products/881)
- http://www.instructables.com/id/Buzz-Wire-Alarm-Clock/step6/Coding/
