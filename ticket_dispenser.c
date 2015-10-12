#include "Time.h" // https://github.com/PaulStoffregen/Time
#include "Wire.h"
#include "Keypad.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_7segment matrix = Adafruit_7segment();
//
// Setup the keypad
//
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
	{1,2,3},
	{4,5,6},
	{7,8,9},
	{'*','^','#'}
};
byte rowPins[ROWS] = {8, 7, 6, 5};				//connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 3, 2};					//connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//
// setup variables
//
int count = 0;										// the number of digits that have been entered
int n1 = 0;											// first number entered
int n2 = 0;											// second number entered
int n3 = 0;											// thid number entered
int n4 = 0;											// fourth number entered
int dn2 = 0;											// 2 digit value (n1*10+n2) - fix for 7 segment display
int dn3 = 0;											// 2 digit value (dn2*10+n3) - fix for 7 segment display
int dn4 = 0;											// 2 digit value (dn3*10+n4) - fix for 7 segment display
//
// ticket dispenser variables
//
int tickets_requested = 0;							// number of tickets requested to dispense
int tickets_dispensed = 0;							// number of tickets that have been dispensed
int last_opto_signal = 0;							// determine the last opto_signal time
int current_time = 0;								// current time
//
// setup pins
//
const int opto_signal_pin = 12;					// opto-sensor pin# (blue wire on DL-1275)
const int dispenser_motor = 11;					// motor control pin# (white wire on DL-1275)
//
// counter state machine variables
//
int opto_signal_status = 0;							// listner for opto-sensor LOW / HIGH status
int current_status = 0;								// variable for current state machine status
int previous_status = 0;							// variable for the last state machine status
//
// initialize setup
//
void setup() {
#ifndef __AVR_ATtiny85__
	Serial.begin(9600);							// initialize serial connection
	Serial.println("Ticket Dispenser");				// hello
#endif
	pinMode(opto_signal_pin, INPUT_PULLUP);	// set the pin mode for the optical sensor
	pinMode(dispenser_motor, OUTPUT);		// set the pin mode for the dispenser motor
	matrix.begin(0x70);							// set the address of the 7 segment display
	matrix.setBrightness(1);						// set the brightness (1-15) - 1 = low - 15 = incinerated cornea's
	matrix.print(10000, DEC);						// display ---- on the display - just for show...
	matrix.writeDisplay();							// write the display
	delay(200);									// pause and make it look like we're doing something...
	matrix.clear();									// clear the display
	matrix.writeDisplay();							// write the clear
	matrix.printNumber(0);						// set the display to 0
	matrix.writeDisplay();							// write the 0 to the display
}
//
// start the main program
//
void(* resetFunc) (void) = 0;						// reset program function
void loop() {

	char key = keypad.getKey(); 					// get the value of the key pressed
//
// key event trigger
//
	if (key != NO_KEY) {
//
// Special key cases
//
		switch (key) {
			case '#' :								// case '#" is pressed on keypad'
				if (tickets_requested > 0) {			// check if any number inputted - if not : restart
					matrix.clear();  				// clear the display
					matrix.writeDisplay();			// write the cleared display
					delay(50);						// pause for the display
					matrix.println(0); 				// set display to zero for the counting
					matrix.writeDisplay(); 			// write the zero
					delay(50);						// pause for the display
//
// ticket dispensing function
//
					setTime();													// set now(); to zero for accurate timeing (no ticket detector)
					previous_status = digitalRead(opto_signal_pin);			// set prev state to what *should* default to 1 - needed for proper state machine function
					digitalWrite(dispenser_motor, HIGH); 						// turn motor on
					for (;tickets_dispensed < tickets_requested;) {			// start the loop to start dispensing and the state machine

						opto_signal_status = digitalRead(opto_signal_pin);	// get the status of the optical sensor (1 = HIGH, 0 = LOW)

						// state machine conditions
						/*
							setup so that tickets_dispensed++ is only counted if a gap has been detected in the tickets,
							which is indicated by a 0 (LOW) from the opto_signal_pin (opto_signal_status), but only if the
							last status was that there were no gaps detected. This ensures accurate counting and decreases
							false positives due to bad timing.
						*/
						if (opto_signal_status == 1) {
							current_status = 1;
							previous_status = 1;
						}

						if (opto_signal_status == 0) {
							current_status = 0;
						}

						if (current_status == 0 	&& previous_status == 1) {
							tickets_dispensed++;
							last_opto_signal = now();		// read the current time
							previous_status = 0;
						}

						matrix.print(tickets_dispensed);	// update the display with the current number of tickets dispensed
						matrix.writeDisplay();				// write the # dispensed

//
// end of roll catch
//
						current_time = now();									// get the current time
						int no_ticket_reset = current_time - last_opto_signal; // calculate difference
						if (no_ticket_reset >= 3) {								// reset if no ticket has ben detected in over 3 seconds
							digitalWrite(dispenser_motor, LOW);
							delay(1000);
							resetFunc();
						}
//
// cancel dispensing
//
						key = keypad.getKey();								// read key value
						if (key == '*') {											// reset if * pressed on keypad
							digitalWrite(dispenser_motor, LOW);
							delay(1000);
							resetFunc();
						}
					}

					delay(25);								// critical delay - fine tune exact stop to ensure tear bar aligns with perforations
					digitalWrite(dispenser_motor, LOW);	// turn motor off
					delay(1000);							// delay 1 second before restart
					resetFunc();							// restart
				}

				delay(500);						// if nothing was selected, but # pressed, delay 1/2 second then reset
				resetFunc();						// reset

			case '*' :								// case '*' is pressed on keypad
			//
			// need interrupt to cancel ticket dispensing - will need to go in dispensing loop
			//
			delay(500); 							// if * is pressed, pause for 1/2 second then reset
			resetFunc();							// reset
		}
//
// determine how many numbers have been entered
//
		switch (count) {
			// first number entered
			case 0 :
				matrix.clear();
				matrix.writeDisplay();
				count++;
				n1 = key;
				tickets_requested = n1;
				matrix.print(n1);
				matrix.writeDisplay();
			break;
			// second number entered
			case 1 :
				count++;
				if (key == '^') {
					n2 = 0;
				} else {
					n2 = key;
				}
				dn2 = n1*10+n2;
				tickets_requested = dn2;
				matrix.print(dn2);
				matrix.writeDisplay();
			break;
			// third number entered
			case 2:
				count++;
				if (key == '^') {
					n3 = 0;
				} else {
					n3 = key;
				}
				dn3 = dn2*10+n3;
				tickets_requested = dn3;
				matrix.print(dn3);
				matrix.writeDisplay();
			break;
			// fourth number entered
			case 3:
				count++;
				if (key == '^') {
					n4 = 0;
				} else {
					n4 = key;
				}
				dn4 = dn3*10+n4;
				tickets_requested = dn4;
				matrix.print(dn4);
				matrix.writeDisplay();
			break;
//
// resets if over 4 characters long (max of display)
//
			default:
			resetFunc();
		}

	}

}
