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

void(* resetFunc) (void) = 0; // reset function to restart the app & clear the key var
void loop() {

	char key = keypad.getKey();
	if (key != NO_KEY) {

		switch (key) {
			case '#' :
				Serial.println();
				Serial.println("Dispensing tickets");
				// need to add counter from opto-sensor
				for (uint16_t counter = 1; counter <= dispense; counter++)  {
					matrix.println(counter);
					Serial.println(counter);
					matrix.writeDisplay();
					delay(10);
				}
				Serial.println("Done dispensing... resetting...");
				delay(500);
				resetFunc(); // reset function to restart the app & clear the key var

			case '*' :
				// need interrupt to cancel ticket dispensing
				delay(500);
				resetFunc();
		}

		switch (count) {
			// first number to be entered
			case 0 :
				matrix.clear(); // clear display
				matrix.writeDisplay(); // write clear
				count++; // ++ the counter
				n1 = key; // set n1 to the key pressed
				dispense = n1; // set dispensed var to n1 (key)
				matrix.print(n1); // set display to show n1
				matrix.writeDisplay(); // write show n1
			break;
			// second number to be entered
			case 1 :
				count++;
				// funky fix for keypad not registering the zero key -
				if (key == '^') {
					n2 = 0;
				} else {
					n2 = key;
				}
				dn2 = n1*10+n2; // fix so if n1 was a 5, and n2 is a 3, then dn2 = 53
				dispense = dn2; // set dispense var to dn2
				matrix.print(dn2);
				matrix.writeDisplay();
			break;
			// third number to be entered
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
			// fourth number to be entered
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
		}

	}

}
