int tsignal = 12;
int tmotor = 11;
int tstate = LOW;
int lastTState = LOW;
int count = 0;

void setup() {

    Serial.begin(9600);

    pinMode(tsignal, INPUT_PULLUP);
    pinMode(tmotor, OUTPUT);
    tstate = digitalRead(tsignal);

}

void loop() {

    while (count < 1) {
        digitalWrite(tmotor, HIGH);
        count ++;
    }

    digitalWrite(tmotor, LOW);
    count = 1;

}
