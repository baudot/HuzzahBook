#include "AdafruitIO_WiFi.h"
#include <ESP8266WebServer.h>

#define WEIGHT_SENSOR A0
#define WEIGHT_THRESHOLD 500
#define SOLENOID 0
#define IO_USERNAME "Your Adafruit IO Username Goes Here"
#define IO_KEY "Your Adafruit IO Key, Found in io.adafruit settings, goes here."

ESP8266WebServer server(80);
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, "YOUR NETWORK NAME", "YOUR WIFI PASSWORD");
AdafruitIO_Feed *chair_occupied = io.feed("ChairOccupied");
AdafruitIO_Feed *trigger_trap = io.feed("TriggerTrap");

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
  io.connect();
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("Connected! ");
  WiFi.config(IPAddress(192, 168, 42, 80), IPAddress(192, 168, 42, 1), IPAddress(192, 168, 42, 1));
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
}

void publish_status() {
  static boolean occupied = false;
  static boolean previously_occupied = true;
  int weight_sensor_reading = analogRead(WEIGHT_SENSOR);
  if (weight_sensor_reading > WEIGHT_THRESHOLD) occupied = true;
  else occupied = false;
  if (occupied != previously_occupied) {
    if (occupied) {
      chair_occupied->save("Occupied");
      Serial.println("Chair: Occupied");
    } else {
      chair_occupied->save("Empty");
      Serial.println("Chair: Empty");
    }
    previously_occupied = occupied;
  }
}

void activateWebserver() {
  Serial.println("Starting the web server");
  server.on("/", mainPage);
  server.onNotFound(mainPage);
  server.begin(); 
}

void fire_trap(AdafruitIO_Data *data) {
  int fire_trap = data->toInt();
  if (fire_trap) {
    digitalWrite(SOLENOID, HIGH);
    Serial.println("Firing trap");
    delay(2000);
    trigger_trap->save(false);
    digitalWrite(SOLENOID, LOW);
  }
  else digitalWrite(SOLENOID, LOW);
}

void setup() {
  pinMode(WEIGHT_SENSOR, INPUT);
  pinMode(SOLENOID, OUTPUT);
  Serial.begin(9600);
  delay(100);
  Serial.println("Debugging activated");
  trigger_trap->onMessage(fire_trap);
  connectWifi();
  activateWebserver();
}

void loop() {
  io.run();
  server.handleClient();
  publish_status();
}
