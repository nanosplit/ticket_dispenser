#include "Time.h" // https://github.com/PaulStoffregen/Time
#include "Wire.h"
#include "Keypad.h"
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_7segment matrix = Adafruit_7segment();
//
// keypad
//
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {1,2,3},
  {4,5,6},
  {7,8,9},
  {'*','^','#'}
};
byte rowPins[ROWS] = {8, 7, 6, 5}; // row pinouts
byte colPins[COLS] = {4, 3, 2};    // column pinouts
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//
// keypad number variables
//
int count = 0;  // number of key presses (n1, n2, n3, n4)
int n1 = 0;     // first number entered
int n2 = 0;     // second number entered
int n3 = 0;     // thid number entered
int n4 = 0;     // fourth number entered
int dn2 = 0;    // 2 digit value (n1*10+n2) - fix for 7 segment display
int dn3 = 0;    // 2 digit value (dn2*10+n3) - fix for 7 segment display
int dn4 = 0;    // 2 digit value (dn3*10+n4) - fix for 7 segment display
//
// dispenser variables
//
int tickets_requested = 0;
int tickets_dispensed = 0;
int last_opto_signal = 0;
int current_time = 0;
//
// arduino pins
//
const int opto_signal_pin = 12;         // opto-sensor pin# (blue wire on DL-1275)
const int dispenser_motor = 11;         // motor control pin# (white wire on DL-1275)
//
// counter state machine variables
//
int opto_signal_status = 0;           // opto-sensor LOW / HIGH status
int current_status = 0;               // current state machine status
int previous_status = 0;              // last state machine status
//
// initialize setup
//
void setup() {
  Serial.begin(9600);
  pinMode(opto_signal_pin, INPUT_PULLUP);
  pinMode(dispenser_motor, OUTPUT);
  matrix.begin(0x70);           // set the address of the 7 segment display
  matrix.setBrightness(1);      // set the brightness (1-15) - 1 = low :: 15 = incinerated corneas
  matrix.print(10000, DEC);
  matrix.writeDisplay();
  delay(200);
  matrixReset();
  Serial.println("ready...");
}
//
// main
//
void(* resetFunc) (void) = 0;           // reset
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
          matrixReset();
//
// ticket dispensing function
//
          setTime(0);
          previous_status = digitalRead(opto_signal_pin);
          motorOn();
          while (tickets_dispensed < tickets_requested) {

            if (tickets_requested == 1) {
              delay(490);   // varies based on length of each ticket - longer ticket = higher delay
              motorOff();
              delay(1000);
              resetFunc();
            }

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

            // count tickets dispensed
            for (;current_status == 0 && previous_status == 1;) {

              if (tickets_dispensed == (tickets_requested - 1)) {
                delay(100);   // varies based on length of each ticket - longer ticket = higher delay
                printMatrix(tickets_requested);
                cancelDispensing();
              }

              if (digitalRead(opto_signal_pin) == 1) {
                countTicket();
              }

              // reset if *
              key = keypad.getKey();
              if (key == '*') {
                cancelDispensing();
              }
            }
//
// end of roll catch
//
            current_time = now();
            int no_ticket_reset = current_time - last_opto_signal;
            if (no_ticket_reset >= 3) {
              cancelDispensing();
            }

            key = keypad.getKey();
            if (key == '*') {
              cancelDispensing();
            }
          }
        }
      // if * is pressed, reset
      case '*' :
      cancelDispensing();
    }
//
// count how many numbers have been entered
//
    switch (count) {
      // first number entered
      case 0 :
        clearMatrix();
        count++;
        n1 = key;
        tickets_requested = n1;
        printMatrix(n1);
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
        printMatrix(dn2);
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
        printMatrix(dn3);
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
        printMatrix(dn4);
      break;
//
// over 4 character input reset (max of display)
//
      default:
      resetFunc();
    }
  }
}

void printMatrix(int keyed_num) {
  matrix.print(keyed_num);
  matrix.writeDisplay();
}

void motorOn() {
  digitalWrite(dispenser_motor, HIGH);
}

void motorOff() {
  digitalWrite(dispenser_motor, LOW);
}

void clearMatrix() {
  matrix.clear();
  matrix.writeDisplay();
}

void matrixReset() {
  matrix.clear();
  matrix.writeDisplay();
  matrix.printNumber(0);
  matrix.writeDisplay();
}

void cancelDispensing() {
  digitalWrite(dispenser_motor, LOW);
  delay(1000);
  resetFunc();
}

void countTicket() {
  tickets_dispensed++;
  last_opto_signal = now();
  previous_status = 0;
  printMatrix(tickets_dispensed);
}
