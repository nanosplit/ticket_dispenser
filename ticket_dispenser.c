#include <Wire.h>

#include "Keypad.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_7segment matrix = Adafruit_7segment();

// Setup the keypad
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {1,2,3},
  {4,5,6},
  {7,8,9},
  {'*','^','#'}
};
byte rowPins[ROWS] = {8, 7, 6, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
// end keypad

int count = 0;
int n1 = 0;
int n2 = 0;
int n3 = 0;
int n4 = 0;
int dn2 = 0;
int dn3 = 0;
int dn4 = 0;
int dispense = 0;
int tickets_dispensed = 1;

int tsignal = 12; // opto-sensor pin# (blue wire on DL-1275)
int tmotor = 11; // motor control pin# (white wire on DL-1275)
int tcount = 0; // counter for # of tickets to dispense
int tsignal_status = 0; // listner for opto-sensor LOW / HIGH status

void setup() {
#ifndef __AVR_ATtiny85__
  Serial.begin(9600);
  Serial.println("7 Segment + Keypad Test");
#endif
  pinMode(tsignal, INPUT_PULLUP);
  pinMode(tmotor, OUTPUT);
  matrix.begin(0x70);
  matrix.setBrightness(1);
  matrix.print(10000, DEC);
  matrix.writeDisplay();
  delay(50);
  matrix.clear();
  matrix.writeDisplay();
  matrix.printNumber(0);
  matrix.writeDisplay();
}

void(* resetFunc) (void) = 0;
void loop() {

  int tsignal_status = digitalRead(tsignal);

  char key = keypad.getKey();
  if (key != NO_KEY) {

    switch (key) {
      case '#' :
        matrix.clear();
        matrix.writeDisplay();
        matrix.println(0);
        matrix.writeDisplay();
        digitalWrite(tmotor, HIGH);
        for (uint16_t sig_count = tickets_dispensed; tickets_dispensed <= dispense; sig_count) {
          if (digitalRead(tsignal) == LOW)  // check if signal received from opto-sensor
          {
            delay(120);
            matrix.println(tickets_dispensed);
            tickets_dispensed++;
          }
          matrix.writeDisplay();
        }
        digitalWrite(tmotor, LOW);
        Serial.println("Done dispensing... resetting...");
        delay(500);
        resetFunc();

      case '*' :
        // need interrupt to cancel ticket dispensing
        delay(500);
        resetFunc();
    }

    switch (count) {
      case 0 :
        matrix.clear();
        matrix.writeDisplay();
        count++;
        n1 = key;
        dispense = n1;
        matrix.print(n1);
        matrix.writeDisplay();
      break;

      case 1 :
        count++;
        if (key == '^') {
          n2 = 0;
        } else {
          n2 = key;
        }
        dn2 = n1*10+n2;
        dispense = dn2;
        matrix.print(dn2);
        matrix.writeDisplay();
      break;

      case 2:
        count++;
        if (key == '^') {
          n3 = 0;
        } else {
          n3 = key;
        }
        dn3 = dn2*10+n3;
        dispense = dn3;
        matrix.print(dn3);
        matrix.writeDisplay();
      break;

      case 3:
        count++;
        if (key == '^') {
          n4 = 0;
        } else {
          n4 = key;
        }
        dn4 = dn3*10+n4;
        dispense = dn4;
        matrix.print(dn4);
        matrix.writeDisplay();
      break;
      // resets if over 4 characters long (max of display)
      default:
        resetFunc();
    }

  }

}
