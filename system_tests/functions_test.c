// Ticket dispenser test script for Deltronic Labs DL-1275 ticket dispenser & Arduino Uno

int tsignal = 12; // opto-sensor pin# (blue wire on DL-1275)
int tmotor = 11; // motor control pin# (white wire on DL-1275)
int tcount = 0; // counter for # of tickets to dispense
int tsignal_status = 0; // listner for opto-sensor LOW / HIGH status

void setup() {

  // initialize serial
  Serial.begin(9600);

  // setup the pins
  pinMode(tsignal, INPUT_PULLUP);
  pinMode(tmotor, OUTPUT);
}

void loop() {

  // read the LOW / HIGH status of tsignal (opto-sensor)
  int tsignal_status = digitalRead(tsignal);

  // output the LOW / HIGH status of opto-sensor
  //Serial.println(tsignal_status);

  // turn motor on when opto-sensor is broken - default off (LOW)
  if (tsignal_status == LOW)
  {
    Serial.println("Signal status is LOW");
  }
  else if (tsignal_status == HIGH) {
    Serial.println("Signal status is HIGH");
  } else {
    Serial.println("Nothing detected...");
  }
}
