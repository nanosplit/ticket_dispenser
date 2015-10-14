# Ticket Dispenser

A simple concession ticket dispenser using:
- Arduino Uno Rev3 : http://store-usa.arduino.cc/products/a000066
- Deltronic DL-1275 ticket dispenser : http://www.deltroniclabs.com/?post_type=product&p=165
- Membrane 3x4 Matrix Keypad : https://www.adafruit.com/products/419
- Adafruit 0.56" 4 digit 7 segment display w/I2C Backpack : https://www.adafruit.com/products/881

#### Features
- Enter # to dispense, hit # on the keypad, it dispenses then resets
- Display updates as you type in your #
- Rock solid counting - I developed a 'state machine' that will only allow a ticket to be counted if certain conditions exist to cut down 'bad' counts
- Empty roll detection - if 3 seconds passes without detecting a ticket, it will automatically cut-off. Time is adjustable.
- Cancel active job by pressing asterisk *
- Entered over 4 digits, program resets (limit of display)
- If you hit the process key (#) with no numbers entered, it resets

___

## Dispensing Mechanism:
- Deltronic DL-1275 ticket dispenser : [http://www.deltroniclabs.com](http://www.deltroniclabs.com/?post_type=product&p=165)

#### DL-1275 Wiring
- Blue Wire : 12 : Optical sensor (ticket count trigger)
- Black Wire : GND : Ground
- White Wire : 11 : DC Motor driver : on/off
- Red Wire : Vin : 12V constant

___

## Keypad
Using Membrane 3x4 Matrix Keypad : [https://www.adafruit.com/](https://www.adafruit.com/products/419)
- http://playground.arduino.cc/Code/Keypad
- https://www.adafruit.com/products/419 (keypad_test source)

#### Keypad Wiring:
- pins 2-8
- Check http://www.adafruit.com/images/large/membranekeypad34arduino_LRG.jpg

___

## 7 Segment Display
Using Adafruit 0.56" 4 digit 7 segment display w/I2C Backpack : [https://www.adafruit.com/](https://www.adafruit.com/products/881)
- http://www.instructables.com/id/Buzz-Wire-Alarm-Clock/step6/Coding/

#### 7 Segment Display Wiring:
- + = 5v
- - = GND
- D = A4
- C = A5

___

## Youtube
- Video 1: https://www.youtube.com/watch?v=NaVk65nHHyM
- Video 2: https://www.youtube.com/watch?v=bZLrfu0cXWg
