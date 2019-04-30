void setup() {

  // initialize serial
  Serial.begin(9600);
}

void loop() {

  int long time_start = millis();

  int i = 0;
  while (i < 2500) {
    i++;
  }

  int long time_ad = millis();

  Serial.println(time_start - time_ad);

  delay(2000);

  Serial.println("Restarting");

}
