Adafruit_7segment matrix = Adafruit_7segment();
//
//  methods
//
void(* resetFunc) (void) = 0;           // reset

void printMatrix(int keyed_num) {
  matrix.print(keyed_num);
  matrix.writeDisplay();
}

void motorOn(int dispenser_id) {
  digitalWrite(dispenser_id, HIGH);
}

void motorOff(int dispenser_id) {
  digitalWrite(dispenser_id, LOW);
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

void matrixInitialize() {
  matrix.begin(0x70);           // set the address of the 7 segment display
  matrix.setBrightness(1);      // set the brightness (1-15) - 1 = low :: 15 = incinerated corneas
  matrix.print(10000, DEC);     // write ---- to 7 segment display
  matrix.writeDisplay();
}

void cancelDispensing(int dispenser_id) {
  digitalWrite(dispenser_id, LOW);
  delay(1000);
  resetFunc();
}

void countTicket(int td, int los, int ps) {
  td++;           // td = tickets_dispensed
  los = now();    // los = last_opto_signal
  ps = 0;         // ps = previous_status
  printMatrix(td);
}
