void connectWifi() {
  // fill in later
}

void activateWebserver() {
  // fill in later
}

void setup() {
  Serial.begin(9600);
  delay(100);
  Serial.println(Debugging activated);
  connectWifi();
  activateWebserver();
}

void loop() {
  server.handleClient();
}
