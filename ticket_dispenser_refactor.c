#include <Time.h> // https://github.com/PaulStoffregen/Timea
#include <TimeLib.h>
#include <Wire.h>
#include <Keypad.h>
#include <Adafruit_LEDBackpack.h>
#include <Adafruit_GFX.h>

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
byte rowPins[ROWS] = {8, 7, 6, 5};        //connect to the row pinouts of the keypad
byte colPins[COLS] = {4, 3, 2};         //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//
// setup variables
//
int count = 0;  // the number of digits that have been entered
int n1 = 0;   // first number entered
int n2 = 0;     // second number entered
int n3 = 0;   // thid number entered
int n4 = 0;     // fourth number entered
int dn2 = 0;    // 2 digit value (n1*10+n2) - fix for 7 segment display
int dn3 = 0;    // 2 digit value (dn2*10+n3) - fix for 7 segment display
int dn4 = 0;    // 2 digit value (dn3*10+n4) - fix for 7 segment display
//
// ticket dispenser variables
//
int tickets_requested = 0;
int tickets_dispensed = 0;
int last_opto_signal = 0;
int current_time = 0;
//
// setup pins
//
const int opto_signal_pin = 12;         // opto-sensor pin# (blue wire on DL-1275)
const int dispenser_motor = 11;         // motor control pin# (white wire on DL-1275)
//
// counter state machine variables
//
int opto_signal_status = 0;             // opto-sensor LOW / HIGH status
int current_status = 0;               // current state machine status
int previous_status = 0;              // last state machine status
//
// initialize setup
//
void setup() {
  Serial.begin(9600);
  pinMode(opto_signal_pin, INPUT_PULLUP);
  pinMode(dispenser_motor, OUTPUT);
  matrix.begin(0x70);             // set the address of the 7 segment display
  matrix.setBrightness(1);      // set the brightness (1-15) - 1 = low - 15 = incinerated cornea's
  initializeDisplay();
}
//
// start the main program
//
void(* resetFunc) (void) = 0;           // reset program function
void loop() {

  char key = keypad.getKey();           // get the value of the key pressed
//
// key event trigger
//
  if (key != NO_KEY) {
//
// Special key cases
//
    switch (key) {
      case '#' :
        if (tickets_requested > 0) {
          clearDisplay();
//
// ticket dispensing function
//
          setTime(0);
          previous_status = digitalRead(opto_signal_pin);
          motorOn();
          for (;tickets_dispensed < tickets_requested;) {

            // 1 = HIGH = no ticket gap detected
            // 0 = LOW = ticket gap detected
            opto_signal_status = digitalRead(opto_signal_pin);

            // state machine conditions
            if (opto_signal_status == 1) {
              current_status = 1;
              previous_status = 1;
            }

            if (opto_signal_status == 0) {
              current_status = 0;
            }

            if (current_status == 0 && previous_status == 1) {
              tickets_dispensed++;
              last_opto_signal = now();
              previous_status = 0;

              if (tickets_dispensed == tickets_requested) {
                printDisplay(tickets_requested);
                motorOff();
                break;
              }

            }

            printDisplay(tickets_dispensed);
//
// end of roll catch
//
            current_time = now();
            int no_ticket_reset = current_time - last_opto_signal;
            // reset if no ticket has ben detected in over 3 seconds
            if (no_ticket_reset >= 3) {
             hardReset();
            }
//
// cancel dispensing
//
            key = keypad.getKey();
            // reset if * pressed on keypad
            if (key == '*') {
              hardReset();
            }

          }

          hardReset();              // restet
        }

      // if * is pressed, reset
      case '*' :
      hardReset();              // reset
    }
//
// count how many numbers have been entered
//
    switch (count) {
      // first number entered
      case 0 :
        if (key == '^') {
          n1 = 0;
        } else {
          matrix.clear();
          matrix.writeDisplay();
          count++;
          n1 = key;
          tickets_requested = n1;
          printDisplay(n1);
        }
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
        printDisplay(dn2);
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
        printDisplay(dn3);
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
        printDisplay(dn4);
      break;
//
// over 4 character inputted reset (max of display)
//
      default:
      resetFunc();
    }

  }

}

void clearDisplay() {
  matrix.clear();
  matrix.writeDisplay();
}

void initializeDisplay() {
  matrix.print(10000, DEC);
  matrix.writeDisplay();
  delay(200);
  clearDisplay();
  matrix.printNumber(0);
  matrix.writeDisplay();
}

void printDisplay(int n) {
  matrix.print(n);
  matrix.writeDisplay();
}

void motorOn() {
  digitalWrite(dispenser_motor, HIGH);
}

void motorOff() {
  delay(100);
  digitalWrite(dispenser_motor, LOW);
}

void hardReset() {
  motorOff();
  delay(100);
  resetFunc();
}
