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
  {'*',0,'#'}
};
byte rowPins[ROWS] = {8, 7, 6, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
// end keypad

int count = 0;
int first_key = 0;
int second_key = 0;
int third_key = 0;
int fourth_key = 0;

void setup() {
#ifndef __AVR_ATtiny85__
  Serial.begin(9600);
  Serial.println("7 Segment + Keypad Test");
#endif
  matrix.begin(0x70);
  matrix.setBrightness(1);
  matrix.print(1000, DEC);
  matrix.writeDisplay();
  matrix.print(0);
  matrix.writeDisplay();
  Serial.println("Ready for input");
}

void loop() {

  char key = keypad.getKey();
  if (key != NO_KEY) {

    count++;

    if (key == '#') {
      Serial.println();
      Serial.println("Dispensing tickets");
      delay(3000);
      Serial.println("Done dispensing... resetting...");
      count = 0;
      matrix.print(10000, DEC);
      matrix.writeDisplay();
      delay(500);
      matrix.print(0);
      matrix.writeDisplay();
      Serial.println("Ready for input");
    }

    if (key =='*') {
      // need interrupt to kill ticket dispensing
      Serial.println();
      Serial.println("Resetting...");
      matrix.print(10000, DEC);
      matrix.writeDisplay();
      delay(3000);
      count = 0;
      matrix.print(0);
      matrix.writeDisplay();
      Serial.println("Done resetting...");
      Serial.println("Ready for input");
    }

    if (count == 1) {
      matrix.writeDigitRaw(4, 0x00);
      matrix.writeDigitNum(0, key);

      matrix.writeDisplay();
      first_key = key;
      Serial.print(first_key);
    }
    else if (count == 2) {
      matrix.writeDigitNum(1,key);
      matrix.writeDisplay();
      second_key = key;
      Serial.print(second_key);
    }
    else if (count == 3) {
      matrix.writeDigitNum(3,key);
      matrix.writeDisplay();
      third_key = key;
      Serial.print(third_key);
    }
    else if (count == 4) {
      matrix.writeDigitNum(4,key);
      matrix.writeDisplay();
      fourth_key = key;
      Serial.print(fourth_key);
    }

    else if (count >= 5) {
      Serial.println();
      matrix.print(10000, DEC);
      matrix.writeDisplay();
      delay(500);
      matrix.print(0);
      matrix.writeDisplay();
      count = 0;
      Serial.println("Maximum reached... resetting...");
      delay(1000);
    }
  }

}
