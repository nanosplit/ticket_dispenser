int tsignal = 2;
int tmotor = 7;
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

    while (count < 5) {
        digitalWrite(tmotor, HIGH);
        if (tstate == HIGH && lastTState == LOW) {
            count++;
            Serial.println(count);
        }
        lastTState = tstate;
        tstate=digitalRead(tsignal);
        digitalWrite(tmotor, LOW);
    }
    count = 0;
    delay(5000);
    
}
