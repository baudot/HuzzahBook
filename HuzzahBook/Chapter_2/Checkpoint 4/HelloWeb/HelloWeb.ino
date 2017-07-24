#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

void connectWifi() {
  Serial.print("Attempting to connect to wifi...");
  WiFi.begin("YOUR NETWORK NAME", "YOUR WIFI PASSWORD");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected! ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
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
