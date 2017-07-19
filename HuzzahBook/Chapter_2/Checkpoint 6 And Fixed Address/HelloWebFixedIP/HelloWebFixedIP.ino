#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

void mainPage() {
  Serial.println("Serving our page");
  server.send(200, "text/html", "Hello Web!!!");
}

void connectWifi() {
  Serial.print("Attempting to connect to wifi...");
  WiFi.config(IPAddress(192, 168, 42, 80), IPAddress(192, 168, 42, 1), IPAddress(192, 168, 42, 1));
  WiFi.begin("Your Network Name", "Your Wifi Password");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected! ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
}

void activateWebserver() {
  Serial.println("Starting the web server");
  server.on("/", mainPage);
  server.onNotFound(mainPage);
  server.begin(); 
}

void setup() {
  Serial.begin(9600);
  delay(100);
  Serial.println("Debugging activated");
  connectWifi();
  activateWebserver();
}

void loop() {
  server.handleClient();
}
