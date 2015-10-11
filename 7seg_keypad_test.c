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

void setup() {
#ifndef __AVR_ATtiny85__
	Serial.begin(9600);
	Serial.println("7 Segment + Keypad Test");
#endif
	matrix.begin(0x70);
	matrix.setBrightness(1);
	matrix.print(10000, DEC);
	matrix.writeDisplay();
	delay(500);
	matrix.clear();
	matrix.writeDisplay();
	matrix.printNumber(0);
	matrix.writeDisplay();
}

void(* resetFunc) (void) = 0;
void loop() {

	char key = keypad.getKey();
	if (key != NO_KEY) {

		switch (key) {
			case '#' :
				Serial.println();
				Serial.println("Dispensing tickets");
				for (uint16_t counter = 1; counter <= dn2; counter++)  {
					matrix.println(counter);
					Serial.println(counter);
					matrix.writeDisplay();
					delay(10);
				}
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
				matrix.print(dn4);
				matrix.writeDisplay();
			break;
		}

		switch (count) {
			case 0 :
				matrix.print(n1);
				matrix.writeDisplay();
			break;
		}

		if (count > 4) {
			matrix.clear();
			matrix.writeDisplay();
			resetFunc();
		}

	}

}
