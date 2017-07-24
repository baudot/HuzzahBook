#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#define WEIGHT_SENSOR A0
#define WEIGHT_THRESHOLD 500

ESP8266WebServer server(80);

void mainPage() {
  Serial.println("Serving our page");
  int weight_sensor_reading = analogRead(WEIGHT_SENSOR);
  if (weight_sensor_reading > WEIGHT_THRESHOLD) {
    server.send(200, "text/html", "<H1>THERE'S SOMEONE IN MY CHAIR!</H1>");
  } else {
    server.send(200, "text/html", "<H2>My chair is unoccupied.</H2>");    
  }
}

void connectWifi() {
  Serial.print("Attempting to connect to wifi...");
  WiFi.config(IPAddress(192, 168, 0, 240), IPAddress(192, 168, 0, 1), IPAddress(192, 168, 0, 1));
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
  Serial.println("Starting the web server");
  server.on("/", mainPage);
  server.onNotFound(mainPage);
  server.begin(); 
}

void setup() {
  pinMode(WEIGHT_SENSOR, INPUT);
  Serial.begin(9600);
  delay(100);
  Serial.println("Debugging activated");
  connectWifi();
  activateWebserver();
}

void loop() {
  server.handleClient();
}


